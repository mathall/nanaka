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

#include "gui/Font.h"

#include "resource/ResourceManager.h"

Font::Font(std::string fontFilePath)
	: m_Id(UUID::New())
	, m_asset(g_resourceManager->Acquire<FontResource>(fontFilePath))
	, m_shaderProgram("nanaka/shaders/font.nsp")
{
	if (!m_asset.IsValid())
	{
		return;
	}

	m_texture = Texture(m_asset.GetResource()->m_textureAsset);
}

Vec2f Font::GetTexcoordScale(char character) const
{
	auto charIdx = m_asset.GetResource()->m_charToIdxLookup.find(character);
	if (charIdx != m_asset.GetResource()->m_charToIdxLookup.end())
	{
		return m_asset.GetResource()->m_texcoordsScales.at(charIdx->second);
	}
	return Vec2f::Zero();
}

Vec2f Font::GetTexcoordOffset(char character) const
{
	auto charIdx = m_asset.GetResource()->m_charToIdxLookup.find(character);
	if (charIdx != m_asset.GetResource()->m_charToIdxLookup.end())
	{
		return m_asset.GetResource()->m_texcoordsOffsets.at(charIdx->second);
	}
	return Vec2f::Zero();
}

float Font::GetAspectRatio(char character) const
{
	float ret = 0.0f;

	auto charIdx = m_asset.GetResource()->m_charToIdxLookup.find(character);

	if (charIdx != m_asset.GetResource()->m_charToIdxLookup.end())
	{
		ret = m_asset.GetResource()->m_aspectRatios.at(charIdx->second);
	}

	return ret;
}
