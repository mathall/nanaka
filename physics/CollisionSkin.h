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

#ifndef NANAKA_PHYSICS_COLLISIONSKIN_H
#define NANAKA_PHYSICS_COLLISIONSKIN_H

#include <memory>
#include <vector>

#include "math/Quat.h"
#include "math/Vec3f.h"
#include "physics/CollisionShape.h"

class Entity;
class Collision;
class CollisionListener;

/**
 * CollisionSkin is built up of several CollisionShapes. Two CollisionSkins are
 * involved in a Collision when one or more CollisionShapes from each
 * CollisionSkin have an Impact with each other.
 */
class CollisionSkin final
{
public:

	explicit CollisionSkin(Entity& entity);

	Entity& GetEntity() const;

	void AddShape(std::unique_ptr<CollisionShape> shape);
	const std::vector<std::unique_ptr<CollisionShape>>& GetShapes() const;

	void SetImmovable(bool immovable);
	bool IsImmovable() const;

	void SetCollisionListener(CollisionListener* listener);
	void DoCollisionCallback(const Collision& collision) const;

	void PlacementUpdated(Vec3f position, Quat rotation);

private:

	Entity& m_entity;

	std::vector<std::unique_ptr<CollisionShape>> m_shapes;
	bool m_immovable;

	CollisionListener* m_collisionListener;
};

inline CollisionSkin::CollisionSkin(Entity& entity)
	: m_entity(entity)
	, m_collisionListener(NULL)
{
}

inline void CollisionSkin::AddShape(std::unique_ptr<CollisionShape> shape)
{
	m_shapes.push_back(std::move(shape));
}

inline const std::vector<std::unique_ptr<CollisionShape>>&
CollisionSkin::GetShapes() const
{
	return m_shapes;
}

inline Entity& CollisionSkin::GetEntity() const
{
	return m_entity;
}

inline void CollisionSkin::SetCollisionListener(CollisionListener* listener)
{
	m_collisionListener = listener;
}

inline void CollisionSkin::SetImmovable(bool immovable)
{
	m_immovable = immovable;
}

inline bool CollisionSkin::IsImmovable() const
{
	return m_immovable;
}

#endif // NANAKA_PHYSICS_COLLISIONSKIN_H
