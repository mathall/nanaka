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

#ifndef NANAKA_GUI_LAYOUTMETHOD_H
#define NANAKA_GUI_LAYOUTMETHOD_H

class LayoutMethod final
{
public:

	enum LayoutAlgorithm
	{
		Linear,
		Relative,
	};

	enum Orientation
	{
		Horizontal,
		Vertical,
	};

	enum HAlignment
	{
		Left,
		HCenter,
		Right,
		HFillout,
	};

	enum VAlignment
	{
		Top,
		VCenter,
		Bottom,
		VFillout,
	};

	LayoutMethod(
		LayoutAlgorithm layoutAlgorithm = Relative,
		Orientation orientation = Vertical,
		HAlignment halign = HCenter,
		VAlignment valign = VCenter);

	void SetRelativeLayout();
	void SetLinearLayout(
		Orientation orientation = Vertical,
		HAlignment halign = HCenter,
		VAlignment valign = VCenter);
	void SetOrientation(Orientation orientation);
	void SetAlignment(HAlignment halign, VAlignment valign);

	LayoutAlgorithm m_layoutAlgorithm;
	Orientation m_orientation;
	HAlignment m_HAlignment;
	VAlignment m_VAlignment;
};

inline LayoutMethod::LayoutMethod(
	LayoutAlgorithm layoutAlgorithm,
	Orientation orientation,
	HAlignment halign,
	VAlignment valign)
	: m_layoutAlgorithm(layoutAlgorithm)
	, m_orientation(orientation)
	, m_HAlignment(halign)
	, m_VAlignment(valign)
{
}

inline void LayoutMethod::SetLinearLayout(
	Orientation orientation,
	HAlignment halign,
	VAlignment valign)
{
	m_layoutAlgorithm = Linear;
	m_orientation = orientation;
	m_HAlignment = halign;
	m_VAlignment = valign;
}

inline void LayoutMethod::SetRelativeLayout()
{
	m_layoutAlgorithm = Relative;
}

inline void LayoutMethod::SetOrientation(Orientation orientation)
{
	m_orientation = orientation;
}

inline void LayoutMethod::SetAlignment(HAlignment halign, VAlignment valign)
{
	m_HAlignment = halign;
	m_VAlignment = valign;
}

#endif // NANAKA_GUI_WIDGETMETHOD_H
