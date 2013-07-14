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

#ifndef NANAKA_RESOURCE_RESOURCEHANDLE_H
#define NANAKA_RESOURCE_RESOURCEHANDLE_H

#include "resource/ResourceKey.h"
#include "resource/ResourceManager.h"

/**
 * ResourceHandle instructs the ResourceManager to unload a Resource when the
 * last Asset holding it has destructed.
 */
class ResourceHandle final
{
public:

	explicit ResourceHandle(ResourceKey key);
	~ResourceHandle();

	ResourceHandle(ResourceHandle&) = delete;
	ResourceHandle& operator=(const ResourceHandle&) = delete;

	// ResourceHandle may outlive the ResourceManager on shutdown, thus it needs
	// to be deactivated.
	void Deactivate();

private:

	const ResourceKey m_key;
	bool m_active;
};

inline ResourceHandle::ResourceHandle(ResourceKey key)
	: m_key(key)
	, m_active(true)
{
}

inline ResourceHandle::~ResourceHandle()
{
	if (m_active)
	{
		g_resourceManager->Unload(m_key);
	}
}

inline void ResourceHandle::Deactivate()
{
	m_active = false;
}

#endif // NANAKA_RESOURCE_RESOURCEHANDLE_H
