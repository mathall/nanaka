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

#ifndef NANAKA_GUI_WIDGET_H
#define NANAKA_GUI_WIDGET_H

#include <memory>
#include <vector>

#include "gui/DisplayProperties.h"
#include "gui/Frame.h"
#include "gui/LayoutAlgorithm.h"
#include "gui/LayoutMethod.h"
#include "gui/LayoutProperties.h"
#include "input/InputEvent.h"
#include "math/Vec4f.h"
#include "utils/UUID.h"

class RenderPipeline;

enum LayoutDirtyMode
{
	LayoutDirtyModeAll,
	LayoutDirtyModeLayoutOnly,
};

class Widget
{
public:

	Widget();
	virtual ~Widget(){}

	UUID GetId() const;
	void SetSwallowsInput(bool swallowsInput);
	void SetAlpha(float alpha);
	void SetVisible(bool visible);
	bool IsVisible() const;

	const LayoutProperties& GetLayoutPropertiesConstRef() const;
	LayoutProperties GetLayoutProperties() const;
	void SetLayoutProperties(LayoutProperties props);

	void RemoveFromParent();

protected:

	virtual void OnPlacementUpdated(){}
	virtual void OnAlphaUpdated(float totalAlpha){}
	virtual void OnDraw(RenderPipeline& renderPipeline) const{}
	virtual void OnReset(){}

	virtual void OnPointerLost(int pointerId){}
	virtual void OnTouchDown(const InputEvent& event){}
	virtual void OnTouchUp(const InputEvent& event){}
	virtual void OnTouchIn(const InputEvent& event){}
	virtual void OnTouchOut(const InputEvent& event){}
	virtual void OnTouchMove(const InputEvent& event){}

	void AddWidget(std::shared_ptr<Widget> widget);
	void SetLayoutMethod(LayoutMethod method);
	void SetBackground(
		std::string texture,
		Vec4f color,
		ImageRotation rotation = ImageRotationNone,
		bool flipH = false,
		bool flipV = false);
	void SetBorder(
		std::string edgeTex,
		std::string cornerTex,
		Vec4f color,
		float edgeSize,
		GUISizeUnit edgeSizeUnit,
		FrameBorderEdges edges);

	void PropagateDirty(LayoutDirtyMode dirtyMode = LayoutDirtyModeAll);

	Vec2f m_position;
	Vec2f m_size;
	Vec2f m_depthRange;
	float m_depth;

private:

	// View controlls the root widget and calls on several private methods.
	friend class View;

	void SetAlphaRecursively(float alpha, float ancestorAlpha);
	void SetVisibleRecursively(bool visible, bool ancestorsVisible);
	void PointerLostRecursively(int pointerId);
	void DrawRecursively(RenderPipeline& renderPipeline) const;
	void ResetRecursively();

	void RecalculateLayoutPropertiesIfNeeded(
		DisplayProperties displayProps, bool force);
	void RecalculateLayoutPropertiesRecursively(
		DisplayProperties displayProps, bool force);
	void LayoutIfNeeded();
	void Layout();
	void SetPlacement(WidgetPlacement placement);
	void SortChildrenByDepth();
	void UpdateFrame();

	bool GetTouchedWidgets(std::vector<Widget*>& widgets, Vec2f touchPoint);

	UUID m_Id;
	Widget* m_parent;
	std::vector<std::shared_ptr<Widget>> m_children;
	LayoutAlgorithm m_layoutAlgorithm;
	LayoutProperties m_layoutProperties;
	Frame m_frame;
	bool m_needLayout;
	bool m_needRecalcLayoutProperties;
	bool m_swallowsInput;
	float m_alpha;
	float m_ancestorAlpha;
	bool m_visible;
	bool m_ancestorsVisible;

	// Kept as members to avoid unnecessary heap allocations.
	std::unordered_map<UUID, WidgetPlacement> m_childrenPlacements;
};

inline UUID Widget::GetId() const
{
	return m_Id;
}

inline const LayoutProperties& Widget::GetLayoutPropertiesConstRef() const
{
	return m_layoutProperties;
}

inline LayoutProperties Widget::GetLayoutProperties() const
{
	return m_layoutProperties;
}

inline void Widget::SetVisible(bool visible)
{
	if (visible != m_visible)
	{
		SetVisibleRecursively(visible, m_ancestorsVisible);
	}
}

inline bool Widget::IsVisible() const
{
	return m_visible && m_ancestorsVisible;
}

inline void Widget::SetAlpha(float alpha)
{
	SetAlphaRecursively(alpha, m_ancestorAlpha);
}

inline void Widget::SetSwallowsInput(bool swallowsInput)
{
	m_swallowsInput = swallowsInput;
}

inline void Widget::UpdateFrame()
{
	m_frame.SetPlacement(m_position, m_size, m_depth);
}

inline void Widget::RecalculateLayoutPropertiesIfNeeded(
	DisplayProperties displayProps, bool force)
{
	if (m_needRecalcLayoutProperties || force)
	{
		RecalculateLayoutPropertiesRecursively(displayProps, force);
	}
}

inline void Widget::LayoutIfNeeded()
{
	if (m_needLayout)
	{
		Layout();
	}
}

#endif // NANAKA_GUI_WIDGET_H
