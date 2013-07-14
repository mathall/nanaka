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

#include "graphics/ShaderProgramLoader.h"

#include <cassert>

#include <json.h>

#include "graphics/ShaderProgram.h"
#include "resource/ResourceManager.h"

std::shared_ptr<Resource> ShaderProgramLoader::Load(
	const ResourceKey& key,
	const FileManager& fileManager) const
{
	File file;
	fileManager.Open(key.GetFilePath(), file);
	auto programJson = file.ReadAll();

	auto deleter = [](json_value* p){ json_value_free(p); };
	auto programVal = std::unique_ptr<json_value, decltype(deleter)>(
		json_parse(programJson.c_str(),
			programJson.length() * sizeof(std::string::value_type)),
		deleter);

	assert(programVal->type == json_object);

	auto VShaderVal = (*programVal)["VShader"];
	assert(VShaderVal.type == json_string);
	std::string vShader(VShaderVal);

	auto FShaderVal = (*programVal)["FShader"];
	assert(FShaderVal.type == json_string);
	std::string fShader(FShaderVal);

	auto vertexShader = g_resourceManager->Acquire<ShaderResource>(
		ResourceKey(vShader, std::make_shared<ShaderResourceParameters>(
			GL_VERTEX_SHADER)));

	auto fragmentShader = g_resourceManager->Acquire<ShaderResource>(
		ResourceKey(fShader, std::make_shared<ShaderResourceParameters>(
			GL_FRAGMENT_SHADER)));

	if (vertexShader.IsValid() && fragmentShader.IsValid())
	{
		return std::make_shared<ShaderProgramResource>(
			std::make_shared<ShaderProgramGLResource>(
				vertexShader, fragmentShader));
	}
	else
	{
		return nullptr;
	}
}
