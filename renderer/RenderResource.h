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

#ifndef NANAKA_RENDERER_RENDERRESOURCE_H
#define NANAKA_RENDERER_RENDERRESOURCE_H

#include "renderer/GLResource.h"
#include "renderer/RenderResourceHandle.h"

enum RenderResourceType
{
	TextureRenderResourceType,
	FrameBufferRenderResourceType,
};

class RenderResource : public GLResource
{
public:

	explicit RenderResource(RenderResourceType type);
	virtual ~RenderResource(){}

	RenderResourceType GetType() const;
	RenderResourceHandle GetHandle() const;

private:

	RenderResourceType m_type;
	RenderResourceHandle m_handle;
};

inline RenderResource::RenderResource(RenderResourceType type)
	: GLResource()
	, m_type(type)
	, m_handle(RenderResourceHandle::New())
{
}

inline RenderResourceType RenderResource::GetType() const
{
	return m_type;
}

inline RenderResourceHandle RenderResource::GetHandle() const
{
	return m_handle;
}

#endif // NANAKA_RENDERER_RENDERRESOURCE_H
