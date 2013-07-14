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

#include "renderer/RenderElement.h"
#include "resource/ResourceManager.h"

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

	RE.m_shaderProgram = m_shaderProgram.GetShaderProgramHandle();

	for (const auto& optUniform : s_optionalUniforms)
	{
		if (m_shaderProgram.HasUniform(optUniform.m_name))
		{
			Uniform uniform;
			uniform.m_location =
				m_shaderProgram.GetUniformLocation(optUniform.m_name);
			uniform.m_type = optUniform.m_type;
			uniform.m_value = GetUniformValue(optUniform.m_name);
			RE.m_uniforms.push_back(uniform);
		}
	}
}

UniformValue Material::GetUniformValue(const std::string& name) const
{
	UniformValue ret;

	if (name.compare("color") == 0)
	{
		ret.m_vec4 = m_color;
	}
	else if (name.compare("texcoordsScale") == 0)
	{
		ret.m_vec2 = m_texcoordsScale;
	}
	else if (name.compare("texcoordsOffset") == 0)
	{
		ret.m_vec2 = m_texcoordsOffset;
	}
	else if (name.compare("tex") == 0)
	{
		ret.m_tex = {m_texture.GetTextureHandle(), 0};
	}

	return ret;
}
