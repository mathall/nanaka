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

#ifndef NANAKA_AUDIO_OPENAL_ALAUDIOSTREAMCLIENT_H
#define NANAKA_AUDIO_OPENAL_ALAUDIOSTREAMCLIENT_H

#include <vector>

#include <AL/al.h>

#include "audio/AudioStream.h"

class ALAudioStreamClient final : public AudioStreamClient
{
public:

	ALAudioStreamClient(
		AudioSource* audioSource,
		size_t numBuffers,
		size_t bufferSize);
	~ALAudioStreamClient();

	ALAudioStreamClient(ALAudioStreamClient&) = delete;
	ALAudioStreamClient& operator=(const ALAudioStreamClient&) = delete;

	/**
	 * AudioStreamClient implementation.
	 */
	char* GetBuffer(size_t bufferIdx) override;
	size_t NumProcessedBuffers() const override;
	void UnqueueBuffers(size_t numBuffers) override;
	void EnqueueBuffer(size_t bufferIdx, size_t size) override;
	void PlayStream() override;
	void StopStream() override;
	void PauseStream() override;
	void ResetStream() override;
	AudioStreamState GetState() const override;

private:

	ALuint m_ALSource;
	std::vector<ALuint> m_ALBuffers;
	ALsizei m_samplingRate;
	ALenum m_sampleFormat;

	std::vector<char> m_buffer;
};

inline char* ALAudioStreamClient::GetBuffer(size_t bufferIdx)
{
	return m_buffer.data();
}

inline void ALAudioStreamClient::PlayStream()
{
	alSourcePlay(m_ALSource);
}

inline void ALAudioStreamClient::StopStream()
{
	alSourceStop(m_ALSource);
}

inline void ALAudioStreamClient::PauseStream()
{
	alSourcePause(m_ALSource);
}

#endif // NANAKA_AUDIO_OPENAL_ALAUDIOSTREAMCLIENT_H
