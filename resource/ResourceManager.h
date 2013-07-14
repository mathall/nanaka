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

#ifndef NANAKA_RESOURCE_RESOURCEMANAGER_H
#define NANAKA_RESOURCE_RESOURCEMANAGER_H

#define g_resourceManager ResourceManager::GetInstance()

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "pi/FileManager.h"
#include "resource/Asset.h"
#include "resource/ResourceKey.h"
#include "resource/ResourceLoader.h"
#include "utils/YOLO.h"

class Resource;

/**
 * ResourceManager is used to acquire Resources via Assets.
 */
class ResourceManager final : public YOLO<ResourceManager>
{
public:

	ResourceManager();

	template<typename R>
	Asset<R> Acquire(std::string filePath);

	template<typename R>
	Asset<R> Acquire(ResourceKey key);

	template<typename L>
	void RegisterLoader();

	/**
	 * YOLO implementation.
	 */
	void OnDestroy() override;

private:

	// ResourceHandle calls Unload when no Asset hold the Resource.
	friend class ResourceHandle;

	// Helper method for aquiring a Resource. "Required" since the Acquire
	// methods are templated and thus necessarily inlined.
	void AcquireHelper(
		ResourceKey key,
		std::weak_ptr<Resource>& resource,
		std::shared_ptr<ResourceHandle>& resourceHandle);

	std::weak_ptr<Resource> Load(ResourceKey key);

	void Unload(const ResourceKey& key);

	std::unique_ptr<FileManager> m_fileManager;

	std::unordered_map<
		ResourceKey,
		std::shared_ptr<Resource>,
		ResourceKey::Hash,
		ResourceKey::Equals> m_resources;

	std::vector<std::unique_ptr<ResourceLoader>> m_resourceLoaders;
};

inline ResourceManager::ResourceManager()
	: m_fileManager(FileManager::Create())
{
}

template<typename R>
inline Asset<R> ResourceManager::Acquire(std::string filePath)
{
	return Acquire<R>(ResourceKey(filePath));
}

template<typename R>
inline Asset<R> ResourceManager::Acquire(ResourceKey key)
{
	std::weak_ptr<Resource> resource;
	std::shared_ptr<ResourceHandle> resourceHandle;

	AcquireHelper(key, resource, resourceHandle);

	return Asset<R>(resource, resourceHandle);
}

template<typename L>
void ResourceManager::RegisterLoader()
{
	m_resourceLoaders.push_back(std::unique_ptr<ResourceLoader>(new L()));
}

#endif // NANAKA_RESOURCE_RESOURCEMANAGER_H
