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

#ifndef NANAKA_PHYSICS_PHYSICSEMMGR_H
#define NANAKA_PHYSICS_PHYSICSEMMGR_H

#include <cassert>
#include <unordered_map>

#include "entity/Entity.h"
#include "entity/EntityModule.h"
#include "entity/EntityModuleManager.h"
#include "physics/CollisionManager.h"
#include "physics/PhysicsEM.h"
#include "utils/UUID.h"

class PhysicsEMMgr final : public EntityModuleManager
{
public:

	static EntityModuleType GetModuleType();

	PhysicsEMMgr();

	void SetHandleResolve(UUID colliderType, UUID collideeType, bool resolve);
	void SetHandleResponse(UUID colliderType, UUID collideeType, bool respond);

	void Integrate(const Time& time);

	/**
	 * EntityModuleManager implementation.
	 */
	void ManageModule(EntityModule* module) override;
	void UnmanageModule(EntityModule& module) override;

private:

	std::unordered_map<UUID, PhysicsEM*> m_modules;
	CollisionManager m_collisionManager;
};

inline EntityModuleType PhysicsEMMgr::GetModuleType()
{
	return PhysicsEM::GetModuleType();
}

inline PhysicsEMMgr::PhysicsEMMgr()
	: EntityModuleManager(GetModuleType())
	, m_collisionManager()
{
}

inline void PhysicsEMMgr::ManageModule(EntityModule* module)
{
	assert(m_modules.find(module->GetEntity().GetId()) == m_modules.end());
	assert(module->GetType() == PhysicsEM::GetModuleType());

	m_modules.insert(std::make_pair(module->GetEntity().GetId(),
		static_cast<PhysicsEM*>(module)));
}

inline void PhysicsEMMgr::UnmanageModule(EntityModule& module)
{
	assert(m_modules.find(module.GetEntity().GetId()) != m_modules.end());

	m_modules.erase(m_modules.find(module.GetEntity().GetId()));
}

inline void PhysicsEMMgr::SetHandleResolve(
	UUID colliderType,
	UUID collideeType,
	bool resolve)
{
	m_collisionManager.SetHandleResolve(colliderType, collideeType, resolve);
}

inline void PhysicsEMMgr::SetHandleResponse(
	UUID colliderType,
	UUID collideeType,
	bool respond)
{
	m_collisionManager.SetHandleResponse(colliderType, collideeType, respond);
}

#endif // NANAKA_PHYSICS_PHYSICSEMMGR_H
