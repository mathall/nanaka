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

#include "gui/DigitField.h"

#include <cassert>
#include <cmath>

#include "gui/Font.h"
#include "gui/StaticChar.h"
#include "renderer/RenderPipeline.h"

DigitField::DigitField(const Font& font, unsigned int maxDigits, char fillChar)
{
	maxDigits = maxDigits == 0 ? 1 : maxDigits;

	m_number.resize(maxDigits);

	std::string digits("0123456789");
	digits.push_back(fillChar);

	float maxDigitAspectRatio = 0.0f;

	for (size_t i = 0; i < maxDigits; ++i)
	{
		std::vector<std::shared_ptr<StaticChar>> digitLookup;

		for (char digit : digits)
		{
			assert(font.HasGlyph(digit));
			digitLookup.push_back(std::make_shared<StaticChar>(font, digit));
			maxDigitAspectRatio =
				std::max(maxDigitAspectRatio, font.GetAspectRatio(digit));
		}

		m_digitLookups.push_back(digitLookup);
	}

	m_aspectRatio = maxDigits * maxDigitAspectRatio;

	SetNumber(0);
}

float DigitField::Place(Vec2f position, float depth, float textSize)
{
	float nextX = position.x;

	for (size_t i = m_number.size(); i > 0; --i)
	{
		auto character = m_digitLookups[i - 1][m_number[i - 1]];

		float charH = textSize;
		float charW = charH * character->GetAspectRatio();

		character->SetPosition(
			Vec3f(nextX + charW * 0.5f, position.y + charH * 0.5f, depth));
		character->SetScale(Vec3f(charW, charH, 1.0f));

		nextX += charW;
	}

	return nextX;
}

void DigitField::Draw(RenderPipeline& renderPipeline) const
{
	for (size_t i = 0; i < m_number.size(); ++i)
	{
		auto RE = renderPipeline.GetRE();
		m_digitLookups[i][m_number[i]]->PrepRender(*RE);
		renderPipeline.QueueRE(std::move(RE));
	}
}

void DigitField::SetNumber(unsigned int num)
{
	for (size_t i = 0; i < m_number.size(); ++i)
	{
		if (num == 0 && i != 0)
		{
			m_number[i] = s_fillchar;
		}
		else
		{
			m_number[i] = num % 10;
			num /= 10;
		}
	}
}
