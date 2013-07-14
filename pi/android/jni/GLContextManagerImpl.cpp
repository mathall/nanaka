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

#include "GLContextManagerImpl.h"

#include "NanakaNativeWindowImpl.h"

GLContextManager* GLContextManager::Create()
{
	return new GLContextManagerImpl();
}

void GLContextManagerImpl::CreateContext(NanakaNativeWindow& nativeWindow)
{
	DestroyContext(nativeWindow);

	m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(m_display, 0, 0);

	ChooseConfig();

	const EGLint attrib_list [] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
	m_context = eglCreateContext(m_display, m_config, 0, attrib_list);
}

void GLContextManagerImpl::CreateSurface(NanakaNativeWindow& nativeWindow)
{
	ANativeWindow_setBuffersGeometry(
		&static_cast<NanakaNativeWindowImpl&>(nativeWindow).m_window,
		0, 0, m_format);

	DestroySurface();

	m_surface = eglCreateWindowSurface(m_display, m_config,
		&static_cast<NanakaNativeWindowImpl&>(nativeWindow).m_window, 0);
	eglMakeCurrent(m_display, m_surface, m_surface, m_context);

	eglSwapInterval(m_display, 0);
}

void GLContextManagerImpl::DestroyContext(NanakaNativeWindow& nativeWindow)
{
	if (m_display == EGL_NO_DISPLAY)
	{
		return;
	}

	DestroySurface();

	if (m_context != EGL_NO_CONTEXT)
	{
		eglDestroyContext(m_display, m_context);
	}

	eglTerminate(m_display);

	m_surface = EGL_NO_SURFACE;
	m_context = EGL_NO_CONTEXT;
	m_display = EGL_NO_DISPLAY;
}

void GLContextManagerImpl::DestroySurface()
{
	eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

	if (m_surface != EGL_NO_SURFACE)
	{
		eglDestroySurface(m_display, m_surface);
	}
}

void GLContextManagerImpl::Swap(NanakaNativeWindow& nativeWindow)
{
	eglSwapBuffers(m_display, m_surface);
}

void GLContextManagerImpl::ChooseConfig()
{
	const EGLint attributes[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_DEPTH_SIZE, 16,
		EGL_STENCIL_SIZE, 0,
		EGL_ALPHA_SIZE, 0,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};

	EGLint numConfigs;
	eglChooseConfig(m_display, attributes, &m_config, 1, &numConfigs);
	eglGetConfigAttrib(m_display, m_config, EGL_NATIVE_VISUAL_ID, &m_format);
}
