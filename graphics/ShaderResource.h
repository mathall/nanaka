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

#ifndef NANAKA_GRAPHICS_SHADERRESOURCE_H
#define NANAKA_GRAPHICS_SHADERRESOURCE_H

#include <functional>
#include <vector>

#include "renderer/GLResource.h"
#include "renderer/Renderer.h"
#include "resource/Resource.h"
#include "resource/ResourceParameters.h"

class ShaderResourceParameters final : public ResourceParameters
{
public:

	explicit ShaderResourceParameters(GLuint type);

	/**
	 * ResourceParameters implementation.
	 */
	std::size_t Hash() const override;
	bool Equals(const ResourceParameters& rhs) const override;

	GLuint m_type;
};

inline ShaderResourceParameters::ShaderResourceParameters(GLuint type)
	: m_type(type)
{
}

class ShaderGLResource final : public GLResource
{
public:

	ShaderGLResource(GLuint type, std::string source);

	/**
	 * GLResource implementation.
	 */
	void Build() override;
	void Destroy() override;

	std::string m_source;

	GLuint m_type;
	GLuint m_shaderHandle;
};

inline ShaderGLResource::ShaderGLResource(
	GLuint type,
	std::string source)
	: m_source(source)
	, m_type(type)
{
}

inline void ShaderGLResource::Destroy()
{
	glDeleteShader(m_shaderHandle);
}

class ShaderResource final : public Resource
{
public:

	ShaderResource(
		std::shared_ptr<ShaderGLResource> GLResource,
		std::vector<std::string> uniforms,
		std::vector<std::string> attributes);
	~ShaderResource();

	ShaderResource(ShaderResource&) = delete;
	ShaderResource& operator=(const ShaderResource&) = delete;

	GLuint GetShaderHandle() const;
	const std::vector<std::string> GetUniforms() const;
	const std::vector<std::string> GetAttributes() const;

private:

	// shared_ptr because GLResources may be queued up on the GLResourceManager.
	std::shared_ptr<ShaderGLResource> m_GLResource;

	std::vector<std::string> m_uniforms;
	std::vector<std::string> m_attributes;
};

inline ShaderResource::ShaderResource(
	std::shared_ptr<ShaderGLResource> GLResource,
	std::vector<std::string> uniforms,
	std::vector<std::string> attributes)
	: m_GLResource(GLResource)
	, m_uniforms(uniforms)
	, m_attributes(attributes)
{
	g_renderer->QueueGLResourceForBuild(m_GLResource);
}

inline ShaderResource::~ShaderResource()
{
	g_renderer->QueueGLResourceForDestruction(m_GLResource);
}

inline GLuint ShaderResource::GetShaderHandle() const
{
	return m_GLResource->m_shaderHandle;
}

inline const std::vector<std::string> ShaderResource::GetUniforms() const
{
	return m_uniforms;
}

inline const std::vector<std::string> ShaderResource::GetAttributes() const
{
	return m_attributes;
}

#endif // NANAKA_GRAPHICS_SHADERRESOURCE_H
