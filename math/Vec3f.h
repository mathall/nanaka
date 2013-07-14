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

#ifndef NANAKA_MATH_VEC3F_H
#define NANAKA_MATH_VEC3F_H

#include <cstddef>
#include <cmath>

union Vec3f final
{
public:

	Vec3f() = default;
	explicit Vec3f(float pv);
	Vec3f(float px, float py, float pz);

	Vec3f operator-() const;

	Vec3f operator-(const Vec3f& pv) const;
	Vec3f operator+(const Vec3f& pv) const;
	Vec3f operator*(const Vec3f& pv) const;
	Vec3f operator/(const Vec3f& pv) const;

	Vec3f& operator+=(const Vec3f& pv);
	Vec3f& operator-=(const Vec3f& pv);
	Vec3f& operator*=(const Vec3f& pv);
	Vec3f& operator/=(const Vec3f& pv);

	Vec3f operator*(float pv) const;
	Vec3f operator/(float pv) const;

	Vec3f& operator*=(float pv);
	Vec3f& operator/=(float pv);

	float& operator[](size_t i);
	const float& operator[](size_t i) const;

	Vec3f Perp() const;
	Vec3f PerpTowards(const Vec3f& pv) const;
	Vec3f ProjectedOnto(const Vec3f& pv) const;
	float Dot(const Vec3f& pv) const;
	Vec3f Cross(const Vec3f& pv) const;
	Vec3f& Normalize();
	Vec3f Normalized() const;
	float SquareSum() const;
	float Length() const;
	bool Equals(const Vec3f& pv) const;
	bool AnyEquals(const Vec3f& pv) const;
	bool AnyEquals(float pv) const;

	float* data();
	const float* data() const;

	static Vec3f Zero();
	static Vec3f UnitX();
	static Vec3f UnitY();
	static Vec3f UnitZ();

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-pedantic"
#endif // defined(__GNUC__) && !defined (__clang__)

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu"
#endif // defined(__clang__)

	struct { float x, y, z; };
	struct { float data[3]; } v;

#if defined(__clang__)
#pragma clang diagnostic pop
#endif // defined(__clang__)

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && !defined (__clang__)
};

inline Vec3f::Vec3f(float pv)
	: v({{pv, pv, pv}})
{
}

inline Vec3f::Vec3f(float px, float py, float pz)
	: v({{px, py, pz}})
{
}

inline Vec3f Vec3f::operator-() const
{
	return Vec3f(-x, -y, -z);
}

inline Vec3f Vec3f::operator+(const Vec3f& pv) const
{
	return Vec3f(x + pv.x, y + pv.y, z + pv.z);
}

inline Vec3f Vec3f::operator-(const Vec3f& pv) const
{
	return Vec3f(x - pv.x, y - pv.y, z - pv.z);
}

inline Vec3f Vec3f::operator*(const Vec3f& pv) const
{
	return Vec3f(x * pv.x, y * pv.y, z * pv.z);
}

inline Vec3f Vec3f::operator/(const Vec3f& pv) const
{
	return Vec3f(x / pv.x, y / pv.y, z / pv.z);
}

inline Vec3f& Vec3f::operator+=(const Vec3f& pv)
{
	return *this = *this + pv;
}

inline Vec3f& Vec3f::operator-=(const Vec3f& pv)
{
	return *this = *this - pv;
}

inline Vec3f& Vec3f::operator*=(const Vec3f& pv)
{
	return *this = *this * pv;
}

inline Vec3f& Vec3f::operator/=(const Vec3f& pv)
{
	return *this = *this / pv;
}

inline Vec3f Vec3f::operator*(float pv) const
{
	return *this * Vec3f(pv);
}

inline Vec3f Vec3f::operator/(float pv) const
{
	return *this / Vec3f(pv);
}

inline Vec3f& Vec3f::operator*=(float pv)
{
	return *this *= Vec3f(pv);
}

inline Vec3f& Vec3f::operator/=(float pv)
{
	return *this /= Vec3f(pv);
}

inline float& Vec3f::operator[](size_t i)
{
	return v.data[i];
}

inline const float& Vec3f::operator[](size_t i) const
{
	return v.data[i];
}

inline Vec3f Vec3f::Perp() const
{
	return
		x ? Vec3f(-y / x, 1.0f, 0.0f) :
		y ? Vec3f(0.0f, -z / y, 1.0f) :
		z ? Vec3f(0.0f, 1.0f, -y / z) :
		Vec3f(0.0f, 0.0f, 0.0f);
}

inline Vec3f Vec3f::PerpTowards(const Vec3f& pv) const
{
	return pv - pv.ProjectedOnto(*this);
}

inline Vec3f Vec3f::ProjectedOnto(const Vec3f& pv) const
{
	const auto thisNormalized = this->Normalized();
	return thisNormalized * pv.Dot(thisNormalized);
}

inline float Vec3f::Dot(const Vec3f& pv) const
{
	return x * pv.x + y * pv.y + z * pv.z;
}

inline Vec3f Vec3f::Cross(const Vec3f& pv) const
{
	return Vec3f(y * pv.z - z * pv.y, z * pv.x - x * pv.z, x * pv.y - y * pv.x);
}

inline Vec3f& Vec3f::Normalize()
{
	return *this = Normalized();
}

inline Vec3f Vec3f::Normalized() const
{
	return *this / Length();
}

inline float Vec3f::SquareSum() const
{
	return x * x + y * y + z * z;
}

inline float Vec3f::Length() const
{
	return sqrt(SquareSum());
}

inline float* Vec3f::data()
{
	return v.data;
}

inline const float* Vec3f::data() const
{
	return v.data;
}

inline bool Vec3f::Equals(const Vec3f& pv) const
{
	return x == pv.x && y == pv.y && z == pv.z;
}

inline bool Vec3f::AnyEquals(const Vec3f& pv) const
{
	return x == pv.x || y == pv.y || z == pv.z;
}

inline bool Vec3f::AnyEquals(float pv) const
{
	return AnyEquals(Vec3f(pv));
}

inline Vec3f Vec3f::Zero()
{
	return Vec3f(0.0f);
}

inline Vec3f Vec3f::UnitX()
{
	return Vec3f(1.0f, 0.0f, 0.0f);
}

inline Vec3f Vec3f::UnitY()
{
	return Vec3f(0.0f, 1.0f, 0.0f);
}

inline Vec3f Vec3f::UnitZ()
{
	return Vec3f(0.0f, 0.0f, 1.0f);
}

#endif // NANAKA_MATH_VEC3F_H
