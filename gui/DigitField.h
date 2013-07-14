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

#ifndef NANAKA_GUI_DIGITFIELD_H
#define NANAKA_GUI_DIGITFIELD_H

#include <memory>
#include <vector>

#include "math/Vec2f.h"

class Font;
class RenderPipeline;
class StaticChar;

class DigitField final
{
public:

	DigitField(const Font& font, unsigned int maxDigits, char fillChar);

	void SetNumber(unsigned int num);
	float Place(Vec2f position, float depth, float textSize);
	void Draw(RenderPipeline& renderPipeline) const;
	float GetAspectRatio() const;

private:

	static const int s_fillchar = 10;

	std::vector<std::vector<std::shared_ptr<StaticChar>>> m_digitLookups;
	std::vector<unsigned int> m_number;
	float m_aspectRatio;
};

inline float DigitField::GetAspectRatio() const
{
	return m_aspectRatio;
}

#endif // NANAKA_GUI_DIGITFIELD_H
