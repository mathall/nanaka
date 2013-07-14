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

#ifndef NANAKA_RESOURCE_RESOURCEKEY_H
#define NANAKA_RESOURCE_RESOURCEKEY_H

#include <cstddef>
#include <memory>
#include <string>

class ResourceParameters;

/**
 * A ResourceKey uniquely identifies a Resource by a file path and possibly
 * extra ResourceParameters.
 */
class ResourceKey final
{
public:

	explicit ResourceKey(std::string filePath);
	ResourceKey(
		std::string filePath,
		std::shared_ptr<ResourceParameters> params);

	const std::string& GetFilePath() const;
	ResourceParameters* GetParams() const;

	struct Hash
	{
		std::size_t operator()(const ResourceKey& key) const;
	};

	struct Equals
	{
		bool operator()(const ResourceKey& lhs, const ResourceKey& rhs) const;
	};

private:

	std::string m_filePath;

	// The key may be copied around, so the parameters are shared among copies
	// of the same key.
	std::shared_ptr<ResourceParameters> m_params;
};

inline ResourceKey::ResourceKey(std::string filePath)
	: m_filePath(filePath)
	, m_params(nullptr)
{
}

inline ResourceKey::ResourceKey(
	std::string filePath,
	std::shared_ptr<ResourceParameters> params)
	: m_filePath(filePath)
	, m_params(params)
{
}

inline const std::string& ResourceKey::GetFilePath() const
{
	return m_filePath;
}

inline ResourceParameters* ResourceKey::GetParams() const
{
	return m_params.get();
}

#endif // NANAKA_RESOURCE_RESOURCEKEY_H
