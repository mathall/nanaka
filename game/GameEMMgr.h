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

#ifndef NANAKA_GAME_GAMEEMMGR_H
#define NANAKA_GAME_GAMEEMMGR_H

#include <cassert>
#include <unordered_map>

#include "entity/Entity.h"
#include "entity/EntityModuleManager.h"
#include "game/GameEM.h"
#include "utils/Time.h"
#include "utils/UUID.h"

class GameEMMgr final : public EntityModuleManager
{
public:

	static EntityModuleType GetModuleType();

	GameEMMgr();

	void Update(Time& time) const;

	/**
	 * EntityModuleManager implementation.
	 */
	void ManageModule(EntityModule* module) override;
	void UnmanageModule(EntityModule& module) override;

private:

	std::unordered_map<UUID, GameEM*> m_modules;
};

inline EntityModuleType GameEMMgr::GetModuleType()
{
	return GameEM::GetModuleType();
}

inline GameEMMgr::GameEMMgr()
	: EntityModuleManager(GetModuleType())
{
}

inline void GameEMMgr::ManageModule(EntityModule* module)
{
	assert(m_modules.find(module->GetEntity().GetId()) == m_modules.end());
	assert(module->GetType() == GetModuleType());

	m_modules.insert(std::make_pair(module->GetEntity().GetId(),
		static_cast<GameEM*>(module)));
}

inline void GameEMMgr::UnmanageModule(EntityModule& module)
{
	assert(m_modules.find(module.GetEntity().GetId()) != m_modules.end());

	m_modules.erase(m_modules.find(module.GetEntity().GetId()));
}

#endif // NANAKA_GAME_GAMEEMMGR_H
