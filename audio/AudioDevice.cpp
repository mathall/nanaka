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

#include "audio/AudioDevice.h"

void AudioDevice::CalculateStreamBufferNumAndSize(
	AudioSamplingRate samplingRate,
	AudioSampleFormat sampleFormat,
	size_t numChannels,
	size_t& numBuffers,
	size_t& bufferSize)
{
	const size_t samplesPerSecondPerChannel = samplingRate;
	const size_t samplesPerSecond = samplesPerSecondPerChannel * numChannels;
	const size_t bytesPerSample = sampleFormat;
	const size_t bytesPerSecond = samplesPerSecond * bytesPerSample;

	const size_t minimumFPS = 20;
	const float totBufferLengthInSeconds = 1.0f / minimumFPS;
	const size_t totBufferSize = totBufferLengthInSeconds * bytesPerSecond;
	const float minBufferLengthInSeconds = 0.02f;
	const size_t minBufferSize = minBufferLengthInSeconds * bytesPerSecond;

	bufferSize = 2;
	while (bufferSize < minBufferSize)
	{
		bufferSize *= 2;
	}
	numBuffers = totBufferSize / bufferSize + 1;
	numBuffers = numBuffers < 2 ? 2 : numBuffers;
}
