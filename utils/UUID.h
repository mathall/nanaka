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

#ifndef NANAKA_UTILS_UUID_H
#define NANAKA_UTILS_UUID_H

#if !defined(SINGLE_THREADED)
#include <atomic>
#endif // !defined(SINGLE_THREADED)
#include <functional>

/**
 * A not so UU ID class implementation.
 */
class UUID final
{
public:

	UUID();

	UUID(const UUID& uuid);

	UUID& operator=(const UUID& uuid);

	bool operator==(const UUID& uuid) const;

	static void Initialize();

	static UUID New();

	static const UUID Invalid;

private:

	friend struct std::hash<UUID>;

	UUID(int priv);

#if defined(SINGLE_THREADED)
	int m_private;
#else
	std::atomic<int> m_private;
#endif // defined(SINGLE_THREADED)

#if defined(SINGLE_THREADED)
	static int s_nextId;
#else
	static std::atomic<int> s_nextId;
#endif // defined(SINGLE_THREADED)
};

inline UUID::UUID()
{
#if defined(SINGLE_THREADED)
	m_private = Invalid.m_private;
#else
	m_private = Invalid.m_private.load();
#endif // defined(SINGLE_THREADED)
}

inline UUID::UUID(int priv)
{
#if defined(SINGLE_THREADED)
	m_private = priv;
#else
	m_private = {priv};
#endif // defined(SINGLE_THREADED)
}

inline UUID::UUID(const UUID& uuid)
{
#if defined(SINGLE_THREADED)
	m_private = uuid.m_private;
#else
	m_private = uuid.m_private.load();
#endif // defined(SINGLE_THREADED)
}

inline bool UUID::operator==(const UUID& uuid) const
{
	return m_private == uuid.m_private;
}

inline UUID& UUID::operator=(const UUID& uuid)
{
#if defined(SINGLE_THREADED)
	m_private = uuid.m_private;
#else
	m_private = uuid.m_private.load();
#endif // defined(SINGLE_THREADED)
	return *this;
}

inline void UUID::Initialize()
{
#if defined(SINGLE_THREADED)
	s_nextId = 1;
#else
	s_nextId = {1};
#endif // defined(SINGLE_THREADED)
}

inline UUID UUID::New()
{
	return UUID(s_nextId++);
}

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmismatched-tags"
#endif // defined(__clang__)

namespace std {
template<>
struct hash<UUID> {
public:
	size_t operator()(const UUID& uuid) const
	{
#if defined(SINGLE_THREADED)
		return std::hash<int>()(uuid.m_private);
#else
		return std::hash<int>()(uuid.m_private.load());
#endif // defined(SINGLE_THREADED)
	}
};
} // namespace std

#if defined(__clang__)
#pragma clang diagnostic pop
#endif // defined(__clang__)

#endif // NANAKA_UTILS_UUID_H
