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

#include "sound/OggDecoder.h"

#include <algorithm>
#include <cassert>

size_t oggCallbackRead(
	void* buffer,
	size_t numElem,
	size_t elemSize,
	void* dataSource)
{
	OggFile* oggFile = static_cast<OggFile*>(dataSource);

	size_t size = numElem * elemSize;
	if (oggFile->m_position + size > oggFile->m_beginning + oggFile->m_size)
	{
		size = oggFile->m_beginning + oggFile->m_size - oggFile->m_position;
	}

	std::copy(oggFile->m_position, oggFile->m_position + size,
		static_cast<char*>(buffer));
	oggFile->m_position += size;

	return size;
}

int oggCallbackSeek(void *dataSource, ogg_int64_t offset, int seekType)
{
	OggFile* oggFile = static_cast<OggFile*>(dataSource);

	switch (seekType)
	{
	case SEEK_CUR:
		oggFile->m_position += offset;
		break;

	case SEEK_END:
		oggFile->m_position = oggFile->m_beginning + oggFile->m_size - offset;
		break;

	case SEEK_SET:
		oggFile->m_position = oggFile->m_beginning + offset;
		break;

	default:
		return -1;
	}

	if (oggFile->m_position < oggFile->m_beginning)
	{
		oggFile->m_position = oggFile->m_beginning;
		return -1;
	}

	if (oggFile->m_position > oggFile->m_beginning + oggFile->m_size)
	{
		oggFile->m_position = oggFile->m_beginning + oggFile->m_size;
		return -1;
	}

	return 0;
}

int oggCallbackClose(void* dataSource)
{
	return 0;
}

long oggCallbackTell(void *dataSource)
{
	OggFile* oggFile = static_cast<OggFile*>(dataSource);
	return oggFile->m_position - oggFile->m_beginning;
}

OggDecoder::OggDecoder(const char* data, size_t size)
	: m_vorbisFile(new OggVorbis_File())
	, m_oggFile({data, data, size})
{
	ov_callbacks callbacks;
	callbacks.read_func = oggCallbackRead;
	callbacks.seek_func = oggCallbackSeek;
	callbacks.close_func = oggCallbackClose;
	callbacks.tell_func = oggCallbackTell;

	if (ov_open_callbacks(
		&m_oggFile, m_vorbisFile.get(), NULL, -1, callbacks) < 0)
	{
		m_vorbisFile = nullptr;
	}
}

OggDecoder::~OggDecoder()
{
	if (m_vorbisFile)
	{
		ov_clear(m_vorbisFile.get());
	}
}

AudioSamplingRate OggDecoder::GetAudioSamplingRate() const
{
	vorbis_info* info = ov_info(m_vorbisFile.get(), -1);
	assert(info);

	AudioSamplingRate samplingRate = AudioSamplingRate44100;

	switch (info->rate)
	{
	case 8000:
		samplingRate = AudioSamplingRate8000;
		break;

	case 44100:
		samplingRate = AudioSamplingRate44100;
		break;

	default:
		assert(!"Unsupported sampling rate.");
		break;
	}

	return samplingRate;
}

size_t OggDecoder::NumChannels() const
{
	vorbis_info* info = ov_info(m_vorbisFile.get(), -1);
	assert(info);

	return info->channels;
}

size_t OggDecoder::GetData(void* buffer, size_t size)
{
	auto byteBuffer = static_cast<char*>(buffer);
	size_t toRead = size;
	const size_t maxSize = 4096;

	while (toRead > 0)
	{
		int bitstream;
		auto bytesRead = ov_read(m_vorbisFile.get(), byteBuffer,
			toRead >= maxSize ? maxSize : toRead, &bitstream);

		if (bytesRead <= 0)
		{
			break;
		}

		byteBuffer += bytesRead;
		toRead -= bytesRead;
	}

	return size - toRead;
}
