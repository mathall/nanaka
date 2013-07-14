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

#ifndef NANAKA_SCENE_SCENEEMMGR_H
#define NANAKA_SCENE_SCENEEMMGR_H

#include <cassert>
#include <unordered_map>
#include <memory>

#include "entity/Entity.h"
#include "entity/EntityModule.h"
#include "entity/EntityModuleManager.h"
#include "scene/SceneEM.h"
#include "utils/UUID.h"

class Camera;

class SceneEMMgr final : public EntityModuleManager
{
public:

	static EntityModuleType GetModuleType();

	SceneEMMgr();

	void DrawScene(const Camera& camera) const;
	void Animate(const Time& time) const;

	/**
	 * EntityModuleManager implementation.
	 */
	void ManageModule(EntityModule* module) override;
	void UnmanageModule(EntityModule& module) override;

private:

	std::unordered_map<UUID, SceneEM*> m_modules;
};

inline EntityModuleType SceneEMMgr::GetModuleType()
{
	return SceneEM::GetModuleType();
}

inline SceneEMMgr::SceneEMMgr()
	: EntityModuleManager(GetModuleType())
{
}

inline void SceneEMMgr::ManageModule(EntityModule* module)
{
	assert(m_modules.find(module->GetEntity().GetId()) == m_modules.end());
	assert(module->GetType() == GetModuleType());

	m_modules.insert(std::make_pair(module->GetEntity().GetId(),
		static_cast<SceneEM*>(module)));
}

inline void SceneEMMgr::UnmanageModule(EntityModule& module)
{
	assert(m_modules.find(module.GetEntity().GetId()) != m_modules.end());

	m_modules.erase(m_modules.find(module.GetEntity().GetId()));
}

#endif // NANAKA_SCENE_SCENEEMMGR_H
