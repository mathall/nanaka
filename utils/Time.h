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

#ifndef NANAKA_UTILS_TIME_H
#define NANAKA_UTILS_TIME_H

#include <chrono>

/**
 * Time follows the global clock and is used to divide passed time into slices
 * appropriate for updating the game world. It uses the concepts of real time to
 * refer to time spent on the global clock and game time to refer to time which
 * may be scaled according to the speed of the game. All time is in
 * milliseconds.
 */
class Time final
{
public:

	Time(unsigned int timeStep, unsigned int maxTimeStepBuildup);

	// Follow the global clock, accumulated time to be stepped through.
	void Tick();

	// Step through accumulated time. Returns false if it can't step further.
	bool Step();

	// Time passed since creation.
	long unsigned int GetGameTime() const;
	long unsigned int GetRealTime() const;

	// Time passed during the last tick.
	float GetGameTick() const;
	float GetRealTick() const;

	// Time passed during the last step.
	float GetGameStep() const;
	float GetRealStep() const;

	void SetGameTimeMultiplier(float multiplier);

private:

	std::chrono::microseconds m_maxRealTimeBuildup;
	std::chrono::microseconds m_realTimeBuildup;
	std::chrono::microseconds m_lastTick;

	std::chrono::microseconds m_gameTime;
	std::chrono::microseconds m_realTime;
	std::chrono::microseconds m_gameStepDuration;
	std::chrono::microseconds m_realStepDuration;
	std::chrono::microseconds m_gameTickDuration;
	std::chrono::microseconds m_realTickDuration;

	float m_gameTimeMultiplier;
};

inline long unsigned int Time::GetGameTime() const
{
	return m_gameTime.count() * 0.001f;
}

inline long unsigned int Time::GetRealTime() const
{
	return m_realTime.count() * 0.001f;
}

inline float Time::GetGameTick() const
{
	return m_gameTickDuration.count() * 0.001f;
}

inline float Time::GetRealTick() const
{
	return m_realTickDuration.count() * 0.001f;
}

inline float Time::GetGameStep() const
{
	return m_gameStepDuration.count() * 0.001f;
}

inline float Time::GetRealStep() const
{
	return m_realStepDuration.count() * 0.001f;
}

inline void Time::SetGameTimeMultiplier(float multiplier)
{
	m_gameTimeMultiplier = multiplier;
}

#endif // NANAKA_UTILS_TIME_H
