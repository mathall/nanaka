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

#ifndef NANAKA_UTILS_MONITOR_H
#define NANAKA_UTILS_MONITOR_H

#include <array>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "utils/Sem.h"

class Monitor
{
public:

	Monitor();
	virtual ~Monitor(){}

protected:

	void Yield();
	void WaitFor(Sem& sem);
	void Signal(Sem& sem) const;

	// ScopedMonitorLock enters and exits critical section.
	friend class ScopedMonitorLock;

	// Return true if critical section was adopted. (thread re-entry)
	bool EnterCriticalSection();
	void ExitCriticalSection();

private:

	int NextTicket(int ticket) const;

	std::mutex m_mutex;

	static const int s_maxTickets = 5;
	std::array<std::condition_variable, s_maxTickets> m_queue;
	int m_headTicket;
	int m_tailTicket;

	std::thread::id m_criticalRunnerThreadId;
	bool m_criticalRunner;
};

inline Monitor::Monitor()
	: m_headTicket(0)
	, m_tailTicket(0)
	, m_criticalRunner(false)
{
}

inline void Monitor::Signal(Sem& sem) const
{
	sem.Post();
}

inline int Monitor::NextTicket(int ticket) const
{
	return (ticket + 1) % s_maxTickets;
}

class ScopedMonitorLock final
{
public:

	explicit ScopedMonitorLock(Monitor* monitor);
	~ScopedMonitorLock();

private:

	Monitor* m_monitor;
	bool m_didAdoptCriticalSection;
};

inline ScopedMonitorLock::ScopedMonitorLock(Monitor* monitor)
	: m_monitor(monitor)
	, m_didAdoptCriticalSection(m_monitor->EnterCriticalSection())
{
}

inline ScopedMonitorLock::~ScopedMonitorLock()
{
	if (!m_didAdoptCriticalSection)
	{
		m_monitor->ExitCriticalSection();
	}
}

#endif // NANAKA_UTILS_MONITOR_H
