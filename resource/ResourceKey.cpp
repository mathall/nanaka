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

#include "resource/ResourceKey.h"

#include <functional>

#include "resource/ResourceParameters.h"

std::size_t ResourceKey::Hash::operator()(const ResourceKey& key) const
{
	const int prime = 101;

	int hash = 1;

	std::hash<std::string> hashString;
	hash = hash * prime + hashString(key.m_filePath);

	if (key.m_params != nullptr)
	{
		hash = hash * prime + key.m_params->Hash();
	}

	return hash;
}

bool ResourceKey::Equals::operator()(
	const ResourceKey& lhs,
	const ResourceKey& rhs) const
{
	if (&lhs == &rhs)
	{
		return true;
	}

	if (lhs.m_filePath.compare(rhs.m_filePath) != 0)
	{
		return false;
	}

	if (lhs.m_params == nullptr || rhs.m_params == nullptr)
	{
		return lhs.m_params == rhs.m_params;
	}

	return lhs.m_params->Equals(*rhs.m_params);
}
