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

#include "physics/CollisionManager.h"
#include "physics/CollisionSkin.h"
#include "physics/CollisionBox.h"
#include "physics/CollisionSphere.h"
#include "entity/Entity.h"

void CollisionManager::CheckForCollisions()
{
	for (const auto collider : m_collisionSkins)
	{
		for(const auto collidee : m_collisionSkins)
		{
			auto colliderId = collider->GetEntity().GetEntityTypeID();
			auto collideeId = collidee->GetEntity().GetEntityTypeID();
			if (ShouldHandleResponse(colliderId, collideeId)
				|| (ShouldHandleResolve(colliderId, collideeId)
					&& !collider->IsImmovable()))
			{
				CheckForCollision(collider, collidee);
			}
		}
	}

	m_collisionSkins.clear();
}

void CollisionManager::CheckForCollision(
	const CollisionSkin* collider,
	const CollisionSkin* collidee)
{
	auto collision = m_collisionPool.GetObject();

	for (const auto& colliderShape : collider->GetShapes())
	{
		for (const auto& collideeShape : collidee->GetShapes())
		{
			Vec3f penetration = CheckShapeShape(colliderShape, collideeShape);

			if (!penetration.AnyEquals(0.0f))
			{
				collision->AddImpact(Impact(
					colliderShape.get(), collideeShape.get(), penetration));
			}
		}
	}

	if (collision->GetImpacts().size() > 0)
	{
		collision->Instantiate(collider, collidee);
		m_collisions.push_back(std::move(collision));
	}
	else
	{
		m_collisionPool.PutObject(std::move(collision));
	}
}

void CollisionManager::RespondToCollisions()
{
	for (const auto& collision : m_collisions)
	{
		if (ShouldHandleResponse(
			collision->GetCollider().GetEntity().GetEntityTypeID(),
			collision->GetCollidee().GetEntity().GetEntityTypeID()))
		{
			collision->Respond();
		}
	}
}

void CollisionManager::ResolveCollisions()
{
	// Not implemented yet.
}

void CollisionManager::ClearCollisions()
{
	for (auto& collision : m_collisions)
	{
		m_collisionPool.PutObject(std::move(collision));
	}

	m_collisions.clear();
}

Vec3f CollisionManager::CheckShapeShape(
	const std::unique_ptr<CollisionShape>& shape1,
	const std::unique_ptr<CollisionShape>& shape2) const
{
	Vec3f penetration = Vec3f::Zero();

	switch (shape1->GetType())
	{
	case BoxCollisionShapeType:
		switch (shape2->GetType())
		{
		case BoxCollisionShapeType:
			penetration = CheckBoxBox(shape1, shape2);
			break;

		case SphereCollisionShapeType:
			penetration = CheckBoxSphere(shape1, shape2);
			break;
		}
		break;

	case SphereCollisionShapeType:
		switch (shape2->GetType())
		{
		case BoxCollisionShapeType:
			penetration = -CheckBoxSphere(shape2, shape1);
			break;

		case SphereCollisionShapeType:
			penetration = CheckSphereSphere(shape1, shape2);
			break;
		}
		break;
	}

	return penetration;
}

Vec3f CollisionManager::CheckBoxBox(
	const std::unique_ptr<CollisionShape>& shape1,
	const std::unique_ptr<CollisionShape>& shape2) const
{
	CollisionBox& box1 = *static_cast<CollisionBox*>(shape1.get());
	CollisionBox& box2 = *static_cast<CollisionBox*>(shape2.get());

	Vec3f pos1m2 = box1.GetPosition() - box2.GetPosition();
	Vec3f dim1p2 = box1.GetDimensions() + box2.GetDimensions();

	return Vec3f(
		(pos1m2.x > 0 ? 1 : -1) * std::max(dim1p2.x - std::abs(pos1m2.x), 0.0f),
		(pos1m2.y > 0 ? 1 : -1) * std::max(dim1p2.y - std::abs(pos1m2.y), 0.0f),
		(pos1m2.z > 0 ? 1 : -1) * std::max(dim1p2.z - std::abs(pos1m2.z), 0.0f)
	);
}

Vec3f CollisionManager::CheckBoxSphere(
	const std::unique_ptr<CollisionShape>& shape1,
	const std::unique_ptr<CollisionShape>& shape2) const
{
	return Vec3f::Zero();
}

Vec3f CollisionManager::CheckSphereSphere(
	const std::unique_ptr<CollisionShape>& shape1,
	const std::unique_ptr<CollisionShape>& shape2) const
{
	return Vec3f::Zero();
}
