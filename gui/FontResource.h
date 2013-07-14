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

#ifndef NANAKA_GUI_FONTRESOURCE_H
#define NANAKA_GUI_FONTRESOURCE_H

#include <unordered_map>
#include <vector>

#include "graphics/TextureResource.h"
#include "math/Vec2f.h"
#include "resource/Asset.h"
#include "resource/Resource.h"

class FontResource final : public Resource
{
public:

	std::vector<Vec2f> m_texcoordsScales;
	std::vector<Vec2f> m_texcoordsOffsets;
	std::unordered_map<char, size_t> m_charToIdxLookup;
	std::vector<float> m_aspectRatios;
	Asset<TextureResource> m_textureAsset;

	FontResource(
		std::vector<Vec2f> texcoordsScales,
		std::vector<Vec2f> texcoordsOffsets,
		std::vector<float> aspectRatios,
		std::unordered_map<char, size_t> charToIdxLookup,
		Asset<TextureResource> textureAsset)
		: m_texcoordsScales(texcoordsScales)
		, m_texcoordsOffsets(texcoordsOffsets)
		, m_charToIdxLookup(charToIdxLookup)
		, m_aspectRatios(aspectRatios)
		, m_textureAsset(textureAsset)
	{
	}
};

#endif // NANAKA_GUI_FONTRESOURCE_H
