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

#ifndef NANAKA_SCENE_CAMERA_H
#define NANAKA_SCENE_CAMERA_H

#include "math/Vec2f.h"
#include "math/Vec3f.h"
#include "utils/UUID.h"

class Projection;

/**
 * The Camera only need a RenderContext Id before being used for drawing a
 * scene. This is so that the renderer will know where to draw to. The Id is
 * generally attained through the GUI.
 */
class Camera final
{
public:

	Camera();

	void SetViewDimensions(Vec2f viewDimensions);

	void SetPosition(Vec3f position);
	void SetViewDirection(Vec3f viewDirection);
	void SetUpVector(Vec3f upVector);

	void SetRenderContextId(UUID renderContextId);
	UUID GetRenderContextId() const;

	void Setup(Projection& projection) const;

protected:

	Vec3f m_position;
	Vec3f m_viewDirection;
	Vec3f m_upVector;
	Vec2f m_viewDimensions;
	UUID m_renderContextId;
};

inline Camera::Camera()
	: m_position(-Vec3f::UnitY())
	, m_viewDirection(Vec3f::UnitY())
	, m_upVector(Vec3f::UnitZ())
	, m_viewDimensions(Vec2f::Zero())
	, m_renderContextId(UUID::Invalid)
{
}

inline void Camera::SetViewDimensions(Vec2f viewDimensions)
{
	m_viewDimensions = viewDimensions;
}

inline void Camera::SetPosition(Vec3f position)
{
	m_position = position;
}

inline void Camera::SetViewDirection(Vec3f viewDirection)
{
	m_viewDirection = viewDirection;
}

inline void Camera::SetUpVector(Vec3f upVector)
{
	m_upVector = upVector;
}

inline void Camera::SetRenderContextId(UUID renderContextId)
{
	m_renderContextId = renderContextId;
}

inline UUID Camera::GetRenderContextId() const
{
	return m_renderContextId;
}

#endif // NANAKA_SCENE_CAMERA_H
