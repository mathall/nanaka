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

#ifndef NANAKA_RENDERER_INDEXBUFFERRENDERRESOURCE_H
#define NANAKA_RENDERER_INDEXBUFFERRENDERRESOURCE_H

#include <memory>

#include "renderer/GL.h"
#include "renderer/RenderResource.h"

class IndexBufferRenderResource final : public RenderResource
{
public:

	static const RenderResourceType s_type = IndexBufferRenderResourceType;

	IndexBufferRenderResource(
		std::unique_ptr<GLushort[]> bufferData,
		int indexBufferSize);

	/**
	 * RenderResource implementation.
	 */
	bool Build(const RenderResourceManager& renderResourceManager) override;
	void Destroy() override;

	std::unique_ptr<GLushort[]> m_bufferData;
	int m_bufferDataSize;

	GLuint m_EBO;
};

#endif // NANAKA_RENDERER_INDEXBUFFERRENDERRESOURCE_H
