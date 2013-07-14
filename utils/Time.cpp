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

#include "utils/Time.h"

#include "utils/GlobalClock.h"

using namespace std::chrono;

Time::Time(unsigned int timeStep, unsigned int maxTimeStepBuildup)
	: m_maxRealTimeBuildup(maxTimeStepBuildup * 1000)
	, m_realTimeBuildup(0)
	, m_lastTick(0)
	, m_gameTime(0)
	, m_realTime(0)
	, m_gameStepDuration(0)
	, m_realStepDuration(timeStep * 1000)
	, m_gameTickDuration(0)
	, m_realTickDuration(0)
	, m_gameTimeMultiplier(1.0f)
{
}

void Time::Tick()
{
	const auto timeNow = g_clock->GetTime();
	m_realTimeBuildup += duration_cast<microseconds>(timeNow - m_lastTick);
	m_lastTick = timeNow;

	if (m_realTimeBuildup > m_maxRealTimeBuildup)
	{
		m_realTimeBuildup = m_realStepDuration;
	}

	m_gameTickDuration = microseconds(0);
	m_realTickDuration = microseconds(0);
}

bool Time::Step()
{
	if (m_realTimeBuildup < m_realStepDuration)
	{
		return false;
	}

	m_realTimeBuildup -= m_realStepDuration;

	m_gameStepDuration = duration_cast<microseconds>(
		m_gameTimeMultiplier * m_realStepDuration);

	m_gameTickDuration += m_gameStepDuration;
	m_realTickDuration += m_realStepDuration;

	m_gameTime += m_gameStepDuration;
	m_realTime += m_realStepDuration;

	return true;
}
