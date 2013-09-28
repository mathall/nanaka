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

#ifndef NANAKA_SOUND_OGGDECODER_H
#define NANAKA_SOUND_OGGDECODER_H

#include <memory>

#include <ivorbisfile.h>

#include "sound/SoundDecoder.h"

struct OggFile
{
	const char* m_position;
	const char* m_beginning;
	size_t m_size;
};

class OggDecoder final : public SoundDecoder
{
public:

	OggDecoder(const char* data, size_t size);
	~OggDecoder();

	OggDecoder(OggDecoder&) = delete;
	OggDecoder& operator=(const OggDecoder&) = delete;

	/**
	 * SoundDecoder implementation.
	 */
	bool IsValid() const override;

	/**
	 * AudioSource implementation.
	 */
	AudioSamplingRate GetAudioSamplingRate() const override;
	AudioSampleFormat GetAudioSampleFormat() const override;
	size_t NumChannels() const override;
	bool HasData() const override;
	size_t GetData(void* buffer, size_t size) override;
	void Reset() override;

private:

	std::unique_ptr<OggVorbis_File> m_vorbisFile;
	OggFile m_oggFile;
};

inline bool OggDecoder::IsValid() const
{
	return m_vorbisFile != nullptr;
}

inline AudioSampleFormat OggDecoder::GetAudioSampleFormat() const
{
	return AudioSampleFormatFixed16;
}

inline bool OggDecoder::HasData() const
{
	return m_oggFile.m_position != m_oggFile.m_beginning + m_oggFile.m_size;
}

inline void OggDecoder::Reset()
{
	ov_pcm_seek(m_vorbisFile.get(), 0);
}

#endif // NANAKA_SOUND_OGGDECODER_H
