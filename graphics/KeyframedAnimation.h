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

#ifndef NANAKA_GRAPHICS_KEYFRAMEDANIMATION_H
#define NANAKA_GRAPHICS_KEYFRAMEDANIMATION_H

#include <cassert>
#include <vector>

#include "graphics/Interpolation.h"

template<typename T>
class KeyframedAnimation final
{
public:

	void Animate(long unsigned int ms);

	void ResetAnimation();

	void AddKeyframe(
		long unsigned int ms,
		T value,
		Interpolation<T> interpolation);

	T GetValue() const;

private:

	int NextFrame() const;

	long unsigned int m_currentFrame;
	long unsigned int m_msSpentOnFrame;
	float m_percentOfFrame;

	std::vector<long unsigned int> m_keyframeDurations;
	std::vector<T> m_values;
	std::vector<Interpolation<T>> m_interpolations;
};

template<typename T>
inline void KeyframedAnimation<T>::Animate(long unsigned int ms)
{
	m_msSpentOnFrame += ms;

	while (m_msSpentOnFrame > m_keyframeDurations.at(m_currentFrame))
	{
		m_msSpentOnFrame -= m_keyframeDurations.at(m_currentFrame);
		m_currentFrame = NextFrame();
	}

	m_percentOfFrame = m_msSpentOnFrame /
		static_cast<float>(m_keyframeDurations.at(m_currentFrame));
}

template<typename T>
inline void KeyframedAnimation<T>::ResetAnimation()
{
	m_currentFrame = 0;
	m_msSpentOnFrame = 0;
	m_percentOfFrame = 0.0f;
}

template<typename T>
inline void KeyframedAnimation<T>::AddKeyframe(
	long unsigned int ms,
	T value,
	Interpolation<T> interpolation)
{
	assert(ms != 0);
	m_keyframeDurations.push_back(ms);
	m_values.push_back(value);
	m_interpolations.push_back(interpolation);
}

template<typename T>
inline T KeyframedAnimation<T>::GetValue() const
{
	return m_interpolations[m_currentFrame].Eval(
		std::max(std::min(m_percentOfFrame, 1.0f), 0.0f),
		m_values[m_currentFrame],
		m_values[NextFrame()]);
}

template<typename T>
inline int KeyframedAnimation<T>::NextFrame() const
{
	return (m_currentFrame + 1) % m_keyframeDurations.size();
}

#endif // NANAKA_GRAPHICS_KEYFRAMEDANIMATION_H
