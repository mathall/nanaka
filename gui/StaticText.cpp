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

#include "gui/StaticText.h"

#include <sstream>

#include "gui/Font.h"
#include "gui/StaticChar.h"
#include "renderer/RenderData.h"

StaticText::StaticText(
	std::string text,
	const Font& font,
	float textSize,
	GUISizeUnit sizeUnit)
{
	float maxLineAspectRatio = 0.0f;

	std::stringstream stream(text);
	std::string lineString;

	while (stream.good())
	{
		std::getline(stream, lineString);
		std::vector<std::shared_ptr<StaticChar>> line;
		float lineAspectRatio = 0.0f;

		for (auto character : lineString)
		{
			if (font.HasGlyph(character))
			{
				line.push_back(std::make_shared<StaticChar>(font, character));
				lineAspectRatio += font.GetAspectRatio(character);
			}
		}

		m_lines.push_back(line);
		maxLineAspectRatio = std::max(maxLineAspectRatio, lineAspectRatio);
	}

	float textHeight = m_lines.size() * textSize;
	float textWidth = maxLineAspectRatio * textSize;

	LayoutProperties layoutProperties;
	layoutProperties.m_desiredSize = Vec2f(textWidth, textHeight);
	layoutProperties.m_desiredSizeUnit = sizeUnit;
	layoutProperties.m_aspectRatio = textWidth / textHeight;
	SetLayoutProperties(layoutProperties);
}

void StaticText::OnPlacementUpdated()
{
	float lineH = m_size.y / m_lines.size();
	float nextY = m_position.y;

	for (auto line : m_lines)
	{
		float nextX = m_position.x;

		for (auto character : line)
		{
			float charW = lineH * character->GetAspectRatio();

			character->SetPosition(
				Vec3f(nextX + charW * 0.5f, nextY + lineH * 0.5f, m_depth));
			character->SetScale(Vec3f(charW, lineH, 1.0f));

			nextX += charW;
		}

		nextY += lineH;
	}
}

void StaticText::OnDraw(RenderData& renderData) const
{
	for (auto line : m_lines)
	{
		for (auto character : line)
		{
			auto RE = renderData.GetRenderElement();
			character->PrepRender(*RE);
			renderData.QueueRenderElement(std::move(RE));
		}
	}
}
