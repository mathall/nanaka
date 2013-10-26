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

#include "renderer/Renderer.h"
#include "resource/Resource.h"
#include "resource/ResourceParameters.h"

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

class ShaderResource final : public Resource
{
public:

	explicit ShaderResource(RenderResourceHandle renderResourceHandle);
	~ShaderResource();

	ShaderResource(ShaderResource&) = delete;
	ShaderResource& operator=(const ShaderResource&) = delete;

	RenderResourceHandle GetShaderHandle() const;

private:

	RenderResourceHandle m_renderResourceHandle;
};

inline ShaderResource::ShaderResource(RenderResourceHandle renderResourceHandle)
	: m_renderResourceHandle(renderResourceHandle)
{
}

inline ShaderResource::~ShaderResource()
{
	g_renderer->DestroyRenderResource(m_renderResourceHandle);
}

inline RenderResourceHandle ShaderResource::GetShaderHandle() const
{
	return m_renderResourceHandle;
}

#endif // NANAKA_GRAPHICS_SHADERRESOURCE_H
