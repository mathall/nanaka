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

#include "utils/GlobalClock.h"

using namespace std::chrono;

GlobalClock::GlobalClock()
	: m_startupTime(system_clock::now())
	, m_time(0)
	, m_FPSUpdateResolution(1000 * 1000)
	, m_lastFPSUpdate(0)
	, m_framesSinceLastFPSUpdate(0)
	, m_FPS(0)
{
}

void GlobalClock::UpdateFPS()
{
	m_framesSinceLastFPSUpdate += 1;
	const auto timeNow = GetTime();
	if (timeNow > (m_lastFPSUpdate + m_FPSUpdateResolution))
	{
		const float secondsSinceLastFPSUpdate = duration_cast<microseconds>(
			timeNow - m_lastFPSUpdate).count() * 0.001f * 0.001f;
		m_FPS = m_framesSinceLastFPSUpdate / secondsSinceLastFPSUpdate;
		m_framesSinceLastFPSUpdate = 0;
		m_lastFPSUpdate = timeNow;
	}
}
