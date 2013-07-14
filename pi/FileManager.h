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

#ifndef NANAKA_PI_FILEMANAGER_H
#define NANAKA_PI_FILEMANAGER_H

#include <memory>
#include <string>

class FileHandle
{
public:

	virtual ~FileHandle(){}

	virtual int Read(void* buffer, int bytes) const = 0;
	virtual std::string ReadAll() const = 0;
};

class File final
{
public:

	int Read(void* buffer, int bytes) const;
	std::string ReadAll() const;

	std::unique_ptr<FileHandle> handle;
};

inline int File::Read(void* buffer, int bytes) const
{
	return handle->Read(buffer, bytes);
}

inline std::string File::ReadAll() const
{
	return handle->ReadAll();
}

class FileManager
{
public:

	static FileManager* Create();

	explicit FileManager(std::string prefix);
	virtual ~FileManager(){}

	virtual void Open(const std::string& filePath, File& file) const = 0;

protected:

	const std::string m_prefix;
};

inline FileManager::FileManager(std::string prefix)
	: m_prefix(prefix)
{
}

#endif // NANAKA_PI_FILEMANAGER_H
