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

#include <memory>

#include "renderer/GLResource.h"
#include "renderer/Renderer.h"
#include "resource/Resource.h"

class TextureGLResource final : public GLResource
{
public:

	TextureGLResource(int width, int height, std::unique_ptr<GLvoid> pixels);

	/**
	 * GLResource implementation.
	 */
	void Build() override;
	void Destroy() override;

	const int m_width;
	const int m_height;

	GLuint m_texHandle;
	std::unique_ptr<GLvoid> m_pixels;
};

inline void TextureGLResource::Destroy()
{
	glDeleteTextures(1, &m_texHandle);
}

class TextureResource final : public Resource
{
public:

	explicit TextureResource(std::shared_ptr<TextureGLResource> GLResource);
	~TextureResource();

	TextureResource(TextureResource&) = delete;
	TextureResource& operator=(const TextureResource&) = delete;

	GLuint GetTextureHandle();

private:

	// shared_ptr because GLResources may be queued up on the GLResourceManager.
	std::shared_ptr<TextureGLResource> m_GLResource;
};

inline TextureResource::TextureResource(
	std::shared_ptr<TextureGLResource> GLResource)
	: m_GLResource(GLResource)
{
	g_renderer->QueueGLResourceForBuild(m_GLResource);
}

inline TextureResource::~TextureResource()
{
	g_renderer->QueueGLResourceForDestruction(m_GLResource);
}

inline GLuint TextureResource::GetTextureHandle()
{
	return m_GLResource->m_texHandle;
}

#endif // NANAKA_GRAPHICS_TEXTURERESOURCE_H
