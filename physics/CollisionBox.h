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

#ifndef NANAKA_PHYSICS_COLLISIONBOX_H
#define NANAKA_PHYSICS_COLLISIONBOX_H

#include "physics/CollisionShape.h"

class CollisionBox final : public CollisionShape
{
public:

	CollisionBox();

	void SetPosition(Vec3f position);
	Vec3f GetPosition() const;

	void SetDimensions(Vec3f dimensions);
	Vec3f GetDimensions() const;

	/**
	 * CollisionShape implementation.
	 */
	void SkinPlacementUpdated(Vec3f position, Quat skinRotation) override;

private:

	Vec3f m_position;
	Vec3f m_finalPosition;
	Vec3f m_dimensions;
};

inline CollisionBox::CollisionBox()
	: CollisionShape(BoxCollisionShapeType)
{
}

inline void CollisionBox::SkinPlacementUpdated(
	Vec3f skinPosition,
	Quat skinRotation)
{
	m_finalPosition = skinPosition + m_position * skinRotation;
}

inline void CollisionBox::SetPosition(Vec3f position)
{
	m_position = position;
}

inline Vec3f CollisionBox::GetPosition() const
{
	return m_finalPosition;
}

inline void CollisionBox::SetDimensions(Vec3f dimensions)
{
	m_dimensions = dimensions;
}

inline Vec3f CollisionBox::GetDimensions() const
{
	return m_dimensions;
}

#endif // NANAKA_PHYSICS_COLLISIONBOX_H
