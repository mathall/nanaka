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

#include "gui/Gauge.h"

#include <algorithm>

#include "gui/Panel.h"
#include "gui/Image.h"

Gauge::Gauge(
	bool vertical,
	float width,
	float endWidth,
	GUISizeUnit sizeUnit,
	std::string fillEndTex,
	std::string emptyEndTex,
	std::string middleFullTex,
	std::string middleEmptyTex,
	Vec4f color)
	: m_vertical(vertical)
	, m_endSize(endWidth, width)
	, m_middleFull(middleFullTex, color,
		m_vertical ? ImageRotation270Deg : ImageRotationNone)
	, m_middleEmpty(middleEmptyTex, color,
		m_vertical ? ImageRotation270Deg : ImageRotationNone)
{
	auto content = std::make_shared<Panel>();

	{
		LayoutMethod::Orientation orientation = LayoutMethod::Horizontal;
		ImageRotation imageRotation = ImageRotationNone;
		if (m_vertical)
		{
			std::swap(m_endSize.x, m_endSize.y);
			orientation = LayoutMethod::Vertical;
			imageRotation = ImageRotation270Deg;
		}

		content->SetLayoutMethod(LayoutMethod(
			LayoutMethod::Linear, orientation));

		auto start = std::make_shared<Image>(
			fillEndTex, color, m_endSize, sizeUnit, imageRotation);
		auto middle = std::make_shared<Panel>();
		m_endFull = std::make_shared<Image>(
			fillEndTex, color, m_endSize, sizeUnit, imageRotation, true);
		m_endEmpty = std::make_shared<Image>(
			emptyEndTex, color, m_endSize, sizeUnit, imageRotation, true);

		content->AddWidget(start);
		content->AddWidget(middle);
		content->AddWidget(m_endFull);
		content->AddWidget(m_endEmpty);
	}

	AddWidget(content);

	LayoutProperties layoutProperties;
	if (m_vertical)
	{
		layoutProperties.m_growVertically = true;
	}
	else
	{
		layoutProperties.m_growHorizontally = true;
	}
	SetLayoutProperties(layoutProperties);

	// set m_load to 1.0f before reseting to trigger proper visibility settings.
	m_load = 1.0f;
	SetLoad(0.0f);
}

void Gauge::OnDraw(RenderPipeline& renderPipeline) const
{
	m_middleFull.Draw(renderPipeline);
	m_middleEmpty.Draw(renderPipeline);
}

void Gauge::OnAlphaUpdated(float totalAlpha)
{
	m_middleFull.SetAlpha(totalAlpha);
	m_middleEmpty.SetAlpha(totalAlpha);
}

void Gauge::SetLoad(float load)
{
	float lastLoad = m_load;
	m_load = std::max(0.0f, std::min(1.0f, load));

	if (m_load == 1.0f && lastLoad != 1.0f)
	{
		m_endFull->SetVisible(true);
		m_endEmpty->SetVisible(false);
	}
	else if (m_load != 1.0f && lastLoad == 1.0f)
	{
		m_endFull->SetVisible(false);
		m_endEmpty->SetVisible(true);
	}

	if (m_load != lastLoad)
	{
		UpdateMiddle();
	}
}

void Gauge::UpdateMiddle()
{
	Vec2f endSizeScale =
		m_vertical ? Vec2f(0.0f, 1.0f) : Vec2f(1.0f, 0.0f);
	Vec2f fullLoadScale =
		m_vertical ? Vec2f(1.0f, m_load) : Vec2f(m_load, 1.0f);
	Vec2f emptyLoadScale =
		(m_vertical ? Vec2f(2.0f, 1.0f) : Vec2f(1.0f, 2.0f)) - fullLoadScale;
	Vec2f middleSize = m_size - m_endSize * 2 * endSizeScale;
	Vec2f middleFullSize = middleSize * fullLoadScale;
	Vec2f middleEmptySize = middleSize * emptyLoadScale;

	Vec2f middleFullOffset = m_endSize * endSizeScale;
	Vec2f middleEmptyOffset = middleFullOffset + middleFullSize * endSizeScale;

	m_middleFull.SetPlacement(
		m_position + middleFullOffset, middleFullSize, m_depth);
	m_middleEmpty.SetPlacement(
		m_position + middleEmptyOffset, middleEmptySize, m_depth);
}
