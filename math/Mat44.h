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

#ifndef NANAKA_MATH_MAT44_H
#define NANAKA_MATH_MAT44_H

#include <array>
#include <cstddef>

#include "math/Vec3f.h"
#include "math/Quat.h"

class Mat44 final
{
public:

	Mat44() = default;

	Mat44 operator*(const Mat44& mat) const;

	float* operator[](size_t i);
	const float* operator[](size_t i) const;

	float* data();
	const float* data() const;

	static Mat44 ToScaleMatrix(const Vec3f& scale);
	static Mat44 ToRotationMatrix(const Quat& rotation);
	static Mat44 MakeTranslationMatrix(const Vec3f& trans);

	static Mat44 Ortho(
		float left,
		float right,
		float top,
		float bottom,
		float near,
		float far);

	static Mat44 LookAt(const Vec3f& pos, const Vec3f& dir, const Vec3f& up);

	static Mat44 Identity();

	static Mat44 MakeTransform(
		const Vec3f& translate,
		const Quat& rotate,
		const Vec3f& scale);

	const Mat44& Transform(
		const Vec3f& translate,
		const Quat& rotate,
		const Vec3f& scale);

	std::array<float, 16> m_data;
};

inline float* Mat44::data()
{
	return m_data.data();
}

inline const float* Mat44::data() const
{
	return m_data.data();
}

inline float* Mat44::operator[](size_t i)
{
	return &m_data[i*4];
}

inline const float* Mat44::operator[](size_t i) const
{
	return &m_data[i*4];
}

inline Mat44 Mat44::operator*(const Mat44& mat) const
{
	Mat44 ret;

	#define MATMUL(C,R) \
		ret[(C)][(R)] = \
		(*this)[0][(R)] * mat[(C)][0] + \
		(*this)[1][(R)] * mat[(C)][1] + \
		(*this)[2][(R)] * mat[(C)][2] + \
		(*this)[3][(R)] * mat[(C)][3]

	MATMUL(0,0);
	MATMUL(0,1);
	MATMUL(0,2);
	MATMUL(0,3);

	MATMUL(1,0);
	MATMUL(1,1);
	MATMUL(1,2);
	MATMUL(1,3);

	MATMUL(2,0);
	MATMUL(2,1);
	MATMUL(2,2);
	MATMUL(2,3);

	MATMUL(3,0);
	MATMUL(3,1);
	MATMUL(3,2);
	MATMUL(3,3);

	#undef MATMUL

	return ret;
}

inline Mat44 Mat44::ToScaleMatrix(const Vec3f& scale)
{
	Mat44 scaleMat;

	scaleMat[0][0] = scale.x;
	scaleMat[0][1] = 0.0f;
	scaleMat[0][2] = 0.0f;
	scaleMat[0][3] = 0.0f;

	scaleMat[1][0] = 0.0f;
	scaleMat[1][1] = scale.y;
	scaleMat[1][2] = 0.0f;
	scaleMat[1][3] = 0.0f;

	scaleMat[2][0] = 0.0f;
	scaleMat[2][1] = 0.0f;
	scaleMat[2][2] = scale.z;
	scaleMat[2][3] = 0.0f;

	scaleMat[3][0] = 0.0f;
	scaleMat[3][1] = 0.0f;
	scaleMat[3][2] = 0.0f;
	scaleMat[3][3] = 1.0f;

	return scaleMat;
}

inline Mat44 Mat44::ToRotationMatrix(const Quat& rotation)
{
	Mat44 rotMat;

	float x2 = rotation.v.x * rotation.v.x;
	float y2 = rotation.v.y * rotation.v.y;
	float z2 = rotation.v.z * rotation.v.z;
	float xy = rotation.v.x * rotation.v.y;
	float xz = rotation.v.x * rotation.v.z;
	float yz = rotation.v.y * rotation.v.z;
	float wx = rotation.w * rotation.v.x;
	float wy = rotation.w * rotation.v.y;
	float wz = rotation.w * rotation.v.z;

	rotMat[0][0] = 1.0f - 2.0f * (y2 + z2);
	rotMat[0][1] = 2.0f * (xy + wz);
	rotMat[0][2] = 2.0f * (xz - wy);
	rotMat[0][3] = 0.0f;

	rotMat[1][0] = 2.0f * (xy - wz);
	rotMat[1][1] = 1.0f - 2.0f * (x2 + z2);
	rotMat[1][2] = 2.0f * (yz + wx);
	rotMat[1][3] = 0.0f;

	rotMat[2][0] = 2.0f * (xz + wy);
	rotMat[2][1] = 2.0f * (yz - wx);
	rotMat[2][2] = 1.0f - 2.0f * (x2 + y2);
	rotMat[2][3] = 0.0f;

	rotMat[3][0] = 0.0f;
	rotMat[3][1] = 0.0f;
	rotMat[3][2] = 0.0f;
	rotMat[3][3] = 1.0f;

	return rotMat;
}

