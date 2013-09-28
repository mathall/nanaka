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

#ifndef NANAKA_AUDIO_AUDIOENGINE_H
#define NANAKA_AUDIO_AUDIOENGINE_H

#define g_audioEngine AudioEngine::GetInstance()

#include <memory>
#include <unordered_map>
#include <vector>

#include "audio/AudioDevice.h"
#include "audio/AudioStream.h"
#include "audio/AudioStreamHandle.h"
#include "utils/YOLO.h"

class AudioSource;

/**
 * The AudioEngine manages the AudioDevice and all the individual AudioStreams.
 * Depending on the AudioDevice, the AudioStreams may be informed asynchronously
 * when their buffers has been processed. Thus the AudioEngine must take special
 * care of AudioStreams which may still be in use after asked to be destroyed.
 */
class AudioEngine final : public YOLO<AudioEngine>
{
public:

	AudioEngine();

	/**
	 * YOLO implementation.
	 */
	void OnDestroy() override;

	AudioStreamHandle CreateAudioStream(AudioSource* audioSource);
	void PlayOnce(AudioStreamHandle streamHandle);
	void Repeat(AudioStreamHandle streamHandle);
	void Stop(AudioStreamHandle streamHandle);
	void Pause(AudioStreamHandle streamHandle);
	void Unpause(AudioStreamHandle streamHandle);

	void Update();

private:

	// InternalAudioStreamHandle is the only one allowed to call
	// DestroyAudioStream().
	friend class InternalAudioStreamHandle;

	void DestroyAudioStream(UUID streamId);

	std::unique_ptr<AudioDevice> m_device;

	std::unordered_map<UUID, std::unique_ptr<AudioStream>> m_streams;
	std::vector<std::unique_ptr<AudioStream>> m_abandonedStreams;
};

#endif // NANAKA_AUDIO_AUDIOENGINE_H
