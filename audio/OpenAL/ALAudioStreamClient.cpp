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

#include "audio/OpenAL/ALAudioStreamClient.h"

#include <cassert>

ALAudioStreamClient::ALAudioStreamClient(
	AudioSource* audioSource,
	size_t numBuffers,
	size_t bufferSize)
{
	m_buffer.resize(bufferSize);

	alGenSources(1, &m_ALSource);
	alSourcef(m_ALSource, AL_PITCH, 1);
	alSourcef(m_ALSource, AL_GAIN, 1);
	alSource3f(m_ALSource, AL_POSITION, 0, 0, 0);
	alSource3f(m_ALSource, AL_VELOCITY, 0, 0, 0);
	alSourcei(m_ALSource, AL_LOOPING, AL_FALSE);

	m_ALBuffers.resize(numBuffers);
	alGenBuffers(numBuffers, m_ALBuffers.data());

	m_samplingRate = audioSource->GetAudioSamplingRate();

	switch (audioSource->GetAudioSampleFormat())
	{
	case AudioSampleFormatFixed8:
		m_sampleFormat = audioSource->NumChannels() == 1 ?
			AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
		break;

	case AudioSampleFormatFixed16:
		m_sampleFormat = audioSource->NumChannels() == 1 ?
			AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		break;
	}
}

ALAudioStreamClient::~ALAudioStreamClient()
{
	alDeleteSources(1, &m_ALSource);
	alDeleteBuffers(m_ALBuffers.size(), m_ALBuffers.data());
}

size_t ALAudioStreamClient::NumProcessedBuffers() const
{
	ALint processedBuffers = 0;
	alGetSourcei(m_ALSource, AL_BUFFERS_PROCESSED, &processedBuffers);
	return processedBuffers;
}

void ALAudioStreamClient::UnqueueBuffers(size_t numBuffers)
{
	while (numBuffers > 0)
	{
		ALuint unqueuedBuffer;
		alSourceUnqueueBuffers(m_ALSource, 1, &unqueuedBuffer);
		numBuffers--;
	}
}

void ALAudioStreamClient::EnqueueBuffer(size_t bufferIdx, size_t size)
{
	alBufferData(m_ALBuffers[bufferIdx],
		m_sampleFormat, m_buffer.data(), size, m_samplingRate);
	alSourceQueueBuffers(m_ALSource, 1, &m_ALBuffers[bufferIdx]);
}

void ALAudioStreamClient::ResetStream()
{
	ALint numQueuedBuffers = 0;
	alGetSourcei(m_ALSource, AL_BUFFERS_QUEUED, &numQueuedBuffers);
	UnqueueBuffers(numQueuedBuffers);
}

AudioStreamState ALAudioStreamClient::GetState() const
{
	ALint sourceState;
	alGetSourcei(m_ALSource, AL_SOURCE_STATE, &sourceState);
	return sourceState == AL_PLAYING ? AudioStreamStatePlaying
		: sourceState == AL_STOPPED ? AudioStreamStateStopped
		: sourceState == AL_PAUSED ? AudioStreamStatePaused
		: AudioStreamStateUnknown;
}
