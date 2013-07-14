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

#ifndef NANAKA_PHYSICS_PHYSICSEM_H
#define NANAKA_PHYSICS_PHYSICSEM_H

#include <memory>

#include "entity/EntityModule.h"
#include "math/Vec3f.h"
#include "physics/CollisionSkin.h"

class Time;
class CollisionListener;

class PhysicsEM final : public EntityModule
{
public:

	static EntityModuleType GetModuleType();

	explicit PhysicsEM(Entity& entity);

	void AddCollisionShape(std::unique_ptr<CollisionShape> shape);
	void SetCollisionListener(CollisionListener* collisionListener);

	void SetImmovable(bool immovable);

	void SetVelocity(Vec3f velocity);
	Vec3f GetVelocity() const;

	void Integrate(const Time& time);
	const CollisionSkin& GetCollisionSkin() const;

private:

	bool m_immovable;
	Vec3f m_velocity;
	CollisionSkin m_collisionSkin;
};

inline EntityModuleType PhysicsEM::GetModuleType()
{
	static const EntityModuleType s_moduleType = PhysicsEntityModuleType;
	return s_moduleType;
}

inline PhysicsEM::PhysicsEM(Entity& entity)
	: EntityModule(GetModuleType(), entity)
	, m_immovable(false)
	, m_velocity(Vec3f::Zero())
	, m_collisionSkin(entity)
{
}

inline Vec3f PhysicsEM::GetVelocity() const
{
	return m_velocity;
}

inline void PhysicsEM::SetVelocity(Vec3f velocity)
{
	m_velocity = velocity;
}

inline void PhysicsEM::SetImmovable(bool immovable)
{
	m_immovable = immovable;
	m_collisionSkin.SetImmovable(m_immovable);
}

inline const CollisionSkin& PhysicsEM::GetCollisionSkin() const
{
	return m_collisionSkin;
}

inline void PhysicsEM::AddCollisionShape(std::unique_ptr<CollisionShape> shape)
{
	m_collisionSkin.AddShape(std::move(shape));
}

inline void PhysicsEM::SetCollisionListener(
	CollisionListener* collisionListener)
{
	m_collisionSkin.SetCollisionListener(collisionListener);
}

#endif // NANAKA_PHYSICS_PHYSICSEM_H
