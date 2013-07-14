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

#include "gui/Widget.h"

#include <algorithm>
#include <cassert>

Widget::Widget()
	: m_Id(UUID::New())
	, m_parent(NULL)
	, m_needLayout(true)
	, m_needRecalcLayoutProperties(true)
	, m_swallowsInput(false)
	, m_alpha(1.0f)
	, m_ancestorAlpha(1.0f)
	, m_visible(true)
	, m_ancestorsVisible(true)
{
}

void Widget::SetVisibleRecursively(bool visible, bool ancestorsVisible)
{
	m_visible = visible;
	m_ancestorsVisible = ancestorsVisible;

	for (auto child : m_children)
	{
		child->SetVisibleRecursively(
			child->m_visible, m_visible && m_ancestorsVisible);
	}

	PropagateDirty();
}

void Widget::SetViewActiveRecursively(bool active)
{
	m_viewActive = active;
	for (auto child : m_children)
	{
		child->SetViewActiveRecursively(active);
	}
}

void Widget::SetAlphaRecursively(float alpha, float ancestorAlpha)
{
	m_alpha = alpha;
	m_ancestorAlpha = ancestorAlpha;

	m_frame.SetAlpha(m_alpha * m_ancestorAlpha);
	OnAlphaUpdated(m_alpha * m_ancestorAlpha);

	for (auto child : m_children)
	{
		child->SetAlphaRecursively(child->m_alpha, m_alpha * m_ancestorAlpha);
	}
}

void Widget::RecalculateLayoutPropertiesRecursively(
	DisplayProperties displayProps, bool force)
{
	for (auto child : m_children)
	{
		child->RecalculateLayoutPropertiesIfNeeded(displayProps, force);
	}

	SetLayoutProperties(m_layoutAlgorithm.CalculateLayoutProperties(
		m_children, m_layoutProperties, displayProps));
	m_frame.SetDPI(displayProps.m_dpi);

	m_needRecalcLayoutProperties = false;
}

void Widget::Layout()
{
	Vec2f size(m_size.x - m_frame.LeftEdgeSize() - m_frame.RightEdgeSize(),
		m_size.y - m_frame.TopEdgeSize() - m_frame.BottomEdgeSize());
	Vec2f position(m_position.x + m_frame.LeftEdgeSize(),
		m_position.y + m_frame.TopEdgeSize());

	m_childrenPlacements.clear();
	m_layoutAlgorithm.CalculateChildrenPlacements(
		m_children, position, size, m_depthRange, m_childrenPlacements);

	for (auto child : m_children)
	{
		child->SetPlacement(m_childrenPlacements[child->GetId()]);
		child->LayoutIfNeeded();
	}

	SortChildrenByDepth();
	m_needLayout = false;
}

void Widget::SortChildrenByDepth()
{
	std::stable_sort(m_children.begin(), m_children.end(),
	[](const std::shared_ptr<Widget>& a, const std::shared_ptr<Widget>& b)
	{
		return a->m_depth < b->m_depth;
	});
}

void Widget::SetPlacement(WidgetPlacement placement)
{
	if (!m_position.Equals(placement.m_position)
		|| !m_size.Equals(placement.m_size)
		|| !m_depthRange.Equals(placement.m_depthRange)
		|| (m_depth != placement.m_depth))
	{
		PropagateDirty(LayoutDirtyModeLayoutOnly);
	}

	m_position = placement.m_position;
	m_size = placement.m_size;
	m_depthRange = placement.m_depthRange;
	m_depth = placement.m_depth;

	UpdateFrame();
	OnPlacementUpdated();
}

void Widget::DrawRecursively(RenderPipeline& renderPipeline) const
{
	if (!IsVisible())
	{
		return;
	}

	m_frame.Draw(renderPipeline);
	OnDraw(renderPipeline);

	for (auto child : m_children)
	{
		child->DrawRecursively(renderPipeline);
	}
}

void Widget::ResetRecursively()
{
	OnReset();
	for (auto child : m_children)
	{
		child->ResetRecursively();
	}
}

bool Widget::GetTouchedWidgets(std::vector<Widget*>& widgets, Vec2f touchPoint)
{
	bool inputSwallowed = false;

	if (IsVisible()
		&& m_position.x <= touchPoint.x
		&& touchPoint.x <= (m_position.x + m_size.x)
		&& m_position.y <= touchPoint.y
		&& touchPoint.y <= (m_position.y + m_size.y))
	{
		for (auto child : m_children)
		{
			inputSwallowed = child->GetTouchedWidgets(widgets, touchPoint);
			if (inputSwallowed)
			{
				break;
			}
		}

		if (!inputSwallowed)
		{
			widgets.push_back(this);
			inputSwallowed = m_swallowsInput;
		}
	}

	return inputSwallowed;
}

void Widget::PointerLostRecursively(int pointerId)
{
	OnPointerLost(pointerId);
	for (auto child : m_children)
	{
		child->PointerLostRecursively(pointerId);
	}
}

void Widget::AddWidget(std::shared_ptr<Widget> widget)
{
	assert(!widget->m_parent);
	widget->m_parent = this;
	m_children.push_back(widget);
	PropagateDirty();
}

void Widget::RemoveFromParent()
{
	if (m_parent)
	{
		auto thisId = GetId();
		m_parent->m_children.erase(
			std::remove_if(
				m_parent->m_children.begin(), m_parent->m_children.end(),
				[thisId](const std::shared_ptr<Widget>& widget)
					{ return widget->GetId() == thisId; }),
			m_parent->m_children.end());
		m_parent->PropagateDirty();
		m_parent = NULL;
	}
}

void Widget::SetLayoutProperties(LayoutProperties props)
{
	m_layoutProperties = props;
	PropagateDirty();
}

void Widget::PropagateDirty(LayoutDirtyMode dirtyMode)
{
	m_needRecalcLayoutProperties |= dirtyMode == LayoutDirtyModeAll;
	m_needLayout = true;
	if (m_parent)
	{
		m_parent->PropagateDirty(dirtyMode);
	}
}

void Widget::SetBackground(
	std::string texture,
	Vec4f color,
	ImageRotation rotation,
	bool flipH,
	bool flipV)
{
	m_frame.SetBackground(texture, color, rotation, flipH, flipV);
	UpdateFrame();
}

void Widget::SetBorder(
	std::string edgeTex,
	std::string cornerTex,
	Vec4f color,
	float edgeSize,
	GUISizeUnit edgeSizeUnit,
	FrameBorderEdges edges)
{
	m_frame.SetBorders(
		edgeTex, cornerTex, color, edgeSize, edgeSizeUnit, edges);
	UpdateFrame();
}

void Widget::SetLayoutMethod(LayoutMethod method)
{
	m_layoutAlgorithm.m_layoutMethod = method;
	PropagateDirty();
}
