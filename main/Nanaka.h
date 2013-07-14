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

#ifndef NANAKA_MAIN_NANAKA_H
#define NANAKA_MAIN_NANAKA_H

#include <memory>

#include "entity/EntitySystem.h"
#include "gui/GUI.h"
#include "input/InputHandler.h"
#include "main/IGame.h"
#include "main/INanaka.h"
#include "pi/Application.h"
#include "utils/Monitor.h"
#include "utils/Thread.h"

class Application;

/**
 * Nanaka is where all the subsystems are tied together. The application hands
 * over execution of Nanaka and the game and is then expected to inform of
 * platform events.
 */
class Nanaka final : public Thread, public Monitor, public INanaka
{
public:

	explicit Nanaka(Application& application);

	void OnResume();
	void OnPause();

	void AddInputEvent(InputEvent event);
	void SetDisplayProperties(DisplayProperties displayProps);

	/**
	 * INanaka implementation (Application shouldn't use these)
	 */
	void Shutdown() const override;
	GUI& GetGUI() override;
	EntitySystem& GetEntitySystem() override;

protected:

	/**
	 * Thread implementation
	 */
	void RunThread() override;
	void OnKillThread() override;

private:

	void Exit();

	Application& m_application;
	std::unique_ptr<IGame> m_game;

	GUI m_GUI;
	InputHandler m_inputHandler;
	EntitySystem m_entitySystem;

	bool m_running;
	Sem m_runPermit;
};

inline GUI& Nanaka::GetGUI()
{
	return m_GUI;
}

inline EntitySystem& Nanaka::GetEntitySystem()
{
	return m_entitySystem;
}

inline void Nanaka::Shutdown() const
{
	m_application.Shutdown();
}

#endif // NANAKA_MAIN_NANAKA_H
