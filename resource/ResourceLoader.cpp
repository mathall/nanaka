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

#include "resource/ResourceLoader.h"

std::string ResourceLoader::ExtractFileEnding(const std::string& filePath) const
{
	const auto lastSlash = filePath.find_last_of("/");
	const auto lastDot = filePath.find_last_of(".");

	if (lastDot != std::string::npos &&
		(lastSlash == std::string::npos || lastDot > lastSlash) &&
		lastDot < filePath.size())
	{
		return filePath.substr(lastDot);
	}

	return "";
}

std::string ResourceLoader::MakeAbsolutePath(
	std::string thisFile,
	std::string thatFile) const
{
	std::string result = thisFile;

	size_t lastSlash = result.find_last_of("/");
	result = result.substr(0, lastSlash);

	std::string that = thatFile;

	size_t firstSlash = that.find_first_of("/");

	while (firstSlash != std::string::npos)
	{
		std::string str = that.substr(0, firstSlash);
		that = that.substr(firstSlash+1);
		firstSlash = that.find_first_of("/");

		if(str.compare("..") == 0)
		{
			lastSlash = result.find_last_of("/");
			result = result.substr(0, lastSlash);
		}
		else
		{
			result = result + "/" + str;
		}
	}

	result += "/" + that;

	return result;
}
