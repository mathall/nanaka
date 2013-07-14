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

#include "graphics/MaterialLoader.h"

#include <cassert>

#include <json.h>

#include "graphics/MaterialResource.h"
#include "resource/ResourceManager.h"

std::shared_ptr<Resource> MaterialLoader::Load(
	const ResourceKey& key,
	const FileManager& fileManager) const
{
	File file;
	fileManager.Open(key.GetFilePath(), file);
	auto materialJson = file.ReadAll();

	auto deleter = [](json_value* p){ json_value_free(p); };
	auto materialVal = std::unique_ptr<json_value, decltype(deleter)>(
		json_parse(materialJson.c_str(),
			materialJson.length() * sizeof(std::string::value_type)),
		deleter);

	assert(materialVal->type == json_object);
	auto programVal = (*materialVal)["Program"];
	auto textureVal = (*materialVal)["Texture"];
	auto transparentVal = (*materialVal)["transparent"];

	assert(programVal.type == json_string);
	auto shaderProgram = g_resourceManager->Acquire<ShaderProgramResource>(
		ResourceKey(std::string(programVal)));

	Asset<TextureResource> texture;
	if (textureVal.type != json_value_none.type)
	{
		assert(textureVal.type == json_string);
		texture = g_resourceManager->Acquire<TextureResource>(
			ResourceKey(std::string(textureVal)));
	}

	bool transparent = false;
	if (transparentVal.type != json_value_none.type)
	{
		assert(transparentVal.type == json_boolean);
		transparent = bool(transparentVal);
	}

	return std::make_shared<MaterialResource>(shaderProgram, texture, transparent);
}
