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

#ifndef NANAKA_GUI_LAYOUTPROPERTIES_H
#define NANAKA_GUI_LAYOUTPROPERTIES_H

#include "math/Vec2f.h"

enum GUISizeUnit
{
	pxGUISizeUnit,
	dipGUISizeUnit,
};

class LayoutProperties final
{
public:

	LayoutProperties();

	// A widget's desire to grow will propagate to its parents, but a widget
	// will also fill its parent without propagating if it doesn't have a
	// desired size.
	bool GrowH() const;
	bool GrowV() const;
	bool FillH() const;
	bool FillV() const;

	Vec2f DesiredPixelSize(float dpi) const;
	float DesiredWidthAtHeight(float height) const;
	float DesiredHeightAtWidth(float width) const;

	float m_sizeWeight;
	float m_aspectRatio;
	Vec2f m_relPosition;
	Vec2f m_relSize;
	Vec2f m_desiredSize;
	GUISizeUnit m_desiredSizeUnit;
	bool m_growVertically;
	bool m_growHorizontally;
	int m_z;

	// Calculated by the layout algorithm.
	bool m_childGrowVertically;
	bool m_childGrowHorizontally;
	Vec2f m_desiredPixelSize;
};

inline bool LayoutProperties::GrowH() const
{
	return m_growHorizontally || m_childGrowHorizontally;
}

inline bool LayoutProperties::GrowV() const
{
	return m_growVertically|| m_childGrowVertically;
}

inline bool LayoutProperties::FillH() const
{
	return GrowH() || m_desiredPixelSize.x == 0.0f;
}

inline bool LayoutProperties::FillV() const
{
	return GrowV() || m_desiredPixelSize.y == 0.0f;
}

inline Vec2f LayoutProperties::DesiredPixelSize(float dpi) const
{
	return m_desiredSize * (m_desiredSizeUnit == dipGUISizeUnit ? dpi : 1.0f);
}

#endif // NANAKA_GUI_LAYOUTPROPERTIES_H
