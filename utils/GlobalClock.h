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

#ifndef NANAKA_UTILS_GLOBALCLOCK_H
#define NANAKA_UTILS_GLOBALCLOCK_H

#define g_clock GlobalClock::GetInstance()

#include <chrono>

#include "utils/YOLO.h"

/**
 * GlobalClock tracks time, in microseconds, spent since startup. By only
 * ticking the clock once per frame, the time point used throughout a frame
 * can be monotonic.
 */
class GlobalClock final : public YOLO<GlobalClock>
{
public:

	GlobalClock();

	void Tick();
	std::chrono::microseconds GetTime() const;

	// Assumes ont tick per frame.
	void UpdateFPS();
	float GetFPS() const;

private:

	std::chrono::time_point<std::chrono::system_clock> m_startupTime;
	std::chrono::microseconds m_time;

	std::chrono::microseconds m_FPSUpdateResolution;
	std::chrono::microseconds m_lastFPSUpdate;
	int m_framesSinceLastFPSUpdate;
	float m_FPS;
};

inline void GlobalClock::Tick()
{
	m_time = std::chrono::duration_cast<std::chrono::microseconds>(
		std::chrono::system_clock::now() - m_startupTime);
}

inline std::chrono::microseconds GlobalClock::GetTime() const
{
	return m_time;
}

inline float GlobalClock::GetFPS() const
{
	return m_FPS;
}

#endif // NANAKA_UTILS_GLOBALCLOCK_H
