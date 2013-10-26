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

#include "graphics/ShaderProgramResource.h"

#include "renderer/RenderResourceManager.h"
#include "renderer/ShaderRenderResource.h"

void ShaderProgramGLResource::Build(
	const RenderResourceManager& renderResourceManager)
{
	auto vertexShader =
		renderResourceManager.Get<ShaderRenderResource>(m_vertexShaderHandle);
	auto fragmentShader =
		renderResourceManager.Get<ShaderRenderResource>(m_fragmentShaderHandle);

	if (!vertexShader || !fragmentShader)
	{
		return;
	}

	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShader->m_shaderHandle);
	glAttachShader(m_program, fragmentShader->m_shaderHandle);
	glLinkProgram(m_program);

	for (auto attributeName : vertexShader->m_attributeNames)
	{
		GLint attribLoc = glGetAttribLocation(m_program, attributeName.c_str());
		m_attributes.insert(std::make_pair(
			s_attributeNameIdentifierLookup.find(attributeName)->second,
			attribLoc));
	}

	for (auto uniformName : vertexShader->m_uniformNames)
	{
		GLint uniLoc = glGetUniformLocation(m_program, uniformName.c_str());
		m_vUniforms.insert(std::make_pair(
			s_uniformNameIdentifierLookup.find(uniformName)->second, uniLoc));
	}

	for (auto uniformName : fragmentShader->m_uniformNames)
	{
		GLint uniLoc = glGetUniformLocation(m_program, uniformName.c_str());
		m_fUniforms.insert(std::make_pair(
			s_uniformNameIdentifierLookup.find(uniformName)->second, uniLoc));
	}
}

GLint ShaderProgramResource::GetAttributeLocation(
	AttributeIdentifier identifier) const
{
	auto attributeIt = m_GLResource->m_attributes.find(identifier);
	if (attributeIt != m_GLResource->m_attributes.end())
	{
		return attributeIt->second;
	}
	else
	{
		return -1;
	}
}

GLint ShaderProgramResource::GetUniformLocation(
	UniformIdentifier identifier) const
{
	auto uniformIt = m_GLResource->m_fUniforms.find(identifier);
	if (uniformIt != m_GLResource->m_fUniforms.end())
	{
		return uniformIt->second;
	}
	else
	{
		uniformIt = m_GLResource->m_vUniforms.find(identifier);
		if (uniformIt != m_GLResource->m_vUniforms.end())
		{
			return uniformIt->second;
		}
		else
		{
			return -1;
		}
	}
}
