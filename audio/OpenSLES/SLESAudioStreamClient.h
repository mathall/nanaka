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

#ifndef NANAKA_AUDIO_OPENSLES_SLESAUDIOSTREAMCLIENT_H
#define NANAKA_AUDIO_OPENSLES_SLESAUDIOSTREAMCLIENT_H

#include <atomic>
#include <vector>

#include <SLES/OpenSLES.h>

#include "audio/AudioStream.h"

class SLESAudioStreamClient final : public AudioStreamClient
{
public:

	SLESAudioStreamClient(
		size_t numBuffers,
		size_t bufferSize,
		SLObjectItf playerObj);
	~SLESAudioStreamClient();

	SLESAudioStreamClient(SLESAudioStreamClient&) = delete;
	SLESAudioStreamClient& operator=(const SLESAudioStreamClient&) = delete;

	void BufferProcessed();

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

	SLObjectItf m_playerObj;
	SLPlayItf m_player;
	SLBufferQueueItf m_playerBufferQueue;

	std::vector<std::vector<char>> m_buffers;

	std::atomic_uint m_processedBuffers;
};

inline void SLESAudioStreamClient::BufferProcessed()
{
	m_processedBuffers++;
}

inline char* SLESAudioStreamClient::GetBuffer(size_t bufferIdx)
{
	return m_buffers[bufferIdx].data();
}

inline size_t SLESAudioStreamClient::NumProcessedBuffers() const
{
	return m_processedBuffers;
}

inline void SLESAudioStreamClient::UnqueueBuffers(size_t numBuffers)
{
	m_processedBuffers -= numBuffers;
}

inline void SLESAudioStreamClient::EnqueueBuffer(size_t bufferIdx, size_t size)
{
	(*m_playerBufferQueue)->Enqueue(
		m_playerBufferQueue, m_buffers[bufferIdx].data(), size);
}

#endif // NANAKA_AUDIO_OPENSLES_SLESAUDIOSTREAMCLIENT_H
