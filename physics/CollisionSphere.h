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

#ifndef NANAKA_PHYSICS_COLLISIONSPHERE_H
#define NANAKA_PHYSICS_COLLISIONSPHERE_H

#include "physics/CollisionShape.h"

class CollisionSphere final : public CollisionShape
{
public:

	CollisionSphere();

	void SetOrigin(Vec3f origin);
	Vec3f GetOrigin() const;

	void SetRadius(float radius);
	float GetRadius() const;

	/**
	 * CollisionShape implementation.
	 */
	void SkinPlacementUpdated(Vec3f skinPosition, Quat skinRotation) override;

private:

	Vec3f m_origin;
	Vec3f m_finalOrigin;
	float m_radius;
};

inline CollisionSphere::CollisionSphere()
	: CollisionShape(SphereCollisionShapeType)
{
}

inline void CollisionSphere::SkinPlacementUpdated(
	Vec3f skinPosition,
	Quat skinRotation)
{
	m_finalOrigin = skinPosition + m_origin * skinRotation;
}

inline void CollisionSphere::SetOrigin(Vec3f origin)
{
	m_origin = origin;
}

inline Vec3f CollisionSphere::GetOrigin() const
{
	return m_finalOrigin;
}

inline void CollisionSphere::SetRadius(float radius)
{
	m_radius = radius;
}

inline float CollisionSphere::GetRadius() const
{
	return m_radius;
}

#endif // NANAKA_PHYSICS_COLLISIONSPHERE_H
