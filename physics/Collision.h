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

#ifndef NANAKA_PHYSICS_COLLISION_H
#define NANAKA_PHYSICS_COLLISION_H

#include <memory>
#include <vector>

#include "physics/CollisionSkin.h"
#include "physics/Impact.h"

/**
 * Collision describes the collision between two CollisionSkins.
 */
class Collision final
{
public:

	void Resolve() const;
	void Respond() const;

	const CollisionSkin& GetCollider() const;
	const CollisionSkin& GetCollidee() const;

	void AddImpact(Impact impact);
	const std::vector<Impact>& GetImpacts() const;

	void Instantiate(
		const CollisionSkin* collider,
		const CollisionSkin* collidee);

	struct Clear
	{
		void operator()(Collision& collision);
	};

private:

	const CollisionSkin* m_collider;
	const CollisionSkin* m_collidee;

	std::vector<Impact> m_impacts;
};

inline void Collision::Resolve() const
{
	// Not implemented yet.
}

inline void Collision::Respond() const
{
	m_collider->DoCollisionCallback(*this);
}

inline void Collision::AddImpact(Impact impact)
{
	m_impacts.push_back(impact);
}

inline const CollisionSkin& Collision::GetCollider() const
{
	return *m_collider;
}

inline const CollisionSkin& Collision::GetCollidee() const
{
	return *m_collidee;
}

inline const std::vector<Impact>& Collision::GetImpacts() const
{
	return m_impacts;
}

#endif // NANAKA_PHYSICS_COLLISION_H
