/*
 * Copyright (c) 2013, Mathias Hällman. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "Nanaka.h"

#include <cassert>
#include <memory>

#include <android/native_window_jni.h>

#include "nanaka/main/Nanaka.h"
#include "nanaka/renderer/Renderer.h"
#include "nanaka/input/InputEvent.h"

#include "Application.h"
#include "FileManagerImpl.h"
#include "JNIHelper.h"
#include "NanakaNativeWindowImpl.h"

std::unique_ptr<Nanaka> g_nanaka;
std::unique_ptr<AndroidApplication> g_application;
std::unique_ptr<NanakaNativeWindowImpl> g_window;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *pvt)
{
	JNIEnv *env;

	if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_4) != JNI_OK)
	{
		return -1;
	}

	initJNIHelper(vm);

	return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_Initialize(
	JNIEnv* env, jobject obj, jstring apkFilePath, jobject application)
{
	const char* apkFilePathCStr;
	jboolean isCopy;
	apkFilePathCStr = env->GetStringUTFChars(apkFilePath, &isCopy);
	g_APK = zip_open(apkFilePathCStr, 0, NULL);

	g_application = std::unique_ptr<AndroidApplication>(
		new AndroidApplication(env->NewGlobalRef(application)));
	g_nanaka = std::unique_ptr<Nanaka>(new Nanaka(*g_application));
}

JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_Start(
	JNIEnv* env, jobject obj)
{
	g_nanaka->StartThread();
}

JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_OnPause(
	JNIEnv* env, jobject obj)
{
	g_nanaka->OnPause();
}

JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_OnResume(
	JNIEnv* env, jobject obj)
{
	g_nanaka->OnResume();
}

JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_OnShutdown(
	JNIEnv* env, jobject obj)
{
	g_nanaka->KillThread();
	g_nanaka = nullptr;
	g_application = nullptr;
}

JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_OnSurfaceCreated(
	JNIEnv* env, jobject obj, jobject surface)
{
	assert(surface);
	assert(!g_window);
	g_window = std::unique_ptr<NanakaNativeWindowImpl>(
		new NanakaNativeWindowImpl(*ANativeWindow_fromSurface(env, surface)));
	g_renderer->OnWindowCreated(g_window.get());
}

JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_OnSurfaceDestroyed(
	JNIEnv* env, jobject obj)
{
	assert(g_window);
	g_renderer->OnWindowDestroyed();
	ANativeWindow_release(&g_window->m_window);
	g_window = nullptr;
}

JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_OnSurfaceChanged(
	JNIEnv* env, jobject obj, int w, int h, float dpi)
{
	assert(g_window);
	DisplayProperties displayProps;
	displayProps.m_realSize = Vec2f(w, h);
	displayProps.m_size = Vec2f(w, h);
	displayProps.m_dpi = dpi;
	g_nanaka->SetDisplayProperties(displayProps);
	g_renderer->OnWindowChanged();
}

JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_AddInputEvent(
	JNIEnv* env, jobject obj, float x, float y, int action, int pointerId)
{
	InputEvent event;
	event.m_type = MotionInputEventType;
	event.m_action = static_cast<InputEventAction>(action);
	event.m_position = Vec2f(x, y);
	event.m_pointerId = pointerId;

	g_nanaka->AddInputEvent(event);
}
