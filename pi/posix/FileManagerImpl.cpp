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

#include "FileManagerImpl.h"

#if defined(OS_OSX)
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif

FileManager* FileManager::Create()
{
	return new FileManagerImpl();
}

std::string FileHandleImpl::ReadAll() const
{
	std::string contents;
	fseek(m_file, 0, SEEK_END);
	contents.resize(ftell(m_file));
	fseek(m_file, 0, SEEK_SET);
	Read(&contents[0], contents.size());
	return contents;
}

void FileManagerImpl::Open(const std::string& filePath, File& file) const
{
	auto path = m_prefix + filePath;
	if (std::FILE* cfile = fopen(path.c_str(), "r"))
	{
		file.handle = std::unique_ptr<FileHandleImpl>(
			new FileHandleImpl(cfile));
	}
}

std::string FileManagerImpl::MakeAssetsFilePath() const
{
	char buf[1024];
	uint32_t len = sizeof(buf);
#if defined(OS_OSX)
	_NSGetExecutablePath(buf, &len);
#else
	len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
#endif
	buf[len] = '\0';
	std::string binPath(buf);
	return binPath.substr(0, binPath.find_last_of('/') + 1).append("assets/");
}
