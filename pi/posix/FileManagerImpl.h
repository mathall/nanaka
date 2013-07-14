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

#ifndef NANAKA_PI_POSIX_FILEMANAGERIMPL_H
#define NANAKA_PI_POSIX_FILEMANAGERIMPL_H

#include "nanaka/pi/FileManager.h"

class FileHandleImpl final : public FileHandle
{
public:

	explicit FileHandleImpl(std::FILE* cfile);
	~FileHandleImpl();

	FileHandleImpl(FileHandleImpl&) = delete;
	FileHandleImpl& operator=(const FileHandleImpl&) = delete;

	/**
	 * FileHandle implementation.
	 */
	int Read(void* buffer, int bytes) const override;
	std::string ReadAll() const override;

private:

	std::FILE* m_file;
};

inline FileHandleImpl::FileHandleImpl(std::FILE* cfile)
	: m_file(cfile)
{
}

inline FileHandleImpl::~FileHandleImpl()
{
	fclose(m_file);
}

inline int FileHandleImpl::Read(void* buffer, int bytes) const
{
	return fread(buffer, sizeof(char), bytes, m_file);
}

class FileManagerImpl final : public FileManager
{
public:

	FileManagerImpl();

	/**
	 * FileManager implementation.
	 */
	void Open(const std::string& filePath, File& file) const override;

private:

	std::string MakeAssetsFilePath() const;
};

inline FileManagerImpl::FileManagerImpl()
	: FileManager(MakeAssetsFilePath())
{
}

#endif // NANAKA_PI_POSIX_FILEMANAGERIMPL_H
