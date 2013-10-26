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

#ifndef NANAKA_RENDERER_RENDERRESOURCEMANAGER_H
#define NANAKA_RENDERER_RENDERRESOURCEMANAGER_H

#include <memory>
#include <unordered_map>

#include "renderer/GL.h"
#include "renderer/RenderResourceHandle.h"

class RenderResource;

/**
 * RenderResourceManager keeps track of RenderResources, allowing lookup of
 * actual RenderResources as the user of the Renderer refers to them using
 * RenderResourceHandles.
 */
class RenderResourceManager final
{
public:

	RenderResourceHandle GenerateTexture(
		int width,
		int height,
		std::unique_ptr<GLvoid> pixels);
	RenderResourceHandle GenerateFrameBuffer();
	void DestroyResource(RenderResourceHandle resourceHandle);
	std::shared_ptr<RenderResource> Get(
		RenderResourceHandle resourceHandle) const;

private:

	std::unordered_map<RenderResourceHandle, std::shared_ptr<RenderResource>>
		m_renderResources;
};

#endif // NANAKA_RENDERER_RENDERRESOURCEMANAGER_H
