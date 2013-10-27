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
#include <vector>

#include "math/Vec2f.h"
#include "renderer/GL.h"
#include "renderer/RenderResource.h"
#include "renderer/RenderResourceHandle.h"

/**
 * RenderResourceManager keeps track of RenderResources, allowing lookup of
 * actual RenderResources as the user of the Renderer refers to them using
 * RenderResourceHandles.
 */
class RenderResourceManager final
{
public:

	template<typename T, typename... Args>
	RenderResourceHandle GenerateResource(Args... args);

	void DestroyResource(RenderResourceHandle resourceHandle);

	void QueueBuiltResourcesForRebuild();
	void ProcessQueues();

	RenderResource* Get(RenderResourceHandle resourceHandle) const;

	template<typename T>
	T* Get(RenderResourceHandle resourceHandle) const;

private:

	std::unordered_map<RenderResourceHandle, std::unique_ptr<RenderResource>>
		m_renderResources;

	std::vector<RenderResourceHandle> m_buildQueue;
	std::vector<RenderResourceHandle> m_destroyQueue;

	// Kept as members to avoid unnecessary heap allocations.
	std::vector<RenderResourceHandle> m_failedBuilds;
};

template<typename T, typename... Args>
RenderResourceHandle RenderResourceManager::GenerateResource(Args... args)
{
	auto renderResource = std::unique_ptr<T>(
		new T(std::forward<Args>(args)...));
	auto resourceHandle = renderResource->GetHandle();

	m_renderResources.insert(std::make_pair(
		resourceHandle, std::move(renderResource)));
	m_buildQueue.push_back(resourceHandle);

	return resourceHandle;
}

template<typename T>
inline T* RenderResourceManager::Get(RenderResourceHandle resourceHandle) const
{
	if (auto renderResource = Get(resourceHandle))
	{
		if (renderResource->GetType() == T::s_type)
		{
			return static_cast<T*>(renderResource);
		}
	}

	return NULL;
}

#endif // NANAKA_RENDERER_RENDERRESOURCEMANAGER_H
