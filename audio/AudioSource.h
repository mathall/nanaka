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

#ifndef NANAKA_AUDIO_AUDIOSOURCE_H
#define NANAKA_AUDIO_AUDIOSOURCE_H

#include <cstddef>

enum AudioSamplingRate
{
	AudioSamplingRate8000 = 8000,
	AudioSamplingRate44100 = 44100,
};

enum AudioSampleFormat
{
	AudioSampleFormatFixed8 = 1,
	AudioSampleFormatFixed16 = 2,
};

/**
 * Interface for the AudioStream to retreive data.
 */
class AudioSource
{
public:

	virtual ~AudioSource(){}

	virtual AudioSamplingRate GetAudioSamplingRate() const = 0;
	virtual AudioSampleFormat GetAudioSampleFormat() const = 0;
	virtual size_t NumChannels() const = 0;
	virtual bool HasData() const = 0;
	virtual size_t GetData(void* buffer, size_t size) = 0;
	virtual void Reset() = 0;
};

#endif // NANAKA_AUDIO_AUDIOSOURCE_H
