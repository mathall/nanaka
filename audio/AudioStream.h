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

#ifndef NANAKA_AUDIO_AUDIOSTREAM_H
#define NANAKA_AUDIO_AUDIOSTREAM_H

#include <memory>

#include "audio/AudioSource.h"
#include "utils/UUID.h"

enum AudioStreamState
{
	AudioStreamStateStopped,
	AudioStreamStatePlaying,
	AudioStreamStatePaused,
	AudioStreamStateUnknown,
};

enum AudioStreamPlayMode
{
	AudioStreamPlayModePlayOnce,
	AudioStreamPlayModeLoop,
};

/**
 * Interface for AudioStream to control backend specific to the AudioDevice.
 */
class AudioStreamClient
{
public:

	virtual ~AudioStreamClient(){}

	virtual char* GetBuffer(size_t bufferIdx) = 0;
	virtual size_t NumProcessedBuffers() const = 0;
	virtual void UnqueueBuffers(size_t numBuffers) = 0;
	virtual void EnqueueBuffer(size_t bufferIdx, size_t size) = 0;
	virtual void PlayStream() = 0;
	virtual void StopStream() = 0;
	virtual void PauseStream() = 0;
	virtual void ResetStream() = 0;
	virtual AudioStreamState GetState() const = 0;
};

/**
 * AudioStream acts as an intermediary between the AudioSource and the
 * AudioStreamClient. It manages buffer and play state, querying the
 * AudioStreamClient and supplying it with new data as necessary.
 */
class AudioStream final
{
public:

	AudioStream(
		AudioStreamClient* client,
		AudioSource* audioSource,
		size_t numBuffers,
		size_t bufferSize);

	void Play(AudioStreamPlayMode playMode);
	void Stop();
	void Pause();
	void Unpause();

	void UpdateBuffers();
	bool HasPendingBuffer() const;
	void NullAudioSource();

	UUID GetId() const;

private:

	void FillBuffers();

	UUID m_id;

	std::unique_ptr<AudioStreamClient> m_client;

	AudioSource* m_source;

	const size_t m_numBuffers;
	const size_t m_bufferSize;
	size_t m_bufferHead;
	size_t m_bufferTail;
	bool m_bufferHasData;

	AudioStreamState m_state;
	AudioStreamPlayMode m_playMode;
};

inline UUID AudioStream::GetId() const
{
	return m_id;
}

inline bool AudioStream::HasPendingBuffer() const
{
	return m_bufferHasData && m_state == AudioStreamStatePlaying;
}

inline void AudioStream::NullAudioSource()
{
	m_source = NULL;
}

#endif // NANAKA_AUDIO_AUDIOSTREAM_H
