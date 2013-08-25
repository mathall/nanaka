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
#include <unordered_map>
#include <vector>

#include "renderer/GLResource.h"
#include "renderer/Renderer.h"
#include "resource/Resource.h"
#include "resource/ResourceParameters.h"

enum AttributeIdentifier
{
	PositionAttributeIdentifier,
	TexcoordAttributeIdentifier,
};

static const std::unordered_map<std::string, AttributeIdentifier>
s_attributeNameIdentifierLookup =
	{
		{"position", PositionAttributeIdentifier},
		{"texcoord", TexcoordAttributeIdentifier},
	};

enum UniformIdentifier
{
	ModelUniformIdentifier,
	ViewUniformIdentifier,
	ProjUniformIdentifier,
	TexUniformIdentifier,
	ColorUniformIdentifier,
	TexcoordsScaleUniformIdentifier,
	TexcoordsOffsetUniformIdentifier,
};

static const std::unordered_map<std::string, UniformIdentifier>
s_uniformNameIdentifierLookup =
	{
		{"model", ModelUniformIdentifier},
		{"view", ViewUniformIdentifier},
		{"proj", ProjUniformIdentifier},
		{"tex", TexUniformIdentifier},
		{"color", ColorUniformIdentifier},
		{"texcoordsScale", TexcoordsScaleUniformIdentifier},
		{"texcoordsOffset", TexcoordsOffsetUniformIdentifier},
	};

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmismatched-tags"
#endif // defined(__clang__)

namespace std {
template<>
struct hash<UniformIdentifier> {
public:
	size_t operator()(const UniformIdentifier& identifier) const
	{
		return std::hash<int>()(identifier);
	}
};

template<>
struct hash<AttributeIdentifier> {
public:
	size_t operator()(const AttributeIdentifier& identifier) const
	{
		return std::hash<int>()(identifier);
	}
};
} // namespace std

#if defined(__clang__)
#pragma clang diagnostic pop
#endif // defined(__clang__)

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
	const std::vector<std::string>& GetUniformNames() const;
	const std::vector<std::string>& GetAttributeNames() const;

private:

	// shared_ptr because GLResources may be queued up on the GLResourceManager.
	std::shared_ptr<ShaderGLResource> m_GLResource;

	std::vector<std::string> m_uniformNames;
	std::vector<UniformIdentifier> m_uniformIdentifiers;
	std::vector<std::string> m_attributeNames;
	std::vector<AttributeIdentifier> m_attributeIdentifiers;
};

inline ShaderResource::ShaderResource(
	std::shared_ptr<ShaderGLResource> GLResource,
	std::vector<std::string> uniforms,
	std::vector<std::string> attributes)
	: m_GLResource(GLResource)
	, m_uniformNames(uniforms)
	, m_attributeNames(attributes)
{
	for (auto& uniformName : m_uniformNames)
	{
		m_uniformIdentifiers.push_back(
			s_uniformNameIdentifierLookup.find(uniformName)->second);
	}

	for (auto& attributeName : m_attributeNames)
	{
		m_attributeIdentifiers.push_back(
			s_attributeNameIdentifierLookup.find(attributeName)->second);
	}

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

inline const std::vector<std::string>& ShaderResource::GetUniformNames() const
{
	return m_uniformNames;
}

inline const std::vector<std::string>& ShaderResource::GetAttributeNames() const
{
	return m_attributeNames;
}

#endif // NANAKA_GRAPHICS_SHADERRESOURCE_H
