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

#ifndef NANAKA_MATH_VEC2F_H
#define NANAKA_MATH_VEC2F_H

#include <cstddef>
#include <cmath>

union Vec2f final
{
public:

	Vec2f() = default;
	explicit Vec2f(float pv);
	Vec2f(float px, float py);

	Vec2f operator-() const;

	Vec2f operator+(const Vec2f& pv) const;
	Vec2f operator-(const Vec2f& pv) const;
	Vec2f operator*(const Vec2f& pv) const;
	Vec2f operator/(const Vec2f& pv) const;

	Vec2f& operator+=(const Vec2f& pv);
	Vec2f& operator-=(const Vec2f& pv);
	Vec2f& operator*=(const Vec2f& pv);
	Vec2f& operator/=(const Vec2f& pv);

	Vec2f operator*(float pv) const;
	Vec2f operator/(float pv) const;

	Vec2f& operator*=(float pv);
	Vec2f& operator/=(float pv);

	float& operator[](size_t i);
	const float& operator[](size_t i) const;

	float SquareSum() const;
	float Length() const;
	bool Equals(const Vec2f& pv) const;
	bool AnyEquals(const Vec2f& pv) const;
	bool AnyEquals(float pv) const;

	float* data();
	const float* data() const;

	static Vec2f Zero();

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-pedantic"
#endif // defined(__GNUC__) && !defined (__clang__)

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu"
#endif // defined(__clang__)

	struct { float x, y; };
	struct { float data[2]; } v;

#if defined(__clang__)
#pragma clang diagnostic pop
#endif // defined(__clang__)

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && !defined (__clang__)
};

inline Vec2f::Vec2f(float pv)
	: v({{pv, pv}})
{
}

inline Vec2f::Vec2f(float px, float py)
	: v({{px, py}})
{
}

inline Vec2f Vec2f::operator-() const
{
	return Vec2f(-x, -y);
}

inline Vec2f Vec2f::operator+(const Vec2f& pv) const
{
	return Vec2f(x + pv.x, y + pv.y);
}

inline Vec2f Vec2f::operator-(const Vec2f& pv) const
{
	return Vec2f(x - pv.x, y - pv.y);
}

inline Vec2f Vec2f::operator*(const Vec2f& pv) const
{
	return Vec2f(x * pv.x, y * pv.y);
}

inline Vec2f Vec2f::operator/(const Vec2f& pv) const
{
	return Vec2f(x / pv.x, y / pv.y);
}

inline Vec2f& Vec2f::operator+=(const Vec2f& pv)
{
	return *this = *this + pv;
}

inline Vec2f& Vec2f::operator-=(const Vec2f& pv)
{
	return *this = *this - pv;
}

inline Vec2f& Vec2f::operator*=(const Vec2f& pv)
{
	return *this = *this * pv;
}

inline Vec2f& Vec2f::operator/=(const Vec2f& pv)
{
	return *this = *this / pv;
}

inline Vec2f Vec2f::operator*(float pv) const
{
	return *this * Vec2f(pv);
}

inline Vec2f Vec2f::operator/(float pv) const
{
	return *this / Vec2f(pv);
}

inline Vec2f& Vec2f::operator*=(float pv)
{
	return *this *= Vec2f(pv);
}

inline Vec2f& Vec2f::operator/=(float pv)
{
	return *this /= Vec2f(pv);
}

inline float& Vec2f::operator[](size_t i)
{
	return v.data[i];
}

inline const float& Vec2f::operator[](size_t i) const
{
	return v.data[i];
}

inline float Vec2f::SquareSum() const
{
	return x * x + y * y;
}

inline float Vec2f::Length() const
{
	return sqrt(SquareSum());
}

inline bool Vec2f::Equals(const Vec2f& pv) const
{
	return x == pv.x && y == pv.y;
}

inline bool Vec2f::AnyEquals(const Vec2f& pv) const
{
	return x == pv.x || y == pv.y;
}

inline bool Vec2f::AnyEquals(float pv) const
{
	return AnyEquals(Vec2f(pv));
}

inline float* Vec2f::data()
{
	return v.data;
}

inline const float* Vec2f::data() const
{
	return v.data;
}

inline Vec2f Vec2f::Zero()
{
	return Vec2f(0.0f);
}

#endif // NANAKA_MATH_VEC2F_H
