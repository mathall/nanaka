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

#include "gui/Frame.h"

Frame::Frame()
	: m_edges(NoEdges)
	, m_edgeSize(0.0f)
	, m_edgeSizeUnit(pxGUISizeUnit)
	, m_dpi(1.0f)
	, m_topLeft("", Vec4f::Zero(), ImageRotationNone)
	, m_topRight("", Vec4f::Zero(), ImageRotation90Deg)
	, m_bottomRight("", Vec4f::Zero(), ImageRotation180Deg)
	, m_bottomLeft("", Vec4f::Zero(), ImageRotation270Deg)
	, m_top("", Vec4f::Zero(), ImageRotationNone)
	, m_right("", Vec4f::Zero(), ImageRotation90Deg)
	, m_bottom("", Vec4f::Zero(), ImageRotation180Deg)
	, m_left("", Vec4f::Zero(), ImageRotation270Deg)
	, m_background("", Vec4f::Zero(), ImageRotationNone)
	, m_hasBackground(false)
{
}

void Frame::SetBackground(
	std::string texture,
	Vec4f color,
	ImageRotation rotation,
	bool flipH,
	bool flipV)
{
	m_background = ImageComponent(texture, color, rotation, flipH, flipV);
	m_hasBackground = true;
}

void Frame::SetBorders(
	std::string edgeTex,
	std::string cornerTex,
	Vec4f color,
	float edgeSize,
	GUISizeUnit edgeSizeUnit,
	int edges)
{
	m_edges = edges;
	m_edgeSize = edgeSize;
	m_edgeSizeUnit = edgeSizeUnit;
	m_topLeft = ImageComponent(cornerTex, color, ImageRotationNone);
	m_topRight = ImageComponent(cornerTex, color, ImageRotation90Deg);
	m_bottomRight = ImageComponent(cornerTex, color, ImageRotation180Deg);
	m_bottomLeft = ImageComponent(cornerTex, color, ImageRotation270Deg);
	m_top = ImageComponent(edgeTex, color, ImageRotationNone);
	m_right = ImageComponent(edgeTex, color, ImageRotation90Deg);
	m_bottom = ImageComponent(edgeTex, color, ImageRotation180Deg);
	m_left = ImageComponent(edgeTex, color, ImageRotation270Deg);
}

void Frame::SetPlacement(Vec2f position, Vec2f size, float depth)
{
	if (m_hasBackground)
	{
		Vec2f backgroundOffset(LeftEdgeSize(), TopEdgeSize());
		Vec2f backgroundSize(
			size.x - LeftEdgeSize() - RightEdgeSize(),
			size.y - TopEdgeSize() - BottomEdgeSize());
		m_background.SetPlacement(
			position + backgroundOffset, backgroundSize, depth);
	}

	if (m_edges == NoEdges)
	{
		return;
	}

	Vec2f cornerSize(EdgeSize(), EdgeSize());
	Vec2f bottomCornerOffset(0.0f, size.y - BottomEdgeSize());
	Vec2f rightCornerOffset(size.x - RightEdgeSize(), 0.0f);
	Vec2f bottomRightCornerOffset = bottomCornerOffset + rightCornerOffset;

	m_bottomLeft.SetPlacement(
		position + bottomCornerOffset, cornerSize, depth);
	m_bottomRight.SetPlacement(
		position + bottomRightCornerOffset, cornerSize, depth);
	m_topLeft.SetPlacement(
		position, cornerSize, depth);
	m_topRight.SetPlacement(
		position + rightCornerOffset, cornerSize, depth);

	Vec2f topEdgeSize(
		size.x - LeftEdgeSize() - RightEdgeSize(), TopEdgeSize());
	Vec2f bottomEdgeSize(
		size.x - LeftEdgeSize() - RightEdgeSize(), BottomEdgeSize());
	Vec2f leftEdgeSize(
		LeftEdgeSize(), size.y - TopEdgeSize() - BottomEdgeSize());
	Vec2f rightEdgeSize(
		RightEdgeSize(), size.y - TopEdgeSize() - BottomEdgeSize());

	Vec2f topEdgeOffset(LeftEdgeSize(), 0.0f);
	Vec2f rightEdgeOffset(size.x - RightEdgeSize(), TopEdgeSize());
	Vec2f leftEdgeOffset(0.0f, TopEdgeSize());
	Vec2f bottomEdgeOffset(LeftEdgeSize(), size.y - BottomEdgeSize());

	m_bottom.SetPlacement(position + bottomEdgeOffset, bottomEdgeSize, depth);
	m_top.SetPlacement(position + topEdgeOffset, topEdgeSize, depth);
	m_left.SetPlacement(position + leftEdgeOffset, leftEdgeSize, depth);
	m_right.SetPlacement(position + rightEdgeOffset, rightEdgeSize, depth);
}

void Frame::SetAlpha(float alpha)
{
	m_bottomLeft.SetAlpha(alpha);
	m_bottomRight.SetAlpha(alpha);
	m_topLeft.SetAlpha(alpha);
	m_topRight.SetAlpha(alpha);
	m_bottom.SetAlpha(alpha);
	m_top.SetAlpha(alpha);
	m_left.SetAlpha(alpha);
	m_right.SetAlpha(alpha);
	m_background.SetAlpha(alpha);
}

void Frame::Draw(RenderData& renderData) const
{
	if (m_hasBackground)
	{
		m_background.Draw(renderData);
	}

	if (m_edges == NoEdges)
	{
		return;
	}

	if (m_edges & TopEdge)
	{
		if (m_edges & LeftEdge)
		{
			m_topLeft.Draw(renderData);
		}

		m_top.Draw(renderData);

		if (m_edges & RightEdge)
		{
			m_topRight.Draw(renderData);
		}
	}

	if (m_edges & LeftEdge)
	{
		m_left.Draw(renderData);
	}

	if (m_edges & RightEdge)
	{
		m_right.Draw(renderData);
	}

	if (m_edges & BottomEdge)
	{
		if (m_edges & LeftEdge)
		{
			m_bottomLeft.Draw(renderData);
		}

		m_bottom.Draw(renderData);

		if (m_edges & RightEdge)
		{
			m_bottomRight.Draw(renderData);
		}
	}
}
