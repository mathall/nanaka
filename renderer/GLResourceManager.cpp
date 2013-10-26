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

#include "renderer/GLResourceManager.h"

#include "renderer/GLResource.h"

void GLResourceManager::QueueBuiltResourcesForRebuild()
{
	for (auto it = m_builtResources.begin(); it != m_builtResources.end(); ++it)
	{
		QueueResourceForBuild(it->second.resource, it->second.level);
	}

	m_builtResources.clear();
}

void GLResourceManager::ProcessQueues(
	const RenderResourceManager& renderResourceManager)
{
	for (auto queuedGLResource : m_destroyQueue)
	{
		auto builtGLResource = m_builtResources.find(queuedGLResource->GetId());
		if (builtGLResource != m_builtResources.end())
		{
			queuedGLResource->Destroy();
			m_builtResources.erase(builtGLResource);
		}
	}
	m_destroyQueue.clear();

	for (int level = 0; level < BUILD_QUEUE_LEVELS; level++)
	{
		for (auto queuedGLResource : m_buildQueues[level])
		{
			if (auto resource = queuedGLResource.lock())
			{
				resource->Build(renderResourceManager);
				BuiltResource builtResource;
				builtResource.resource = resource;
				builtResource.level = level;
				m_builtResources.insert(
					std::make_pair(resource->GetId(), builtResource));
			}
		}
		m_buildQueues[level].clear();
	}
}
