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

#include "audio/AudioStream.h"

#include <cassert>

AudioStream::AudioStream(
	AudioStreamClient* client,
	AudioSource* audioSource,
	size_t numBuffers,
	size_t bufferSize)
	: m_id(UUID::New())
	, m_client(client)
	, m_source(audioSource)
	, m_numBuffers(numBuffers)
	, m_bufferSize(bufferSize)
	, m_bufferHead(0)
	, m_bufferTail(0)
	, m_bufferHasData(false)
	, m_state(AudioStreamStateStopped)
{
}

void AudioStream::FillBuffers()
{
	while (m_state != AudioStreamStateStopped && m_source->HasData()
		&& !(m_bufferHasData && m_bufferHead == m_bufferTail))
	{
		const auto buffer = m_client->GetBuffer(m_bufferHead);

		auto bytesRead = m_source->GetData(buffer, m_bufferSize);

		if (m_playMode == AudioStreamPlayModeLoop)
		{
			while (bytesRead < m_bufferSize)
			{
				m_source->Reset();
				bytesRead += m_source->GetData(
					buffer + bytesRead, m_bufferSize - bytesRead);
			}

			if (!m_source->HasData())
			{
				m_source->Reset();
			}
		}

		m_client->EnqueueBuffer(m_bufferHead, bytesRead);

		m_bufferHasData = true;
		m_bufferHead = (m_bufferHead + 1) % m_numBuffers;
	}
}

void AudioStream::UpdateBuffers()
{
	const auto processedBuffers = m_client->NumProcessedBuffers();

	if (processedBuffers == 0)
	{
		return;
	}

	m_client->UnqueueBuffers(processedBuffers);

	m_bufferTail = (m_bufferTail + processedBuffers) % m_numBuffers;

	if (m_bufferTail == m_bufferHead)
	{
		m_bufferHasData = false;
	}

	if (m_source)
	{
		FillBuffers();

		if (!m_source->HasData() && !m_bufferHasData)
		{
			Stop();
		}
		else if (m_state == AudioStreamStatePlaying &&
			m_client->GetState() != AudioStreamStatePlaying)
		{
			m_client->PlayStream();
		}
	}
}

void AudioStream::Play(AudioStreamPlayMode playMode)
{
	if (m_state != AudioStreamStateStopped)
	{
		Stop();
	}
	m_state = AudioStreamStatePlaying;
	m_playMode = playMode;
	FillBuffers();
	m_client->PlayStream();
}

void AudioStream::Stop()
{
	m_state = AudioStreamStateStopped;
	m_client->StopStream();

	m_bufferHead = 0;
	m_bufferTail = 0;
	m_bufferHasData = false;
	if (m_source)
	{
		m_source->Reset();
	}
	m_client->ResetStream();
}

void AudioStream::Pause()
{
	if (m_state == AudioStreamStatePlaying)
	{
		m_state = AudioStreamStatePaused;
		m_client->PauseStream();
	}
}

void AudioStream::Unpause()
{
	if (m_state == AudioStreamStatePaused)
	{
		m_state = AudioStreamStatePlaying;
		m_client->PlayStream();
	}
}
