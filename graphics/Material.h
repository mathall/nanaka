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

#ifndef NANAKA_GRAPHICS_MATERIAL_H
#define NANAKA_GRAPHICS_MATERIAL_H

#include "graphics/MaterialResource.h"
#include "graphics/ShaderProgram.h"
#include "graphics/Texture.h"
#include "math/Vec2f.h"
#include "math/Vec4f.h"
#include "resource/Asset.h"

class RenderElement;

class Material final
{
public:

	Material() = default;
	explicit Material(std::string filePath);
	explicit Material(Asset<MaterialResource> materialAsset);
	void Constructor();

	void PrepRender(RenderElement& RE) const;

	void SetShaderProgram(ShaderProgram shaderProgram);
	void SetTexture(Texture texture);

	const ShaderProgram& GetShaderProgram() const;
	const Texture& GetTexture() const;

	void SetColor(Vec4f color);
	void SetAlpha(float alpha);
	void SetTransparent(bool transparent);
	void SetTexcoordsScale(Vec2f scale);
	void SetTexcoordsOffset(Vec2f offset);

private:

	UniformValue GetUniformValue(UniformIdentifier identifier) const;

	Asset<MaterialResource> m_materialAsset;

	ShaderProgram m_shaderProgram;
	Texture m_texture;
	Vec4f m_color;
	bool m_transparent;
	Vec2f m_texcoordsScale;
	Vec2f m_texcoordsOffset;
};

inline void Material::SetShaderProgram(ShaderProgram shaderProgram)
{
	m_shaderProgram = shaderProgram;
}

inline void Material::SetTexture(Texture texture)
{
	m_texture = texture;
}

inline const ShaderProgram& Material::GetShaderProgram() const
{
	return m_shaderProgram;
}

inline const Texture& Material::GetTexture() const
{
	return m_texture;
}

inline void Material::SetColor(Vec4f color)
{
	m_color = color;
}

inline void Material::SetAlpha(float alpha)
{
	m_color.a = alpha;
}

inline void Material::SetTransparent(bool transparent)
{
	m_transparent = transparent;
}

inline void Material::SetTexcoordsScale(Vec2f scale)
{
	m_texcoordsScale = scale;
}

inline void Material::SetTexcoordsOffset(Vec2f offset)
{
	m_texcoordsOffset = offset;
}

#endif // NANAKA_GRAPHICS_MATERIAL_H
