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

#ifndef NANAKA_RESOURCE_RESOURCELOADER_H
#define NANAKA_RESOURCE_RESOURCELOADER_H

#include <memory>
#include <string>

#include "pi/FileManager.h"
#include "resource/ResourceKey.h"

class Resource;
class ResourceKey;

/**
 * ResourceLoader may be implemented to create loaders of any file formats.
 */
class ResourceLoader
{
public:

	virtual ~ResourceLoader(){}

	virtual std::shared_ptr<Resource> Load(
		const ResourceKey& key,
		const FileManager& fileManager) const = 0;

	virtual bool CanLoad(
		const ResourceKey& key,
		const FileManager& fileManager) const = 0;

protected:

	std::string ExtractFileEnding(const std::string& filePath) const;

	// Makes an absolute path to thatFile referenced relatively by thisFile.
	std::string MakeAbsolutePath(
		std::string thisFile,
		std::string thatFile) const;
};

#endif // NANAKA_RESOURCE_RESOURCELOADER_H
