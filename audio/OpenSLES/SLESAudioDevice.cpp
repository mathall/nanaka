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

#include "audio/OpenSLES/SLESAudioDevice.h"

#include <cassert>

#include "audio/OpenSLES/SLESAudioStreamClient.h"

AudioDevice* AudioDevice::Create()
{
	return new SLESAudioDevice();
}

SLESAudioDevice::SLESAudioDevice()
{
	SLresult result;

	result = slCreateEngine(&m_engineObj, 0, NULL, 0, NULL, NULL);
	assert(SL_RESULT_SUCCESS == result);

	result = (*m_engineObj)->Realize(m_engineObj, SL_BOOLEAN_FALSE);
	assert(SL_RESULT_SUCCESS == result);

	result = (*m_engineObj)->GetInterface(
		m_engineObj, SL_IID_ENGINE, &m_engine);
	assert(SL_RESULT_SUCCESS == result);

	result = (*m_engine)->CreateOutputMix(
		m_engine, &m_outputMixObj, 0, NULL, NULL);
	assert(SL_RESULT_SUCCESS == result);

	result = (*m_outputMixObj)->Realize(m_outputMixObj, SL_BOOLEAN_FALSE);
	assert(SL_RESULT_SUCCESS == result);

	(void)result;
}

SLESAudioDevice::~SLESAudioDevice()
{
	if (m_outputMixObj != NULL)
	{
		(*m_outputMixObj)->Destroy(m_outputMixObj);
		m_outputMixObj = NULL;
	}

	if (m_engineObj != NULL)
	{
		(*m_engineObj)->Destroy(m_engineObj);
		m_engineObj = NULL;
		m_engine = NULL;
	}
}

std::unique_ptr<AudioStream> SLESAudioDevice::CreateAudioStream(
	AudioSource* audioSource)
{
	size_t numBuffers;
	size_t bufferSize;
	CalculateStreamBufferNumAndSize(
		audioSource->GetAudioSamplingRate(),
		audioSource->GetAudioSampleFormat(),
		audioSource->NumChannels(),
		numBuffers, bufferSize);

	SLuint32 slSamplingRate = 0;
	SLuint32 slSampleFormat = 0;
	SLuint32 slSpeakers = audioSource->NumChannels() > 1 ?
		SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT
		: SL_SPEAKER_FRONT_CENTER;

	switch (audioSource->GetAudioSamplingRate())
	{
	case AudioSamplingRate8000:
		slSamplingRate = SL_SAMPLINGRATE_8;
		break;

	case AudioSamplingRate44100:
		slSamplingRate = SL_SAMPLINGRATE_44_1;
		break;
	}

	switch (audioSource->GetAudioSampleFormat())
	{
	case AudioSampleFormatFixed8:
		slSampleFormat = SL_PCMSAMPLEFORMAT_FIXED_8;
		break;

	case AudioSampleFormatFixed16:
		slSampleFormat = SL_PCMSAMPLEFORMAT_FIXED_16;
		break;
	}

	SLresult result;

	SLDataLocator_BufferQueue bufferQueue =
		{ SL_DATALOCATOR_BUFFERQUEUE, numBuffers };
	SLDataFormat_PCM formatPCM =
	{
		SL_DATAFORMAT_PCM, audioSource->NumChannels(), slSamplingRate,
		slSampleFormat, slSampleFormat,
		slSpeakers, SL_BYTEORDER_LITTLEENDIAN
	};
	SLDataSource dataSource = { &bufferQueue, &formatPCM };

	SLDataLocator_OutputMix outputMix =
		{ SL_DATALOCATOR_OUTPUTMIX, m_outputMixObj };
	SLDataSink dataSink = { &outputMix, NULL };

	SLObjectItf playerObj;

	const SLInterfaceID interfaceIds[1] = { SL_IID_BUFFERQUEUE };
	const SLboolean interfaceRequirements[1] = { SL_BOOLEAN_TRUE };
	result = (*m_engine)->CreateAudioPlayer(
		m_engine, &playerObj, &dataSource, &dataSink, 1,
		interfaceIds, interfaceRequirements);
	assert(SL_RESULT_SUCCESS == result);

	(void)result;

	return std::unique_ptr<AudioStream>(
		new AudioStream(
			new SLESAudioStreamClient(numBuffers, bufferSize, playerObj),
			audioSource, numBuffers, bufferSize));
}
