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

#ifndef NANAKA_RENDERER_SHADERPROGRAMRERENDERRESOURCE_H
#define NANAKA_RENDERER_SHADERPROGRAMRERENDERRESOURCE_H

#include <unordered_map>

#include "renderer/Attribute.h"
#include "renderer/RenderResource.h"
#include "renderer/Uniform.h"

class ShaderProgramRenderResource final : public RenderResource
{
public:

	static const RenderResourceType s_type = ShaderProgramRenderResourceType;

	ShaderProgramRenderResource(
		RenderResourceHandle vertexShaderHandle,
		RenderResourceHandle fragmentShaderHandle);

	/**
	 * GLResource implementation.
	 */
	void Build(const RenderResourceManager& renderResourceManager) override;
	void Destroy() override;

	GLint GetAttributeLocation(AttributeIdentifier identifier) const;
	GLint GetUniformLocation(UniformIdentifier identifier) const;

	RenderResourceHandle m_vertexShaderHandle;
	RenderResourceHandle m_fragmentShaderHandle;
	GLuint m_program;
	std::unordered_map<AttributeIdentifier, GLint> m_attributes;
	std::unordered_map<UniformIdentifier, GLint> m_vUniforms;
	std::unordered_map<UniformIdentifier, GLint> m_fUniforms;
};

inline ShaderProgramRenderResource::ShaderProgramRenderResource(
	RenderResourceHandle vertexShaderHandle,
	RenderResourceHandle fragmentShaderHandle)
	: RenderResource(s_type)
	, m_vertexShaderHandle(vertexShaderHandle)
	, m_fragmentShaderHandle(fragmentShaderHandle)
{
}

inline void ShaderProgramRenderResource::Destroy()
{
	glDeleteProgram(m_program);
}

#endif // NANAKA_RENDERER_SHADERPROGRAMRERENDERRESOURCE_H
