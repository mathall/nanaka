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

#ifndef NANAKA_RENDERER_RENDERRESOURCEHANDLE_H
#define NANAKA_RENDERER_RENDERRESOURCEHANDLE_H

#include <functional>

class RenderResourceHandle final
{
public:

	RenderResourceHandle() = default;

	bool operator==(const RenderResourceHandle& resourceHandle) const;

	static RenderResourceHandle New();

	static const RenderResourceHandle Invalid;

private:

	friend struct std::hash<RenderResourceHandle>;

	explicit RenderResourceHandle(int handle);

	int m_handle;

	static int s_nextId;
};

inline RenderResourceHandle::RenderResourceHandle(int handle)
{
	m_handle = handle;
}

inline bool RenderResourceHandle::operator==(
	const RenderResourceHandle& resourceHandle) const
{
	return m_handle == resourceHandle.m_handle;
}

inline RenderResourceHandle RenderResourceHandle::New()
{
	return RenderResourceHandle(s_nextId++);
}

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmismatched-tags"
#endif // defined(__clang__)

namespace std {
template<>
struct hash<RenderResourceHandle> {
public:
	size_t operator()(const RenderResourceHandle& resourceHandle) const
	{
		return std::hash<int>()(resourceHandle.m_handle);
	}
};
} // namespace std

#if defined(__clang__)
#pragma clang diagnostic pop
#endif // defined(__clang__)

#endif // NANAKA_RENDERER_RENDERRESOURCEHANDLE_H
