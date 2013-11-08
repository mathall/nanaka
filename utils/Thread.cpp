#include "utils/Thread.h"

void Thread::RunThread()
{
	ScopedMonitorLock lock(this);

	m_running = true;
	ThreadInitialize();
	while (m_running)
	{
		Yield();
		ThreadLoop();
	}
	ThreadFinalize();
}

void Thread::StartThread()
{
#if !defined(SINGLE_THREADED)
	m_thread = std::thread(&Thread::RunThread, this);
#else
	m_running = true;
	ThreadInitialize();
#endif // !defined(SINGLE_THREADED)
}

void Thread::KillThread()
{
	EnterCriticalSection();
	m_running = false;
	OnKillThread();
	ExitCriticalSection();

#if !defined(SINGLE_THREADED)
	m_thread.join();
#else
	ThreadFinalize();
#endif // !defined(SINGLE_THREADED)
}
