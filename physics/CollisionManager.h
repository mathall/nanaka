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

#ifndef NANAKA_PHYSICS_COLLISIONMANAGER_H
#define NANAKA_PHYSICS_COLLISIONMANAGER_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "physics/Collision.h"
#include "physics/Impact.h"
#include "utils/ObjectPool.h"
#include "utils/UUID.h"

class CollisionSkin;

/**
 * CollisionManager checks for collisions between CollisionSkins and handles
 * resolves/responds.
 */
class CollisionManager final
{
public:

	void SetHandleResolve(UUID colliderType, UUID collideeType, bool resolve);
	void SetHandleResponse(UUID colliderType, UUID collideeType, bool respond);

	bool ShouldHandleResolve(UUID colliderType, UUID collideeType);
	bool ShouldHandleResponse(UUID colliderType, UUID collideeType);

	void AddCollisionSkin(const CollisionSkin* skin);

	void CheckForCollisions();
	void CheckForCollision(
		const CollisionSkin* collider,
		const CollisionSkin* collidee);

	void RespondToCollisions();
	void ResolveCollisions();

	void ClearCollisions();

private:

	Vec3f CheckShapeShape(
		const std::unique_ptr<CollisionShape>& shape1,
		const std::unique_ptr<CollisionShape>& shape2) const;

	Vec3f CheckBoxBox(
		const std::unique_ptr<CollisionShape>& shape1,
		const std::unique_ptr<CollisionShape>& shape2) const;

	Vec3f CheckBoxSphere(
		const std::unique_ptr<CollisionShape>& shape1,
		const std::unique_ptr<CollisionShape>& shape2) const;

	Vec3f CheckSphereSphere(
		const std::unique_ptr<CollisionShape>& shape1,
		const std::unique_ptr<CollisionShape>& shape2) const;

	std::vector<const CollisionSkin*> m_collisionSkins;

	ObjectPool<Collision, Collision::Clear> m_collisionPool;
	std::vector<std::unique_ptr<Collision>> m_collisions;

	std::unordered_map<
		UUID,
		std::unordered_map<UUID, bool>> m_entityTypeResolveLookup;
	std::unordered_map<
		UUID,
		std::unordered_map<UUID, bool>> m_entityTypeResponseLookup;
};

inline void CollisionManager::SetHandleResolve(
	UUID colliderType,
	UUID collideeType,
	bool resolve)
{
	m_entityTypeResolveLookup[colliderType][collideeType] = resolve;
}

inline void CollisionManager::SetHandleResponse(
	UUID colliderType,
	UUID collideeType,
	bool respond)
{
	m_entityTypeResponseLookup[colliderType][collideeType] = respond;
}

inline bool CollisionManager::ShouldHandleResolve(
	UUID colliderType,
	UUID collideeType)
{
	return m_entityTypeResolveLookup[colliderType][collideeType];
}

inline bool CollisionManager::ShouldHandleResponse(
	UUID colliderType,
	UUID collideeType)
{
	return m_entityTypeResponseLookup[colliderType][collideeType];
}

inline void CollisionManager::AddCollisionSkin(const CollisionSkin* skin)
{
	m_collisionSkins.push_back(skin);
}

#endif // NANAKA_PHYSICS_COLLISIONMANAGER_H
