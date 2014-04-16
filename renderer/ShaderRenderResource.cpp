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

#include "renderer/ShaderRenderResource.h"

ShaderRenderResource::ShaderRenderResource(
	GLuint type,
	std::string source,
	std::vector<std::string> uniformNames,
	std::vector<std::string> attributeNames)
	: RenderResource(s_type)
	, m_source(source)
	, m_type(type)
	, m_uniformNames(uniformNames)
	, m_attributeNames(attributeNames)
{
	if (USE_GLES)
	{
		m_source.insert(0, "#version 100\n");
	}
	else
	{
		m_source.insert(0, "#version 120\n");
	}
}

bool ShaderRenderResource::Build(
	const RenderResourceManager& renderResourceManager)
{
	m_shaderHandle = glCreateShader(m_type);
	const char* shource_cstr = m_source.c_str();
	glShaderSource(m_shaderHandle, 1, &shource_cstr, NULL);
	glCompileShader(m_shaderHandle);

	return true;
}
