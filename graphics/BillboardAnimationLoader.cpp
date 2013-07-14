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

#include "graphics/BillboardAnimationLoader.h"

#include <cassert>

#include <json.h>

#include "graphics/BillboardAnimationResource.h"
#include "resource/ResourceManager.h"

std::shared_ptr<Resource> BillboardAnimationLoader::Load(
	const ResourceKey& key,
	const FileManager& fileManager) const
{
	File file;
	fileManager.Open(key.GetFilePath(), file);
	auto animationJson = file.ReadAll();

	auto deleter = [](json_value* p){ json_value_free(p); };
	auto animationVal = std::unique_ptr<json_value, decltype(deleter)>(
		json_parse(animationJson.c_str(),
			animationJson.length() * sizeof(std::string::value_type)),
		deleter);

	assert(animationVal->type == json_object);
	auto textureVal = (*animationVal)["texture"];
	auto framesXVal = (*animationVal)["framesX"];
	auto framesYVal = (*animationVal)["framesY"];
	auto framesVal = (*animationVal)["frames"];

	assert(textureVal.type == json_string);
	std::string textureFilePath = MakeAbsolutePath(
		key.GetFilePath(), std::string(textureVal));

	auto texture = g_resourceManager->Acquire<TextureResource>(
		ResourceKey(textureFilePath));

	assert(framesXVal.type == json_integer);
	assert(framesYVal.type == json_integer);
	int numFramesX = json_int_t(framesXVal);
	int numFramesY = json_int_t(framesYVal);

	std::vector<Vec2f> texcoordsScales;
	std::vector<Vec2f> texcoordsOffsets;
	std::vector<int> times;

	assert(framesVal.type == json_array);
	const unsigned int numFrames = framesVal.u.array.length;
	for (size_t i = 0; i < numFrames; ++i)
	{
		assert(framesVal[i].type == json_object);
		auto frameXVal = framesVal[i]["x"];
		auto frameYVal = framesVal[i]["y"];
		auto timeVal = framesVal[i]["ms"];

		assert(frameXVal.type == json_integer);
		assert(frameYVal.type == json_integer);
		assert(timeVal.type == json_integer);
		int frameX = json_int_t(frameXVal);
		int frameY = json_int_t(frameYVal);
		int time = json_int_t(timeVal);

		float frameWidth = 1.0f / numFramesX;
		float frameHeight = 1.0f / numFramesY;

		Vec2f texcoordsScale(frameWidth, frameHeight);
		Vec2f texcoordsOffset(frameX * frameWidth, (1 - frameY) * frameHeight);

		texcoordsScales.push_back(texcoordsScale);
		texcoordsOffsets.push_back(texcoordsOffset);
		times.push_back(time);
	}

	return std::make_shared<BillboardAnimationResource>(
		texcoordsScales, texcoordsOffsets, times, texture);
}
