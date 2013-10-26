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

#ifndef NANAKA_RENDERER_RENDERELEMENT_H
#define NANAKA_RENDERER_RENDERELEMENT_H

#include <vector>

#include "math/Mat44.h"
#include "math/Vec2f.h"
#include "math/Vec4f.h"
#include "renderer/GL.h"
#include "renderer/RenderList.h"
#include "renderer/RenderResourceHandle.h"

struct TexUnit
{
	RenderResourceHandle m_handle;
	GLuint m_unit;
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

union UniformValue
{
	TexUnit m_tex;
	Vec2f m_vec2;
	Vec4f m_vec4;
};

struct Uniform
{
	GLint m_location;
	UniformType m_type;
	UniformValue m_value;
};

struct Attribute
{
	GLint m_location;
	GLuint m_buffer;
	GLuint m_num;
};

class RenderElement final
{
public:

	Mat44 m_modelTransform;
	GLuint m_shaderProgram;
	std::vector<Uniform> m_uniforms;
	std::vector<Attribute> m_attributes;
	GLuint m_EBO;
	int m_numIndices;
	RenderList m_renderList;

	struct Clear
	{
		void operator()(RenderElement& renderElement);
	};
};

inline void RenderElement::Clear::operator()(RenderElement& RE)
{
	RE.m_uniforms.clear();
	RE.m_attributes.clear();
}

#endif // NANAKA_RENDERER_RENDERELEMENT_H
