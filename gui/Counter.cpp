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

#include "gui/Counter.h"

#include <cmath>

#include "gui/Font.h"
#include "math/Vec3f.h"
#include "renderer/RenderPipeline.h"

Counter::Counter(
	const Font& font,
	float textSize,
	GUISizeUnit sizeUnit,
	unsigned int maxDigits,
	unsigned int decimals,
	char fillChar)
	: m_dotChar(font, '.')
	, m_subChar(font, ',')
	, m_NPart(font, maxDigits - decimals, fillChar)
	, m_FPart(font, decimals, '0')
	, m_hasFPart(decimals > 0)
	, m_decPow(pow(10, decimals))
{
	float aspectRatio = m_NPart.GetAspectRatio()
		+ (m_hasFPart ? m_FPart.GetAspectRatio() : 0.0f)
		+ font.GetAspectRatio(',')
		+ font.GetAspectRatio('.');

	LayoutProperties layoutProperties;
	layoutProperties.m_desiredSize = Vec2f(aspectRatio * textSize, textSize);
	layoutProperties.m_desiredSizeUnit = sizeUnit;
	layoutProperties.m_aspectRatio = aspectRatio;
	SetLayoutProperties(layoutProperties);

	SetNumber(0);
}

void Counter::UpdateDigitFields()
{
	float charH = m_size.y;

	float dotCharW = charH * m_dotChar.GetAspectRatio();
	float subCharW = charH * m_subChar.GetAspectRatio();

	if (m_printSub)
	{
		m_subChar.SetPosition(Vec3f(
			m_position.x + subCharW * 0.5f,
			m_position.y + charH * 0.5f,
			m_depth));
		m_subChar.SetScale(Vec3f(subCharW, charH, 1.0f));
	}

	float afterNPartX = m_NPart.Place(
		Vec2f(m_position.x + subCharW, m_position.y), m_depth, charH);

	if (m_hasFPart)
	{
		m_dotChar.SetPosition(Vec3f(
			afterNPartX + dotCharW * 0.5f,
			m_position.y + charH * 0.5f,
			m_depth));
		m_dotChar.SetScale(Vec3f(dotCharW, charH, 1.0f));

		m_FPart.Place(
			Vec2f(afterNPartX + dotCharW, m_position.y),
			m_depth, charH);
	}
}

void Counter::OnDraw(RenderPipeline& renderPipeline) const
{
	if (m_printSub)
	{
		auto RE = renderPipeline.GetRE();
		m_subChar.PrepRender(*RE);
		renderPipeline.QueueRE(std::move(RE));
	}

	m_NPart.Draw(renderPipeline);

	if (m_hasFPart)
	{
		auto RE = renderPipeline.GetRE();
		m_dotChar.PrepRender(*RE);
		renderPipeline.QueueRE(std::move(RE));

		m_FPart.Draw(renderPipeline);
	}
}

void Counter::SetNumber(float num)
{
	if (num >= 0)
	{
		m_printSub = false;
	}
	else
	{
		num *= -1;
		m_printSub = true;
	}

	int nPart = static_cast<int>(num);
	m_NPart.SetNumber(nPart);

	if (m_hasFPart)
	{
		m_FPart.SetNumber(static_cast<int>((num - nPart) * m_decPow));
	}

	UpdateDigitFields();
}
