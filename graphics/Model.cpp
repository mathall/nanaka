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

#include "graphics/Model.h"

#include "resource/ResourceManager.h"

Model::Model(std::string modelFilePath)
	: m_modelAsset(g_resourceManager->Acquire<ModelResource>(modelFilePath))
	, m_material()
	, m_position(0.0f)
	, m_rotation(Quat::NoRot())
	, m_scale(1.0f)
{
	if (m_modelAsset.IsValid())
	{
		m_material = Material(m_modelAsset.GetResource()->GetMaterialAsset());
	}
}

void Model::PrepRender(RenderElement& RE) const
{
	if (!m_modelAsset.IsValid())
	{
		return;
	}

	RE.m_modelTransform = Mat44::MakeTransform(m_position, m_rotation, m_scale);

	for (const auto& optAttribute : s_optionalAttributes)
	{
		if (m_material.GetShaderProgram().HasAttribute(optAttribute.m_name))
		{
			Attribute attribute;
			attribute.m_location =
				m_material.GetShaderProgram().GetAttributeLocation(
					optAttribute.m_name);
			attribute.m_buffer = GetAttributeBuffer(optAttribute.m_name);
			attribute.m_num = optAttribute.m_num;
			RE.m_attributes.push_back(attribute);
		}
	}

	RE.m_EBO = m_modelAsset.GetResource()->GetEBO();
	RE.m_numIndices = m_modelAsset.GetResource()->GetIndexBufferSize();

	m_material.PrepRender(RE);
}

GLuint Model::GetAttributeBuffer(const std::string& name) const
{
	GLuint ret = 0;

	if (name.compare("position") == 0)
	{
		ret = m_modelAsset.GetResource()->GetPosVBO();
	}
	else if (name.compare("texcoord") == 0)
	{
		ret = m_modelAsset.GetResource()->GetTexVBO();
	}

	return ret;
}
