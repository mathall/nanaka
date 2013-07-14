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

#ifndef NANAKA_RENDERER_PROJECTION_H
#define NANAKA_RENDERER_PROJECTION_H

#include "math/Vec2f.h"
#include "math/Vec3f.h"
#include "math/Mat44.h"

class Projection final
{
public:

	Projection();

	void SetViewDimensions(Vec2f dim);
	void SetPosition(Vec3f position);
	void SetViewDirection(Vec3f viewDirection);
	void SetUpVector(Vec3f up);
	const Mat44& GetView() const;
	const Mat44& GetProj() const;

private:

	void UpdateCachedView();
	void UpdateCachedProj();

	Vec2f m_viewDimensions;
	Vec3f m_position;
	Vec3f m_viewDirection;
	Vec3f m_up;

	Mat44 m_cachedProj;
	Mat44 m_cachedView;
};

inline void Projection::UpdateCachedView()
{
	m_cachedView = Mat44::LookAt(m_position, m_viewDirection, m_up);
}

inline const Mat44& Projection::GetView() const
{
	return m_cachedView;
}

inline const Mat44& Projection::GetProj() const
{
	return m_cachedProj;
}

#endif // NANAKA_RENDERER_PROJECTION_H
