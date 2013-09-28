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

#include "main/Nanaka.h"

#include "audio/AudioEngine.h"
#include "game/GameEM.h"
#include "graphics/BillboardAnimationLoader.h"
#include "graphics/MaterialLoader.h"
#include "graphics/ModelLoader.h"
#include "graphics/ShaderLoader.h"
#include "graphics/ShaderProgramLoader.h"
#include "graphics/TextureLoader.h"
#include "gui/FontLoader.h"
#include "physics/PhysicsEM.h"
#include "renderer/Renderer.h"
#include "resource/ResourceManager.h"
#include "scene/SceneEM.h"
#include "utils/GlobalClock.h"
#include "utils/UUID.h"

Nanaka::Nanaka(Application& application)
	: m_application(application)
	, m_game(CreateGame())
	, m_runPermit(0)
{
	UUID::Initialize();

	GlobalClock::Instantiate();

	ResourceManager::Instantiate();
	g_resourceManager->RegisterLoader<TextureLoader>();
	g_resourceManager->RegisterLoader<ModelLoader>();
	g_resourceManager->RegisterLoader<BillboardAnimationLoader>();
	g_resourceManager->RegisterLoader<FontLoader>();
	g_resourceManager->RegisterLoader<MaterialLoader>();
	g_resourceManager->RegisterLoader<ShaderLoader>();
	g_resourceManager->RegisterLoader<ShaderProgramLoader>();

	Renderer::Instantiate();
	g_renderer->StartThread();

	AudioEngine::Instantiate();

	m_GUI.Initialize();
	m_inputHandler.AddInputListener(&m_GUI);

	m_entitySystem.RegisterEntityModuleFactory<GameEM>();
	m_entitySystem.RegisterEntityModuleFactory<SceneEM>();
	m_entitySystem.RegisterEntityModuleFactory<PhysicsEM>();
}

void Nanaka::RunThread()
{
	ScopedMonitorLock lock(this);

	m_game->Initialize(this);

	m_running = true;
	while (m_running)
	{
		Yield();
		WaitFor(m_runPermit);

		if (!m_running)
		{
			break;
		}

		g_clock->Tick();
		g_clock->UpdateFPS();

		m_inputHandler.ProcessEvents();
		m_game->GameLoop();
		g_audioEngine->Update();
		m_GUI.Draw();

		Signal(m_runPermit);
	}

	Exit();
}

void Nanaka::OnResume()
{
	ScopedMonitorLock lock(this);
	Signal(m_runPermit);
	m_game->OnResume();
}

void Nanaka::OnPause()
{
	ScopedMonitorLock lock(this);
	WaitFor(m_runPermit);
	m_game->OnPause();
}

void Nanaka::AddInputEvent(InputEvent event)
{
	ScopedMonitorLock lock(this);
	m_inputHandler.AddInputEvent(event);
}

void Nanaka::SetDisplayProperties(DisplayProperties displayProps)
{
	ScopedMonitorLock lock(this);
	m_GUI.SetDisplayProperties(displayProps);
}

void Nanaka::OnKillThread()
{
	ScopedMonitorLock lock(this);
	m_running = false;
	Signal(m_runPermit);
}

void Nanaka::Exit()
{
	m_game->OnExit();
	GlobalClock::DestroyInstance();
	ResourceManager::DestroyInstance();
	g_renderer->KillThread();
	Renderer::DestroyInstance();
	AudioEngine::DestroyInstance();
}
