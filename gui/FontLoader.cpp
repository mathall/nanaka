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

#include "gui/FontLoader.h"

#include <cassert>

#include <json.h>

#include "gui/FontResource.h"
#include "resource/ResourceManager.h"

std::shared_ptr<Resource> FontLoader::Load(
	const ResourceKey& key,
	const FileManager& fileManager) const
{
	File file;
	fileManager.Open(key.GetFilePath(), file);
	auto fontJson = file.ReadAll();

	auto deleter = [](json_value* p){ json_value_free(p); };
	auto fontVal = std::unique_ptr<json_value, decltype(deleter)>(
		json_parse(fontJson.c_str(),
			fontJson.length() * sizeof(std::string::value_type)),
		deleter);

	assert(fontVal->type == json_object);
	auto textureVal = (*fontVal)["texture"];
	auto dimXVal = (*fontVal)["dimX"];
	auto dimYVal = (*fontVal)["dimY"];
	auto charHVal = (*fontVal)["charH"];
	auto charactersVal = (*fontVal)["characters"];

	assert(textureVal.type == json_string);
	std::string textureFilePath = MakeAbsolutePath(
		key.GetFilePath(), std::string(textureVal));

	auto texture = g_resourceManager->Acquire<TextureResource>(
		ResourceKey(textureFilePath));

	assert(dimXVal.type == json_integer);
	assert(dimYVal.type == json_integer);
	assert(charHVal.type == json_integer);
	int dimX = json_int_t(dimXVal);
	int dimY = json_int_t(dimYVal);
	int charH = json_int_t(charHVal);

	float charTexW = 1.0f / dimX;
	float charTexH = 1.0f / dimY;
	float charScaleAspectRatio = 1.0f / (dimX * charH);
	float charScaleY = 1.0f / dimY;

	std::vector<Vec2f> texcoordsScales;
	std::vector<Vec2f> texcoordsOffsets;
	std::vector<float> aspectRatios;
	std::unordered_map<char, size_t> charToIdxLookup;

	assert(charactersVal.type == json_array);
	const unsigned int numCharacters = charactersVal.u.array.length;
	for (size_t i = 0; i < numCharacters; ++i)
	{
		assert(charactersVal[i].type == json_object);
		auto charVal = charactersVal[i]["c"];
		auto xVal = charactersVal[i]["x"];
		auto yVal = charactersVal[i]["y"];
		auto charWVal = charactersVal[i]["w"];

		assert(charVal.type == json_string);
		assert(xVal.type == json_integer);
		assert(yVal.type == json_integer);
		assert(charWVal.type == json_integer);
		char character = std::string(charVal).at(0);
		int charTexX = json_int_t(xVal);
		int charTexY = json_int_t(yVal);
		int charW = json_int_t(charWVal);

		Vec2f texcoordsScale(charW * charScaleAspectRatio, charScaleY);
		Vec2f texcoordsOffset(
			charTexX * charTexW, 1 - charTexY * charTexH - charTexH);

		texcoordsScales.push_back(texcoordsScale);
		texcoordsOffsets.push_back(texcoordsOffset);
		aspectRatios.push_back(charW / static_cast<float>(charH));
		charToIdxLookup.insert(std::make_pair(character, i));
	}

	return std::make_shared<FontResource>(
		texcoordsScales, texcoordsOffsets, aspectRatios, charToIdxLookup,
		texture);
}
