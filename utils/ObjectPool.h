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

#ifndef NANAKA_UTILS_MEMORYPOOL_H
#define NANAKA_UTILS_MEMORYPOOL_H

#include <memory>
#include <vector>

template<typename T, typename C>
class ObjectPool final
{
public:

	void PutObject(std::unique_ptr<T> object);
	std::unique_ptr<T> GetObject();

private:

	std::unique_ptr<T> NewObject();

	void Clear(T& object);

	std::vector<std::unique_ptr<T>> m_objectPool;
};

template<typename T, typename C>
void ObjectPool<T,C>::PutObject(std::unique_ptr<T> object)
{
	if (auto objectPtr = object.get())
	{
		Clear(*objectPtr);
		m_objectPool.push_back(std::move(object));
	}
}

template<typename T, typename C>
std::unique_ptr<T> ObjectPool<T,C>::GetObject()
{
	if (m_objectPool.size() > 0)
	{
		auto object = std::move(m_objectPool.back());
		m_objectPool.pop_back();
		return object;
	}
	else
	{
		return NewObject();
	}
}

template<typename T, typename C>
std::unique_ptr<T> ObjectPool<T,C>::NewObject()
{
	std::unique_ptr<T> object(new T());
	Clear(*object);
	return object;
}

template<typename T, typename C>
void ObjectPool<T,C>::Clear(T& object)
{
	C clearer;
	clearer(object);
}

#endif // NANAKA_UTILS_MEMORYPOOL_H
