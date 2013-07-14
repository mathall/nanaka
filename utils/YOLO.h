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

#ifndef NANAKA_UTILS_YOLO_H
#define NANAKA_UTILS_YOLO_H

template<typename C>
class YOLO
{
public:

	virtual ~YOLO(){}

	static void Instantiate();
	static void DestroyInstance();
	static C* GetInstance();

	virtual void OnDestroy(){}

private:

	static C* s_instance;
};

template<typename C>
C* YOLO<C>::s_instance = NULL;

template<typename C>
inline void YOLO<C>::Instantiate()
{
	if (s_instance == NULL)
	{
		s_instance = new C();
	}
}

template<typename C>
inline void YOLO<C>::DestroyInstance()
{
	s_instance->OnDestroy();
	delete s_instance;
	s_instance = NULL;
}

template<typename C>
inline C* YOLO<C>::GetInstance()
{
	return s_instance;
}

#endif // NANAKA_UTILS_YOLO_H
