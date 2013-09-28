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

#ifndef NANAKA_SOUND_SOUND_H
#define NANAKA_SOUND_SOUND_H

#include <memory>
#include <string>

#include "audio/AudioEngine.h"
#include "resource/Asset.h"
#include "sound/SoundResource.h"

class SoundDecoder;

class Sound final
{
public:

	Sound() = default;
	explicit Sound(std::string filePath);

	Sound Clone() const;

	void PlayOnce() const;
	void Repeat() const;
	void Stop() const;
	void Pause() const;
	void Unpause() const;

private:

	explicit Sound(Asset<SoundResource> soundAsset);
	void Constructor();

	Asset<SoundResource> m_soundAsset;
	std::shared_ptr<SoundDecoder> m_decoder;
	AudioStreamHandle m_audioStreamHandle;
};

inline Sound Sound::Clone() const
{
	return Sound(m_soundAsset);
}

inline void Sound::PlayOnce() const
{
	g_audioEngine->PlayOnce(m_audioStreamHandle);
}

inline void Sound::Repeat() const
{
	g_audioEngine->Repeat(m_audioStreamHandle);
}

inline void Sound::Stop() const
{
	g_audioEngine->Stop(m_audioStreamHandle);
}

inline void Sound::Pause() const
{
	g_audioEngine->Pause(m_audioStreamHandle);
}

inline void Sound::Unpause() const
{
	g_audioEngine->Unpause(m_audioStreamHandle);
}

#endif // NANAKA_SOUND_SOUND_H
