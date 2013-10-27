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

#include "graphics/Material.h"

#include <cassert>

#include "renderer/RenderElement.h"
#include "resource/ResourceManager.h"

static const std::vector<UniformDescription> s_uniformDescriptions =
	{
		{ModelUniformIdentifier, ModelUniformType},
		{ViewUniformIdentifier, ViewUniformType},
		{ProjUniformIdentifier, ProjUniformType},
		{TexUniformIdentifier, TexUniformType},
		{ColorUniformIdentifier, Vec4UniformType},
		{TexcoordsScaleUniformIdentifier, Vec2UniformType},
		{TexcoordsOffsetUniformIdentifier, Vec2UniformType},
	};

Material::Material(std::string filePath)
	: m_materialAsset(g_resourceManager->Acquire<MaterialResource>(filePath))
	, m_color(1.0f)
	, m_transparent(false)
{
	Constructor();
}

Material::Material(Asset<MaterialResource> materialAsset)
	: m_materialAsset(materialAsset)
	, m_color(1.0f)
	, m_transparent(false)
{
	Constructor();
}

void Material::Constructor()
{
	if (!m_materialAsset.IsValid())
	{
		return;
	}

	m_shaderProgram = ShaderProgram(
		m_materialAsset.GetResource()->GetShaderProgramAsset());
	m_texture = Texture(m_materialAsset.GetResource()->GetTextureAsset());
	m_transparent = m_materialAsset.GetResource()->IsTransparent();
}

void Material::PrepRender(RenderElement& RE) const
{
	RE.m_renderList = m_transparent ? RenderListTransparent : RenderListGeneral;

	RE.m_shaderProgramHandle = m_shaderProgram.GetShaderProgramHandle();

	for (const auto& uniformDsc : s_uniformDescriptions)
	{
		if (m_shaderProgram.HasUniform(uniformDsc.m_identifier))
		{
			Uniform uniform;
			uniform.m_desc = uniformDsc;
			uniform.m_value = GetUniformValue(uniformDsc.m_identifier);
			RE.m_uniforms.push_back(uniform);
		}
	}
}

UniformValue Material::GetUniformValue(UniformIdentifier identifier) const
{
	UniformValue ret;

	switch (identifier)
	{
	case ModelUniformIdentifier:
	case ViewUniformIdentifier:
	case ProjUniformIdentifier:
		assert("Unpermitted uniforms in material description.");
		break;

	case TexUniformIdentifier:
		ret.m_tex = {m_texture.GetTextureHandle(), 0};
		break;

	case ColorUniformIdentifier:
		ret.m_vec4 = m_color;
		break;

	case TexcoordsScaleUniformIdentifier:
		ret.m_vec2 = m_texcoordsScale;
		break;

	case TexcoordsOffsetUniformIdentifier:
		ret.m_vec2 = m_texcoordsOffset;
		break;
	}

	return ret;
}
