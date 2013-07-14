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

#include "JNIHelper.h"

static JavaVM* g_JVM = NULL;

#if defined(__GNUC__) && !defined(__clang__)
static __thread JNIEnv* g_env = NULL;
#else
static thread_local JNIEnv* g_env = NULL;
#endif // defined(__GNUC__) && !defined (__clang__)

void initJNIHelper(JavaVM* jvm)
{
	g_JVM = jvm;
}

JNIEnv* getEnv()
{
	JNIEnv* env = g_env;
	if (!env)
	{
		if (g_JVM->GetEnv(
			reinterpret_cast<void**>(&env), JNI_VERSION_1_4) == JNI_EDETACHED)
		{
			if (g_JVM->AttachCurrentThread(&env, 0) != JNI_OK)
			{
				return NULL;
			}
		}
		g_env = env;
	}
	return env;
}

void jniCall(
	jobject object,
	std::string method,
	std::string signature,
	std::function<void(JNIEnv*, jobject, jmethodID)> callback)
{
	JNIEnv* env = getEnv();
	g_JVM->AttachCurrentThread(&env, NULL);

	jclass objclass = env->GetObjectClass(object);
	jmethodID methodID = env->GetStaticMethodID(
		objclass, method.c_str(), signature.c_str());

	if (!methodID)
	{
		g_JVM->DetachCurrentThread();
		return;
	}

	callback(env, object, methodID);

	g_JVM->DetachCurrentThread();
}
