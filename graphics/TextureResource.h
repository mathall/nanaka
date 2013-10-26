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

#ifndef NANAKA_GRAPHICS_TEXTURERESOURCE_H
#define NANAKA_GRAPHICS_TEXTURERESOURCE_H

#include "renderer/Renderer.h"
#include "renderer/RenderResourceHandle.h"
#include "resource/Resource.h"

class TextureResource final : public Resource
{
public:

	explicit TextureResource(RenderResourceHandle renderResourceHandle);
	~TextureResource();

	TextureResource(TextureResource&) = delete;
	TextureResource& operator=(const TextureResource&) = delete;

	RenderResourceHandle GetTextureHandle() const;

private:

	RenderResourceHandle m_renderResourceHandle;
};

inline TextureResource::TextureResource(
	RenderResourceHandle renderResourceHandle)
	: m_renderResourceHandle(renderResourceHandle)
{
}

inline TextureResource::~TextureResource()
{
	g_renderer->DestroyRenderResource(m_renderResourceHandle);
}

inline RenderResourceHandle TextureResource::GetTextureHandle() const
{
	return m_renderResourceHandle;
}

#endif // NANAKA_GRAPHICS_TEXTURERESOURCE_H
