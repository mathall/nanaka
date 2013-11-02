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

#ifndef NANAKA_GRAPHICS_MODELRESOURCE_H
#define NANAKA_GRAPHICS_MODELRESOURCE_H

#include "graphics/MaterialResource.h"
#include "renderer/Renderer.h"
#include "renderer/RenderResourceHandle.h"
#include "resource/Asset.h"
#include "resource/Resource.h"

class ModelResource final : public Resource
{
public:

	ModelResource(
		RenderResourceHandle indexBufferHandle,
		RenderResourceHandle positionBufferHandle,
		RenderResourceHandle texcoordBufferHandle,
		Asset<MaterialResource> materialAsset);
	~ModelResource();

	ModelResource(ModelResource&) = delete;
	ModelResource& operator=(const ModelResource&) = delete;

	RenderResourceHandle GetIndexBufferHandle() const;
	RenderResourceHandle GetPositionBufferHandle() const;
	RenderResourceHandle GetTexcoordBufferHandle() const;

	Asset<MaterialResource> GetMaterialAsset() const;

private:

	RenderResourceHandle m_indexBufferHandle;
	RenderResourceHandle m_positionBufferHandle;
	RenderResourceHandle m_texcoordBufferHandle;

	Asset<MaterialResource> m_materialAsset;
};

inline ModelResource::ModelResource(
	RenderResourceHandle indexBufferHandle,
	RenderResourceHandle positionBufferHandle,
	RenderResourceHandle texcoordBufferHandle,
	Asset<MaterialResource> materialAsset)
	: m_indexBufferHandle(indexBufferHandle)
	, m_positionBufferHandle(positionBufferHandle)
	, m_texcoordBufferHandle(texcoordBufferHandle)
	, m_materialAsset(materialAsset)
{
}

inline ModelResource::~ModelResource()
{
	g_renderer->DestroyRenderResource(m_indexBufferHandle);
	g_renderer->DestroyRenderResource(m_positionBufferHandle);
	g_renderer->DestroyRenderResource(m_texcoordBufferHandle);
}

inline RenderResourceHandle ModelResource::GetIndexBufferHandle() const
{
	return m_indexBufferHandle;
}

inline RenderResourceHandle ModelResource::GetPositionBufferHandle() const
{
	return m_positionBufferHandle;
}

inline RenderResourceHandle ModelResource::GetTexcoordBufferHandle() const
{
	return m_texcoordBufferHandle;
}

inline Asset<MaterialResource> ModelResource::GetMaterialAsset() const
{
	return m_materialAsset;
}

#endif // NANAKA_GRAPHICS_MODELRESOURCE_H
