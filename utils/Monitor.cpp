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

#include "utils/Monitor.h"

bool Monitor::EnterCriticalSection()
{
#if !defined(SINGLE_THREADED)
	std::unique_lock<std::mutex> lock(m_mutex);

	const auto thisThreadId = std::this_thread::get_id();
	if (m_criticalRunner && m_criticalRunnerThreadId == thisThreadId)
	{
		return true;
	}

	const auto myTicket = m_tailTicket;
	m_tailTicket = NextTicket(m_tailTicket);
	m_queue[myTicket].wait(lock, [&]{ return myTicket == m_headTicket; });

	m_criticalRunnerThreadId = thisThreadId;
	m_criticalRunner = true;
#endif // !defined(SINGLE_THREADED)
	return false;
}

void Monitor::ExitCriticalSection()
{
#if !defined(SINGLE_THREADED)
	std::unique_lock<std::mutex> lock(m_mutex);
	m_headTicket = NextTicket(m_headTicket);
	m_queue[m_headTicket].notify_one();

	m_criticalRunner = false;
#endif // !defined(SINGLE_THREADED)
}

void Monitor::Yield()
{
#if !defined(SINGLE_THREADED)
	auto someoneWaiting = [&]()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		return NextTicket(m_headTicket) != m_tailTicket;
	};

	if (someoneWaiting())
	{
		ExitCriticalSection();
		EnterCriticalSection();
	}
#endif // !defined(SINGLE_THREADED)
}

void Monitor::WaitFor(Sem& sem)
{
#if !defined(SINGLE_THREADED)
	if (sem.TryWait() == -1)
	{
		ExitCriticalSection();
		sem.Wait();
		EnterCriticalSection();
	}
#endif // !defined(SINGLE_THREADED)
}
