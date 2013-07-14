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

#include "resource/ResourceManager.h"

#include "resource/Resource.h"
#include "resource/ResourceHandle.h"

void ResourceManager::AcquireHelper(
	ResourceKey key,
	std::weak_ptr<Resource>& resource,
	std::shared_ptr<ResourceHandle>& resourceHandle)
{
	auto loadedResource = m_resources.find(key);

	if (loadedResource != m_resources.end())
	{
		resource = loadedResource->second;
		resourceHandle = loadedResource->second->m_resourceHandle.lock();
	}
	else
	{
		resource = Load(key);

		if (resource.lock())
		{
			resourceHandle = std::make_shared<ResourceHandle>(key);
			resource.lock()->m_resourceHandle = resourceHandle;
		}
	}
}

std::weak_ptr<Resource> ResourceManager::Load(ResourceKey key)
{
	std::shared_ptr<Resource> resource;

	for (const auto& loader : m_resourceLoaders)
	{
		if (loader->CanLoad(key, *m_fileManager)
			&& (resource = loader->Load(key, *m_fileManager)))
		{
			m_resources.insert(std::make_pair(key, resource));
		}
	}

	return resource;
}

void ResourceManager::Unload(const ResourceKey& key)
{
	auto resource = m_resources.find(key);

	if (resource != m_resources.end())
	{
		if (auto handle = resource->second->m_resourceHandle.lock())
		{
			handle->Deactivate();
		}
		m_resources.erase(resource);
	}
}

void ResourceManager::OnDestroy()
{
	std::vector<ResourceKey> keys;
	for (auto resource : m_resources)
	{
		keys.push_back(resource.first);
	}

	for (auto key : keys)
	{
		Unload(key);
	}

	m_resourceLoaders.clear();
}
