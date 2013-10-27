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
#include <unordered_set>
#include <vector>

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

class ShaderResource final : public Resource
{
public:

	ShaderResource(
		RenderResourceHandle renderResourceHandle,
		const std::vector<std::string>& uniformNames,
		const std::vector<std::string>& attributeNames);
	~ShaderResource();

	ShaderResource(ShaderResource&) = delete;
	ShaderResource& operator=(const ShaderResource&) = delete;

	RenderResourceHandle GetShaderHandle() const;
	bool HasAttribute(AttributeIdentifier identifier) const;
	bool HasUniform(UniformIdentifier identifier) const;

private:

	RenderResourceHandle m_renderResourceHandle;
	std::unordered_set<UniformIdentifier> m_uniformIdentifiers;
	std::unordered_set<AttributeIdentifier> m_attributeIdentifiers;
};

inline ShaderResource::ShaderResource(
	RenderResourceHandle renderResourceHandle,
	const std::vector<std::string>& uniformNames,
	const std::vector<std::string>& attributeNames)
	: m_renderResourceHandle(renderResourceHandle)
{
	for (auto& uniformName : uniformNames)
	{
		m_uniformIdentifiers.insert(
			s_uniformNameIdentifierLookup.find(uniformName)->second);
	}

	for (auto& attributeName : attributeNames)
	{
		m_attributeIdentifiers.insert(
			s_attributeNameIdentifierLookup.find(attributeName)->second);
	}
}

inline ShaderResource::~ShaderResource()
{
	g_renderer->DestroyRenderResource(m_renderResourceHandle);
}

inline RenderResourceHandle ShaderResource::GetShaderHandle() const
{
	return m_renderResourceHandle;
}

inline bool ShaderResource::HasAttribute(AttributeIdentifier identifier) const
{
	return m_attributeIdentifiers.find(identifier)
		!= m_attributeIdentifiers.end();
}

inline bool ShaderResource::HasUniform(UniformIdentifier identifier) const
{
	return m_uniformIdentifiers.find(identifier)
		!= m_uniformIdentifiers.end();
}

#endif // NANAKA_GRAPHICS_SHADERRESOURCE_H