inline Mat44 Mat44::MakeTranslationMatrix(const Vec3f& trans)
{
	Mat44 transMat;

	transMat[0][0] = 1.0f;
	transMat[0][1] = 0.0f;
	transMat[0][2] = 0.0f;
	transMat[0][3] = 0.0f;

	transMat[1][0] = 0.0f;
	transMat[1][1] = 1.0f;
	transMat[1][2] = 0.0f;
	transMat[1][3] = 0.0f;

	transMat[2][0] = 0.0f;
	transMat[2][1] = 0.0f;
	transMat[2][2] = 1.0f;
	transMat[2][3] = 0.0f;

	transMat[3][0] = trans.x;
	transMat[3][1] = trans.y;
	transMat[3][2] = trans.z;
	transMat[3][3] = 1.0f;

	return transMat;
}

inline Mat44 Mat44::Ortho(
	float left,
	float right,
	float top,
	float bottom,
	float near,
	float far)
{
	Mat44 orthoMat;

	orthoMat[0][0] = 2.0f / (right - left);
	orthoMat[0][1] = 0.0f;
	orthoMat[0][2] = 0.0f;
	orthoMat[0][3] = 0.0f;

	orthoMat[1][0] = 0.0f;
	orthoMat[1][1] = 2.0f / (top - bottom);
	orthoMat[1][2] = 0.0f;
	orthoMat[1][3] = 0.0f;

	orthoMat[2][0] = 0.0f;
	orthoMat[2][1] = 0.0f;
	orthoMat[2][2] = -2.0f / (far - near);
	orthoMat[2][3] = 0.0f;

	orthoMat[3][0] = -(right + left) / (right - left);
	orthoMat[3][1] = -(top + bottom) / (top - bottom);
	orthoMat[3][2] = -(far + near) / (far - near);
	orthoMat[3][3] = 1.0f;

	return orthoMat;
}

inline Mat44 Mat44::LookAt(const Vec3f& pos, const Vec3f& dir, const Vec3f& up)
{
	const Vec3f normDir = dir.Normalized();
	const Vec3f normUp = up.Normalized();
	const Vec3f normRight = normDir.Cross(normUp).Normalized();

	Mat44 viewMat;

	viewMat[0][0] = normRight.x;
	viewMat[0][1] = normUp.x;
	viewMat[0][2] = -normDir.x;
	viewMat[0][3] = 0.0f;

	viewMat[1][0] = normRight.y;
	viewMat[1][1] = normUp.y;
	viewMat[1][2] = -normDir.y;
	viewMat[1][3] = 0.0f;

	viewMat[2][0] = normRight.z;
	viewMat[2][1] = normUp.z;
	viewMat[2][2] = -normDir.z;
	viewMat[2][3] = 0.0f;

	viewMat[3][0] = -(normRight.Dot(pos));
	viewMat[3][1] = -(normUp.Dot(pos));
	viewMat[3][2] = (normDir.Dot(pos));
	viewMat[3][3] = 1.0f;

	return viewMat;
}

inline Mat44 Mat44::Identity()
{
	Mat44 identityMat;

	identityMat[0][0] = 1.0f;
	identityMat[0][1] = 0.0f;
	identityMat[0][2] = 0.0f;
	identityMat[0][3] = 0.0f;

	identityMat[1][0] = 0.0f;
	identityMat[1][1] = 1.0f;
	identityMat[1][2] = 0.0f;
	identityMat[1][3] = 0.0f;

	identityMat[2][0] = 0.0f;
	identityMat[2][1] = 0.0f;
	identityMat[2][2] = 1.0f;
	identityMat[2][3] = 0.0f;

	identityMat[3][0] = 0.0f;
	identityMat[3][1] = 0.0f;
	identityMat[3][2] = 0.0f;
	identityMat[3][3] = 1.0f;

	return identityMat;
}

inline Mat44 Mat44::MakeTransform(
	const Vec3f& translate,
	const Quat& rotate,
	const Vec3f& scale)
{
	const auto transMat = Mat44::MakeTranslationMatrix(translate);
	const auto rotMat = Mat44::ToRotationMatrix(rotate);
	const auto scaleMat = Mat44::ToScaleMatrix(scale);
	return transMat * rotMat * scaleMat;
}

inline const Mat44& Mat44::Transform(
	const Vec3f& translate,
	const Quat& rotate,
	const Vec3f& scale)
{
	return *this = MakeTransform(translate, rotate, scale) * *this;
}

#endif // NANAKA_MATH_MAT44_H
