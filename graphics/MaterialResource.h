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

#ifndef NANAKA_GRAPHICS_MATERIALRESOURCE_H
#define NANAKA_GRAPHICS_MATERIALRESOURCE_H

#include "graphics/ShaderProgramResource.h"
#include "graphics/TextureResource.h"
#include "resource/Asset.h"
#include "resource/Resource.h"

class MaterialResource final : public Resource
{
public:

	MaterialResource(
		Asset<ShaderProgramResource> shaderProgramAsset,
		Asset<TextureResource> textureAsset,
		bool transparent);

	Asset<ShaderProgramResource> GetShaderProgramAsset() const;
	Asset<TextureResource> GetTextureAsset() const;
	bool IsTransparent() const;

private:

	Asset<ShaderProgramResource> m_shaderProgramAsset;
	Asset<TextureResource> m_textureAsset;
	bool m_transparent;
};

inline MaterialResource::MaterialResource(
	Asset<ShaderProgramResource> shaderProgramAsset,
	Asset<TextureResource> textureAsset,
	bool transparent)
	: m_shaderProgramAsset(shaderProgramAsset)
	, m_textureAsset(textureAsset)
	, m_transparent(transparent)
{
}

inline Asset<ShaderProgramResource>
MaterialResource::GetShaderProgramAsset() const
{
	return m_shaderProgramAsset;
}

inline Asset<TextureResource> MaterialResource::GetTextureAsset() const
{
	return m_textureAsset;
}

inline bool MaterialResource::IsTransparent() const
{
	return m_transparent;
}

#endif // NANAKA_GRAPHICS_MATERIALRESOURCE_H
