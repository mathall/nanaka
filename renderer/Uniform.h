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

#ifndef NANAKA_RENDERER_UNIFORM_H
#define NANAKA_RENDERER_UNIFORM_H

#include <functional>
#include <string>
#include <unordered_map>

#include "math/Vec2f.h"
#include "math/Vec4f.h"
#include "renderer/GL.h"
#include "renderer/RenderResourceHandle.h"

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

enum UniformType
{
	ModelUniformType,
	ViewUniformType,
	ProjUniformType,
	TexUniformType,
	Vec2UniformType,
	Vec4UniformType,
	NumUniformTypes,
};

struct UniformDescription
{
	UniformIdentifier m_identifier;
	UniformType m_type;
};

struct TexUnit
{
	RenderResourceHandle m_handle;
	GLuint m_unit;
};

union UniformValue
{
	TexUnit m_tex;
	Vec2f m_vec2;
	Vec4f m_vec4;
};

struct Uniform
{
	UniformDescription m_desc;
	UniformValue m_value;
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
} // namespace std

#if defined(__clang__)
#pragma clang diagnostic pop
#endif // defined(__clang__)

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

#endif // NANAKA_RENDERER_UNIFORM_H
