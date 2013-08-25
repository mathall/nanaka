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

#ifndef NANAKA_GRAPHICS_SHADERPROGRAM_H
#define NANAKA_GRAPHICS_SHADERPROGRAM_H

#include "graphics/ShaderProgramResource.h"
#include "resource/Asset.h"
#include "resource/ResourceManager.h"

class ShaderProgram final
{
public:

	ShaderProgram() = default;
	explicit ShaderProgram(std::string filePath);
	explicit ShaderProgram(Asset<ShaderProgramResource> shaderProgram);

	GLuint GetShaderProgramHandle() const;
	bool HasAttribute(AttributeIdentifier identifier) const;
	GLint GetAttributeLocation(AttributeIdentifier identifier) const;
	bool HasUniform(UniformIdentifier identifier) const;
	GLint GetUniformLocation(UniformIdentifier identifier) const;

private:

	Asset<ShaderProgramResource> m_shaderProgramAsset;
};

inline ShaderProgram::ShaderProgram(std::string filePath)
	: m_shaderProgramAsset(
		g_resourceManager->Acquire<ShaderProgramResource>(filePath))
{
}

inline ShaderProgram::ShaderProgram(Asset<ShaderProgramResource> shaderProgram)
	: m_shaderProgramAsset(shaderProgram)
{
}

inline GLuint ShaderProgram::GetShaderProgramHandle() const
{
	return m_shaderProgramAsset.IsValid() ?
		m_shaderProgramAsset.GetResource()->GetShaderProgramHandle() : 0;
}

inline bool ShaderProgram::HasAttribute(AttributeIdentifier identifier) const
{
	return m_shaderProgramAsset.IsValid() ?
		m_shaderProgramAsset.GetResource()->HasAttribute(identifier) : false;
}

inline GLint ShaderProgram::GetAttributeLocation(
	AttributeIdentifier identifier) const
{
	return m_shaderProgramAsset.IsValid() ?
		m_shaderProgramAsset.GetResource()->GetAttributeLocation(identifier) :
		-1;
}

inline bool ShaderProgram::HasUniform(UniformIdentifier identifier) const
{
	return m_shaderProgramAsset.IsValid() ?
		m_shaderProgramAsset.GetResource()->HasUniform(identifier) : false;
}

inline GLint ShaderProgram::GetUniformLocation(
	UniformIdentifier identifier) const
{
	return m_shaderProgramAsset.IsValid() ?
		m_shaderProgramAsset.GetResource()->GetUniformLocation(identifier) : -1;
}

#endif // NANAKA_GRAPHICS_SHADERPROGRAM_H
