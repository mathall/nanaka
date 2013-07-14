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

#ifndef NANAKA_PI_ANDROID_JNI_NANAKA_H
#define NANAKA_PI_ANDROID_JNI_NANAKA_H

#include "JNIHelper.h"

extern "C"
{
	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *pvt);
	JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_Initialize(
		JNIEnv* env, jobject obj, jstring apkFilePath, jobject application);
	JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_Start(
		JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_OnPause(
		JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_OnResume(
		JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_OnShutdown(
		JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_OnSurfaceCreated(
		JNIEnv* env, jobject obj, jobject surface);
	JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_OnSurfaceDestroyed(
		JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_OnSurfaceChanged(
		JNIEnv* env, jobject obj, int w, int h, float dpi);
	JNIEXPORT void JNICALL Java_com_madhobo_nanaka_Nanaka_AddInputEvent(
		JNIEnv* env, jobject obj, float x, float y, int action, int pointerId);
};

#endif // NANAKA_PI_ANDROID_JNI_NANAKA_H
