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

#include "audio/OpenAL/ALAudioDevice.h"

#include <cassert>

#include "audio/OpenAL/ALAudioStreamClient.h"

AudioDevice* AudioDevice::Create()
{
	return new ALAudioDevice();
}

ALAudioDevice::ALAudioDevice()
{
	m_ALDevice = alcOpenDevice(NULL);
	assert(m_ALDevice != NULL);

	m_ALContext = alcCreateContext(m_ALDevice, NULL);
	assert(m_ALContext != NULL);

	alcMakeContextCurrent(m_ALContext);
}

ALAudioDevice::~ALAudioDevice()
{
	alcMakeContextCurrent(NULL);
	alcDestroyContext(m_ALContext);
	alcCloseDevice(m_ALDevice);
}

std::unique_ptr<AudioStream> ALAudioDevice::CreateAudioStream(
	AudioSource* audioSource)
{
	size_t numBuffers;
	size_t bufferSize;
	CalculateStreamBufferNumAndSize(
		audioSource->GetAudioSamplingRate(),
		audioSource->GetAudioSampleFormat(),
		audioSource->NumChannels(),
		numBuffers, bufferSize);

	return std::unique_ptr<AudioStream>(
		new AudioStream(
			new ALAudioStreamClient(audioSource, numBuffers, bufferSize),
			audioSource, numBuffers, bufferSize));
}
