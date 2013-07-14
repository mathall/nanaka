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

#ifndef NANAKA_GUI_FONT_H
#define NANAKA_GUI_FONT_H

#include <string>

#include "gui/FontResource.h"
#include "graphics/ShaderProgram.h"
#include "graphics/Texture.h"
#include "resource/Asset.h"
#include "utils/UUID.h"

#include <memory>

class TextureResource;

class Font final
{
public:

	explicit Font(std::string fontFilePath);

	UUID GetId() const;

	bool HasGlyph(char character) const;
	Vec2f GetTexcoordScale(char character) const;
	Vec2f GetTexcoordOffset(char character) const;
	float GetAspectRatio(char character) const;

	const Texture& GetTexture() const;
	const ShaderProgram& GetShaderProgram() const;

private:

	UUID m_Id;
	Asset<FontResource> m_asset;
	Texture m_texture;
	ShaderProgram m_shaderProgram;
};

inline UUID Font::GetId() const
{
	return m_Id;
}

inline bool Font::HasGlyph(char character) const
{
	return m_asset.GetResource()->m_charToIdxLookup.find(character)
		!= m_asset.GetResource()->m_charToIdxLookup.end();
}

inline const Texture& Font::GetTexture() const
{
	return m_texture;
}

inline const ShaderProgram& Font::GetShaderProgram() const
{
	return m_shaderProgram;
}

#endif // NANAKA_GUI_FONT_H
