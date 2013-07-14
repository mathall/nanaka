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

#ifndef NANAKA_GRAPHICS_INTERPOLATION_H
#define NANAKA_GRAPHICS_INTERPOLATION_H

#include <functional>

template<typename T>
class Interpolation final
{
public:

	explicit Interpolation(
		std::function<T(float, const T&, const T&)> interpolationFunction);

	static Interpolation<T> Flat();
	static Interpolation<T> Linear();
	static Interpolation<T> Curb(std::function<float(float)> curb);

	T Eval(float p, const T& start, const T& end) const;

private:

	std::function<T(float, const T&, const T&)> m_interpolationFunction;
};

template<typename T>
inline Interpolation<T>::Interpolation(
	std::function<T(float, const T&, const T&)> interpolationFunction)
	: m_interpolationFunction(interpolationFunction)
{
}

template<typename T>
Interpolation<T> Interpolation<T>::Flat()
{
	return Interpolation<T>(
		[](float p, const T& start, const T& end) { return start; });
}

template<typename T>
Interpolation<T> Interpolation<T>::Linear()
{
	return Curb([](float x) { return x; });
}

template<typename T>
Interpolation<T> Interpolation<T>::Curb(std::function<float(float)> curb)
{
	return Interpolation<T>(
		[curb](float p, const T& start, const T& end)
		{ return start + (end - start) * curb(p); });
}

template<typename T>
T Interpolation<T>::Eval(float p, const T& start, const T& end) const
{
	return m_interpolationFunction(p, start, end);
}

#endif // NANAKA_GRAPHICS_INTERPOLATION_H
