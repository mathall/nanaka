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

#ifndef NANAKA_RENDERER_GLRESOURCEMANAGER_H
#define NANAKA_RENDERER_GLRESOURCEMANAGER_H

#include <array>
#include <cassert>
#include <memory>
#include <unordered_map>
#include <vector>

#include "utils/UUID.h"

class GLResource;

struct BuiltResource
{
	std::weak_ptr<GLResource> resource;
	int level;
};

/**
 * GLResourceManager keeps track of GLResources as they are being built and
 * destroyed, and rebuilt in case of the Renderer acquiring a new GL context.
 * The GLResource owner is responsible for queueing the GLResource for
 * destruction when it is no longer needed.
 */
class GLResourceManager final
{
public:

	void QueueResourceForBuild(
		std::weak_ptr<GLResource> resource,
		int level = 0);
	void QueueResourceForDestruction(std::shared_ptr<GLResource> resource);
	void QueueBuiltResourcesForRebuild();
	void ProcessQueues();

private:

	const static int BUILD_QUEUE_LEVELS = 2;

	std::unordered_map<UUID, BuiltResource> m_builtResources;
	std::array<std::vector<std::weak_ptr<GLResource>>, BUILD_QUEUE_LEVELS>
		m_buildQueues;
	std::vector<std::shared_ptr<GLResource>> m_destroyQueue;
};

inline void GLResourceManager::QueueResourceForBuild(
	std::weak_ptr<GLResource> resource,
	int level)
{
	assert(level < BUILD_QUEUE_LEVELS);
	m_buildQueues[level].push_back(resource);
}

inline void GLResourceManager::QueueResourceForDestruction(
	std::shared_ptr<GLResource> resource)
{
	m_destroyQueue.push_back(resource);
}

#endif // NANAKA_RENDERER_GLRESOURCEMANAGER_H
