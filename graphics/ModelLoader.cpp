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

#include "graphics/ModelLoader.h"

#include <cassert>

#include <json.h>

#include "graphics/ModelResource.h"
#include "resource/ResourceManager.h"

std::shared_ptr<Resource> ModelLoader::Load(
	const ResourceKey& key,
	const FileManager& fileManager) const
{
	File file;
	fileManager.Open(key.GetFilePath(), file);
	auto modelJson = file.ReadAll();

	auto deleter = [](json_value* p){ json_value_free(p); };
	auto modelVal = std::unique_ptr<json_value, decltype(deleter)>(
		json_parse(modelJson.c_str(),
			modelJson.length() * sizeof(std::string::value_type)),
		deleter);

	assert(modelVal->type == json_object);
	auto positionsVal = (*modelVal)["positions"];
	auto texcoordsVal = (*modelVal)["texcoords"];
	auto indicesVal = (*modelVal)["indices"];
	auto materialVal = (*modelVal)["material"];

	// Construct vertex buffer
	assert(positionsVal.type == json_array);
	const unsigned int numPositions = positionsVal.u.array.length;
	auto positionBuffer = std::unique_ptr<GLfloat[]>(new GLfloat[numPositions]);
	for (size_t i = 0; i < numPositions; ++i)
	{
		positionBuffer.get()[i] = positionsVal[i].u.dbl;
	}

	// Construct texture coordinate buffer
	assert(texcoordsVal.type == json_array);
	const unsigned int numTexcoords = texcoordsVal.u.array.length;
	auto texcoordBuffer = std::unique_ptr<GLfloat[]>(new GLfloat[numTexcoords]);
	for (size_t i = 0; i < numTexcoords; ++i)
	{
		texcoordBuffer.get()[i] = texcoordsVal[i].u.dbl;
	}

	// Construct index buffer
	assert(indicesVal.type == json_array);
	const unsigned int numIndices = indicesVal.u.array.length;
	auto indexBuffer = std::unique_ptr<GLushort[]>(new GLushort[numIndices]);
	for (size_t i = 0; i < numIndices; ++i)
	{
		indexBuffer.get()[i] = indicesVal[i].u.integer;
	}

	assert(materialVal.type == json_string);
	auto material = g_resourceManager->Acquire<MaterialResource>(
		ResourceKey(std::string(materialVal)));

	auto positionBufferHandle = g_renderer->GenerateVertexBuffer(
		std::move(positionBuffer), numPositions);

	auto texcoordBufferHandle = g_renderer->GenerateVertexBuffer(
		std::move(texcoordBuffer), numTexcoords);

	auto indexBufferHandle = g_renderer->GenerateIndexBuffer(
		std::move(indexBuffer), numIndices);

	return std::make_shared<ModelResource>(
		indexBufferHandle,
		positionBufferHandle,
		texcoordBufferHandle,
		material);
}
