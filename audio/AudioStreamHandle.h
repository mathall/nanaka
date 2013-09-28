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

#ifndef NANAKA_AUDIO_AUDIOSTREAMHANDLE_H
#define NANAKA_AUDIO_AUDIOSTREAMHANDLE_H

#include <memory>

#include "utils/UUID.h"

class InternalAudioStreamHandle final
{
public:

	explicit InternalAudioStreamHandle(UUID id);
	~InternalAudioStreamHandle();

	InternalAudioStreamHandle(InternalAudioStreamHandle&) = delete;
	InternalAudioStreamHandle& operator=(
		const InternalAudioStreamHandle&) = delete;

	UUID GetId() const;

private:

	UUID m_id;
};

inline InternalAudioStreamHandle::InternalAudioStreamHandle(UUID id)
	: m_id(id)
{
}

inline UUID InternalAudioStreamHandle::GetId() const
{
	return m_id;
}

class AudioStreamHandle final
{
public:

	AudioStreamHandle() = default;

	bool IsValid() const;
	UUID GetId() const;

private:

	// The AudioEngine is the only one allowed to create valid handles.
	friend class AudioEngine;

	explicit AudioStreamHandle(UUID id);

	std::shared_ptr<InternalAudioStreamHandle> m_internalHandle;
};

inline AudioStreamHandle::AudioStreamHandle(UUID id)
	: m_internalHandle(std::make_shared<InternalAudioStreamHandle>(id))
{
}

inline bool AudioStreamHandle::IsValid() const
{
	return m_internalHandle != nullptr;
}

inline UUID AudioStreamHandle::GetId() const
{
	return m_internalHandle->GetId();
}

#endif // NANAKA_AUDIO_AUDIOSTREAMHANDLE_H
