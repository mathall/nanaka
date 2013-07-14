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

#include "gui/LayoutProperties.h"

#include <algorithm>

LayoutProperties::LayoutProperties()
	: m_sizeWeight(1.0f)
	, m_aspectRatio(0.0f)
	, m_relPosition(Vec2f::Zero())
	, m_relSize(Vec2f(1.0f, 1.0f))
	, m_desiredSize(Vec2f::Zero())
	, m_desiredSizeUnit(pxGUISizeUnit)
	, m_growVertically(false)
	, m_growHorizontally(false)
	, m_z(0)
{
}

float LayoutProperties::DesiredWidthAtHeight(float height) const
{
	if (!FillH() && FillV())
	{
		return m_desiredPixelSize.x;
	}
	else if (!FillH() && !FillV())
	{
		return std::min(m_desiredPixelSize.x, height * m_aspectRatio);
	}
	else if (m_aspectRatio > 0.0f)
	{
		return height * m_aspectRatio;
	}

	return 0.0f;
}

float LayoutProperties::DesiredHeightAtWidth(float width) const
{
	if (!FillV() && FillH())
	{
		return m_desiredPixelSize.y;
	}
	else if (!FillV() && !FillH())
	{
		return std::min(m_desiredPixelSize.y, width / m_aspectRatio);
	}
	else if (m_aspectRatio > 0.0f)
	{
		return width / m_aspectRatio;
	}

	return 0.0f;
}
