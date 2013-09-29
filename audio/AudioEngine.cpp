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

#include "audio/AudioEngine.h"

#include <algorithm>
#include <cassert>

AudioEngine::AudioEngine()
	: m_device(AudioDevice::Create())
{
}

void AudioEngine::OnDestroy()
{
	m_streams.clear();
	m_abandonedStreams.clear();

	m_device = nullptr;
}

AudioStreamHandle AudioEngine::CreateAudioStream(AudioSource* audioSource)
{
	auto stream = m_device->CreateAudioStream(audioSource);
	const auto streamId = stream->GetId();
	m_streams.insert(std::make_pair(streamId, std::move(stream)));
	return AudioStreamHandle(streamId);
}

void AudioEngine::PlayOnce(AudioStreamHandle streamHandle)
{
	if (streamHandle.IsValid())
	{
		auto streamIt = m_streams.find(streamHandle.GetId());
		if (streamIt != m_streams.end())
		{
			streamIt->second->Play(AudioStreamPlayModePlayOnce);
		}
	}
}

void AudioEngine::Repeat(AudioStreamHandle streamHandle)
{
	if (streamHandle.IsValid())
	{
		auto streamIt = m_streams.find(streamHandle.GetId());
		if (streamIt != m_streams.end())
		{
			streamIt->second->Play(AudioStreamPlayModeLoop);
		}
	}
}

void AudioEngine::Stop(AudioStreamHandle streamHandle)
{
	if (streamHandle.IsValid())
	{
		auto streamIt = m_streams.find(streamHandle.GetId());
		if (streamIt != m_streams.end())
		{
			streamIt->second->Stop();
		}
	}
}

void AudioEngine::Pause(AudioStreamHandle streamHandle)
{
	if (streamHandle.IsValid())
	{
		auto streamIt = m_streams.find(streamHandle.GetId());
		if (streamIt != m_streams.end())
		{
			streamIt->second->Pause();
		}
	}
}

void AudioEngine::Unpause(AudioStreamHandle streamHandle)
{
	if (streamHandle.IsValid())
	{
		auto streamIt = m_streams.find(streamHandle.GetId());
		if (streamIt != m_streams.end())
		{
			streamIt->second->Unpause();
		}
	}
}

void AudioEngine::DestroyAudioStream(UUID streamId)
{
	auto streamIt = m_streams.find(streamId);
	assert(streamIt != m_streams.end());

	bool stashStream = streamIt->second->HasPendingBuffer();
	streamIt->second->NullAudioSource();
	streamIt->second->Stop();
	if (stashStream)
	{
		m_abandonedStreams.push_back(std::move(streamIt->second));
	}

	m_streams.erase(streamIt);
}

void AudioEngine::Update()
{
	for (auto& streamIt : m_streams)
	{
		streamIt.second->UpdateBuffers();
	}

	m_abandonedStreams.clear();
}
