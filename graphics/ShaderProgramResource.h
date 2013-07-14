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

#ifndef NANAKA_GRAPHICS_SHADERPROGRAMRESOURCE_H
#define NANAKA_GRAPHICS_SHADERPROGRAMRESOURCE_H

#include <unordered_map>

#include "renderer/GLResource.h"
#include "renderer/Renderer.h"
#include "resource/Asset.h"
#include "resource/Resource.h"
#include "graphics/ShaderResource.h"

class ShaderProgramGLResource final : public GLResource
{
public:

	ShaderProgramGLResource(
		Asset<ShaderResource> vertexShader,
		Asset<ShaderResource> fragmentShader);

	/**
	 * GLResource implementation.
	 */
	void Build() override;
	void Destroy() override;

	Asset<ShaderResource> m_vertexShader;
	Asset<ShaderResource> m_fragmentShader;
	GLuint m_program;
	std::unordered_map<std::string, GLint> m_attributes;
	std::unordered_map<std::string, GLint> m_vUniforms;
	std::unordered_map<std::string, GLint> m_fUniforms;
};

inline ShaderProgramGLResource::ShaderProgramGLResource(
	Asset<ShaderResource> vertexShader,
	Asset<ShaderResource> fragmentShader)
	: m_vertexShader(vertexShader)
	, m_fragmentShader(fragmentShader)
{
}

inline void ShaderProgramGLResource::Destroy()
{
	glDeleteProgram(m_program);
}

class ShaderProgramResource final : public Resource
{
public:

	explicit ShaderProgramResource(
		std::shared_ptr<ShaderProgramGLResource> shaderProgram);
	~ShaderProgramResource();

	ShaderProgramResource(ShaderProgramResource&) = delete;
	ShaderProgramResource& operator=(const ShaderProgramResource&) = delete;

	GLuint GetShaderProgramHandle() const;
	bool HasAttribute(const std::string& attribute) const;
	GLint GetAttributeLocation(const std::string& attribute) const;
	bool HasUniform(const std::string& uniform) const;
	GLint GetUniformLocation(const std::string& uniform) const;

private:

	std::shared_ptr<ShaderProgramGLResource> m_GLResource;
};

inline ShaderProgramResource::ShaderProgramResource(
	std::shared_ptr<ShaderProgramGLResource> GLResource)
	: m_GLResource(GLResource)
{
	g_renderer->QueueGLResourceForBuild(m_GLResource, 1);
}

inline ShaderProgramResource::~ShaderProgramResource()
{
	g_renderer->QueueGLResourceForDestruction(m_GLResource);
}

inline GLuint ShaderProgramResource::GetShaderProgramHandle() const
{
	return m_GLResource->m_program;
}

inline bool ShaderProgramResource::HasAttribute(
	const std::string& attribute) const
{
	return m_GLResource->m_attributes.find(attribute)
		!= m_GLResource->m_attributes.end();
}

inline bool ShaderProgramResource::HasUniform(const std::string& uniform) const
{
	return m_GLResource->m_fUniforms.find(uniform)
		!= m_GLResource->m_fUniforms.end()
		|| m_GLResource->m_vUniforms.find(uniform)
		!= m_GLResource->m_vUniforms.end();
}

#endif // NANAKA_GRAPHICS_SHADERPROGRAMRESOURCE_H
