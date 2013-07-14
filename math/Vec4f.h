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

#ifndef NANAKA_MATH_VEC4F_H
#define NANAKA_MATH_VEC4F_H

#include <cstddef>

union Vec4f final
{
public:

	Vec4f() = default;
	explicit Vec4f(float pv);
	Vec4f(float px, float py, float pz, float pw);

	float& operator[](size_t i);
	const float& operator[](size_t i) const;

	float* data();
	const float* data() const;

	static Vec4f Zero();

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-pedantic"
#endif // defined(__GNUC__) && !defined (__clang__)

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu"
#endif // defined(__clang__)

	struct { float x, y, z, w; };
	struct { float r, g, b, a; };
	struct { float data[4]; } v;

#if defined(__clang__)
#pragma clang diagnostic pop
#endif // defined(__clang__)

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && !defined (__clang__)
};

inline Vec4f::Vec4f(float pv)
	: v({{pv, pv, pv, pv}})
{
}

inline Vec4f::Vec4f(float px, float py, float pz, float pw)
	: v({{px, py, pz, pw}})
{
}

inline float* Vec4f::data()
{
	return v.data;
}

inline const float* Vec4f::data() const
{
	return v.data;
}

inline float& Vec4f::operator[](size_t i)
{
	return v.data[i];
}

inline const float& Vec4f::operator[](size_t i) const
{
	return v.data[i];
}

inline Vec4f Vec4f::Zero()
{
	return Vec4f(0.0f);
}

#endif // NANAKA_MATH_VEC4F_H
