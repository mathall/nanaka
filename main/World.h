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

#ifndef NANAKA_MAIN_WORLD_H
#define NANAKA_MAIN_WORLD_H

#include <memory>

#include "entity/EntityTree.h"
#include "game/GameEMMgr.h"
#include "physics/PhysicsEMMgr.h"
#include "scene/SceneEMMgr.h"
#include "utils/Random.h"
#include "utils/Time.h"

class GameEMMgr;
class PhysicsEMMgr;

/**
 * World ties together an EntityTree with the various module managers and
 * support classes and proves functionality to deal with these.
 */
class World
{
public:

	World();

	void Tick();
	void Draw(const Camera& camera) const;

	EntityTree& GetEntityTree();

	SceneEMMgr& GetSceneMgr();
	PhysicsEMMgr& GetPhysicsMgr();
	GameEMMgr& GetGameObjectMgr();

	Time& GetTime();

private:

	EntityTree m_entityTree;

	Time m_time;
};

inline void World::Draw(const Camera& camera) const
{
	m_entityTree.GetModuleManager<SceneEMMgr>()->DrawScene(camera);
}

inline EntityTree& World::GetEntityTree()
{
	return m_entityTree;
}

inline SceneEMMgr& World::GetSceneMgr()
{
	return *m_entityTree.GetModuleManager<SceneEMMgr>();
}

inline PhysicsEMMgr& World::GetPhysicsMgr()
{
	return *m_entityTree.GetModuleManager<PhysicsEMMgr>();
}

inline GameEMMgr& World::GetGameObjectMgr()
{
	return *m_entityTree.GetModuleManager<GameEMMgr>();
}

inline Time& World::GetTime()
{
	return m_time;
}

#endif // NANAKA_MAIN_WORLD_H
