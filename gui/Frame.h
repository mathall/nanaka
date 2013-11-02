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

#ifndef NANAKA_GUI_FRAME_H
#define NANAKA_GUI_FRAME_H

#include "gui/ImageComponent.h"
#include "gui/LayoutProperties.h"

typedef int FrameBorderEdges;

class Frame final
{
public:

	enum BorderEdgesMasks
	{
		TopEdge    = 0x0000000F,
		BottomEdge = 0x000000F0,
		LeftEdge   = 0x00000F00,
		RightEdge  = 0x0000F000,
		NoEdges    = 0x00000000,
		AllEdges   = 0x0000FFFF,
	};

	Frame();

	void SetAlpha(float alpha);
	void SetPlacement(Vec2f position, Vec2f size, float depth);
	void SetBackground(
		std::string texture,
		Vec4f color,
		ImageRotation rotation = ImageRotationNone,
		bool flipH = false,
		bool flipV = false);
	void SetBorders(
		std::string edgeTex,
		std::string cornerTex,
		Vec4f color,
		float edgeSize,
		GUISizeUnit edgeSizeUnit,
		FrameBorderEdges edges);
	void SetDPI(float dpi);

	void Draw(RenderData& renderData) const;

	float TopEdgeSize() const;
	float BottomEdgeSize() const;
	float LeftEdgeSize() const;
	float RightEdgeSize() const;
	float EdgeSize() const;

private:

	FrameBorderEdges m_edges;
	float m_edgeSize;
	GUISizeUnit m_edgeSizeUnit;
	float m_dpi;

	ImageComponent m_topLeft;
	ImageComponent m_topRight;
	ImageComponent m_bottomRight;
	ImageComponent m_bottomLeft;

	ImageComponent m_top;
	ImageComponent m_right;
	ImageComponent m_bottom;
	ImageComponent m_left;

	ImageComponent m_background;
	bool m_hasBackground;
};

inline void Frame::SetDPI(float dpi)
{
	m_dpi = dpi;
}

inline float Frame::TopEdgeSize() const
{
	return (m_edges & TopEdge) ? EdgeSize() : 0.0f;
}

inline float Frame::BottomEdgeSize() const
{
	return (m_edges & BottomEdge) ? EdgeSize() : 0.0f;
}

inline float Frame::LeftEdgeSize() const
{
	return (m_edges & LeftEdge) ? EdgeSize() : 0.0f;
}

inline float Frame::RightEdgeSize() const
{
	return (m_edges & RightEdge) ? EdgeSize() : 0.0f;
}

inline float Frame::EdgeSize() const
{
	return m_edgeSize * (m_edgeSizeUnit == dipGUISizeUnit ? m_dpi : 1.0f);
}

#endif // NANAKA_GUI_FRAME_H
