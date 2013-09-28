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

#include "audio/OpenSLES/SLESAudioStreamClient.h"

#include <cassert>

static void playerCallback(SLBufferQueueItf bufferQueue, void* context)
{
	static_cast<SLESAudioStreamClient*>(context)->BufferProcessed();
}

SLESAudioStreamClient::SLESAudioStreamClient(
	size_t numBuffers,
	size_t bufferSize,
	SLObjectItf playerObj)
	: m_playerObj(playerObj)
	, m_processedBuffers(0)
{
	m_buffers.resize(numBuffers);
	for (auto& buffer : m_buffers)
	{
		buffer.resize(bufferSize);
	}

	SLresult result;

	result = (*m_playerObj)->Realize(m_playerObj, SL_BOOLEAN_FALSE);
	assert(SL_RESULT_SUCCESS == result);

	result = (*m_playerObj)->GetInterface(m_playerObj, SL_IID_PLAY, &m_player);
	assert(SL_RESULT_SUCCESS == result);

	result = (*m_playerObj)->GetInterface(
		m_playerObj, SL_IID_BUFFERQUEUE, &m_playerBufferQueue);
	assert(SL_RESULT_SUCCESS == result);

	result = (*m_playerBufferQueue)->RegisterCallback(
		m_playerBufferQueue, playerCallback, this);
	assert(SL_RESULT_SUCCESS == result);

	(void)result;
}

SLESAudioStreamClient::~SLESAudioStreamClient()
{
	if (m_playerObj != NULL)
	{
		SLuint32 playerState;
		(*m_playerObj)->GetState(m_playerObj, &playerState);

		if (playerState == SL_OBJECT_STATE_REALIZED)
		{
			(*m_playerObj)->AbortAsyncOperation(m_playerObj);
			(*m_playerObj)->Destroy(m_playerObj);
		}

		m_playerObj = NULL;
		m_player = NULL;
		m_playerBufferQueue = NULL;
	}
}

void SLESAudioStreamClient::PlayStream()
{
	SLresult result = (*m_player)->SetPlayState(m_player, SL_PLAYSTATE_PLAYING);
	assert(SL_RESULT_SUCCESS == result);
	(void)result;
}

void SLESAudioStreamClient::StopStream()
{
	SLresult result = (*m_player)->SetPlayState(m_player, SL_PLAYSTATE_STOPPED);
	assert(SL_RESULT_SUCCESS == result);
	(void)result;
}

void SLESAudioStreamClient::PauseStream()
{
	SLresult result = (*m_player)->SetPlayState(m_player, SL_PLAYSTATE_PAUSED);
	assert(SL_RESULT_SUCCESS == result);
	(void)result;
}

void SLESAudioStreamClient::ResetStream()
{
	(*m_playerBufferQueue)->Clear(m_playerBufferQueue);
	m_processedBuffers = 0;
}

AudioStreamState SLESAudioStreamClient::GetState() const
{
	SLuint32 playState;
	(*m_player)->GetPlayState(m_player, &playState);
	return playState == SL_PLAYSTATE_PLAYING ? AudioStreamStatePlaying
		: playState == SL_PLAYSTATE_STOPPED ? AudioStreamStateStopped
		: playState == SL_PLAYSTATE_PAUSED ? AudioStreamStatePaused
		: AudioStreamStateUnknown;
}
