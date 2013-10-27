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

#ifndef NANAKA_GRAPHICS_FRAMEBUFFER_H
#define NANAKA_GRAPHICS_FRAMEBUFFER_H

#include "renderer/Renderer.h"
#include "renderer/RenderResourceHandle.h"

class FrameBuffer final
{
public:

	explicit FrameBuffer(Vec2f size);
	~FrameBuffer();

	FrameBuffer(FrameBuffer&) = delete;
	FrameBuffer& operator=(const FrameBuffer&) = delete;

	RenderResourceHandle GetFrameBufferHandle() const;

private:

	RenderResourceHandle m_renderResourceHandle;
};

inline FrameBuffer::FrameBuffer(Vec2f size)
	: m_renderResourceHandle(g_renderer->GenerateFrameBuffer(size))
{
}

inline FrameBuffer::~FrameBuffer()
{
	if (g_renderer)
	{
		g_renderer->DestroyRenderResource(m_renderResourceHandle);
	}
}

inline RenderResourceHandle FrameBuffer::GetFrameBufferHandle() const
{
	return m_renderResourceHandle;
}

#endif // NANAKA_GRAPHICS_FRAMEBUFFER_H
