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

#ifndef NANAKA_RENDERER_MESHRENDERRESOURCE_H
#define NANAKA_RENDERER_MESHRENDERRESOURCE_H

#include <memory>

#include "renderer/RenderResource.h"

class MeshRenderResource final : public RenderResource
{
public:

	static const RenderResourceType s_type = MeshRenderResourceType;

	MeshRenderResource(
		std::unique_ptr<GLfloat[]> vertexBuffer,
		int vertexBufferSize,
		std::unique_ptr<GLfloat[]> texcoordBuffer,
		int texcoordBufferSize,
		std::unique_ptr<GLushort[]> indexBuffer,
		int indexBufferSize);

	/**
	 * GLResource implementation.
	 */
	void Build() override;
	void Destroy() override;

	std::unique_ptr<GLfloat[]> m_vertexBuffer;
	int m_vertexBufferSize;
	std::unique_ptr<GLfloat[]> m_texcoordBuffer;
	int m_texcoordBufferSize;
	std::unique_ptr<GLushort[]> m_indexBuffer;
	int m_indexBufferSize;

	GLuint m_posVBO;
	GLuint m_texVBO;
	GLuint m_EBO;
};

#endif // NANAKA_RENDERER_MESHRENDERRESOURCE_H
