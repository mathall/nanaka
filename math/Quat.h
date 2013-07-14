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

#ifndef NANAKA_MATH_QUAT_H
#define NANAKA_MATH_QUAT_H

#include <cmath>

#include "math/Vec3f.h"

class Quat final
{
public:

	Quat() = default;
	Quat(float pw, const Vec3f& pv);
	Quat(const Vec3f& axis, float angle);

	Quat operator*(const Quat& q) const;

	Quat operator/(float pv) const;

	float Length() const;
	Quat Inverted() const;

	static Quat NoRot();

	Vec3f v;
	float w;
};

Vec3f operator*(const Vec3f& pv, const Quat& q);

inline Quat::Quat(float pw, const Vec3f& pv)
	: v(pv)
	, w(pw)
{
}

inline Quat::Quat(const Vec3f& axis, float angle)
	: v(axis.Normalized() * sin(0.5f * angle))
	, w(cos(0.5f * angle))
{
}

inline Quat Quat::operator*(const Quat& q) const
{
	return Quat(
		w * q.w - v.x * q.v.x - v.y * q.v.y - v.z * q.v.z,
		Vec3f(
			w * q.v.x + v.x * q.w + v.y * q.v.z - v.z * q.v.y,
			w * q.v.y - v.x * q.v.z + v.y * q.w + v.z * q.v.x,
			w * q.v.z + v.x * q.v.y - v.y * q.v.x + v.z * q.w));
}

inline float Quat::Length() const
{
	return sqrt(v.SquareSum() + w * w);
}

inline Quat Quat::Inverted() const
{
	float length = Length();
	return Quat(w, -v) / (length * length);
}

inline Quat Quat::operator/(float pv) const
{
	static const float s_e = 0.000001f;
	return Quat(w / (pv + s_e), v / (pv + s_e));
}

inline Quat Quat::NoRot()
{
	return Quat(Vec3f::UnitX(), 0.0f);
}

inline Vec3f operator*(const Vec3f& pv, const Quat& q)
{
	return (q * Quat(0.0f, pv) * q.Inverted()).v;
}

#endif // NANAKA_MATH_QUAT_H
