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

#include "graphics/ShaderLoader.h"

#include <cassert>

#include <json.h>

#include "graphics/ShaderResource.h"
#include "resource/ResourceManager.h"

std::shared_ptr<Resource> ShaderLoader::Load(
	const ResourceKey& key,
	const FileManager& fileManager) const
{
	auto params = static_cast<ShaderResourceParameters*>(
		key.GetParams());

	File file;
	fileManager.Open(key.GetFilePath(), file);
	auto shaderJson = file.ReadAll();

	auto deleter = [](json_value* p){ json_value_free(p); };
	auto shaderVal = std::unique_ptr<json_value, decltype(deleter)>(
		json_parse(shaderJson.c_str(),
			shaderJson.length() * sizeof(std::string::value_type)),
		deleter);

	assert(shaderVal->type == json_object);
	auto sourceVal = (*shaderVal)["source"];
	auto uniformsVal = (*shaderVal)["uniforms"];
	auto attributesVal = (*shaderVal)["attributes"];

	assert(sourceVal.type == json_string);
	File sourceFile;
	fileManager.Open(std::string(sourceVal), sourceFile);
	auto source = sourceFile.ReadAll();

	assert(uniformsVal.type == json_array);
	const unsigned int numUniforms = uniformsVal.u.array.length;
	std::vector<std::string> uniforms;
	for (size_t i = 0; i < numUniforms; ++i)
	{
		uniforms.push_back(std::string(uniformsVal[i]));
	}

	assert(attributesVal.type == json_array);
	const unsigned int numAttributes = attributesVal.u.array.length;
	std::vector<std::string> attributes;
	for (size_t i = 0; i < numAttributes; ++i)
	{
		attributes.push_back(std::string(attributesVal[i]));
	}

	auto renderResourceHandle = g_renderer->GenerateShader(
		params->m_type, source, uniforms, attributes);

	return std::make_shared<ShaderResource>(renderResourceHandle);
}
