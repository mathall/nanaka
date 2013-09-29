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

#include "sound/Sound.h"

#include <cassert>

#include "resource/ResourceManager.h"
#include "sound/OggDecoder.h"

Sound::Sound(std::string filePath)
	: m_soundAsset(g_resourceManager->Acquire<SoundResource>(filePath))
{
	Constructor();
}

Sound::Sound(Asset<SoundResource> soundAsset)
	: m_soundAsset(soundAsset)
{
	Constructor();
}

void Sound::Constructor()
{
	if (m_soundAsset.IsValid())
	{
		std::shared_ptr<SoundDecoder> decoder;

		switch (m_soundAsset.GetResource()->GetFileFormat())
		{
		case SoundFileFormatOGG:
			decoder = std::make_shared<OggDecoder>(
				m_soundAsset.GetResource()->GetData(),
				m_soundAsset.GetResource()->GetDataSize());
			break;

		case SoundFileFormatUnknown:
			assert(!"Tried loading unsupported sound file.");
			break;
		}

		if (decoder->IsValid())
		{
			m_audioStreamHandle =
				g_audioEngine->CreateAudioStream(decoder.get());

			m_decoder = decoder;
		}
	}
}
