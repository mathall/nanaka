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

#ifndef NANAKA_GUI_PANEL_H
#define NANAKA_GUI_PANEL_H

#include "gui/Widget.h"

class PanelListener
{
public:

	virtual ~PanelListener(){}

	virtual void PanelInput(UUID panelId, const InputEvent& event) = 0;
};

class Panel : public Widget
{
public:

	Panel();
	virtual ~Panel(){}

	void AddWidget(std::shared_ptr<Widget> widget);
	void SetLayoutMethod(LayoutMethod layoutMethod);
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

	void SetListener(PanelListener* listener);

protected:

	/**
	 * Widget implementation.
	 */
	void OnTouchDown(const InputEvent& event) override;
	void OnTouchUp(const InputEvent& event) override;
	void OnTouchIn(const InputEvent& event) override;
	void OnTouchOut(const InputEvent& event) override;
	void OnTouchMove(const InputEvent& event) override;

private:

	PanelListener* m_listener;
};

inline Panel::Panel()
	: m_listener(NULL)
{
	LayoutProperties layoutProperties;
	layoutProperties.m_growHorizontally = true;
	layoutProperties.m_growVertically = true;
	SetLayoutProperties(layoutProperties);
}

inline void Panel::AddWidget(std::shared_ptr<Widget> widget)
{
	Widget::AddWidget(widget);
}

inline void Panel::SetLayoutMethod(LayoutMethod layoutMethod)
{
	Widget::SetLayoutMethod(layoutMethod);
}

inline void Panel::SetBackground(
	std::string texture,
	Vec4f color,
	ImageRotation rotation,
	bool flipH,
	bool flipV)
{
	Widget::SetBackground(texture, color, rotation, flipH, flipV);
}

inline void Panel::SetBorder(
	std::string edgeTex,
	std::string cornerTex,
	Vec4f color,
	float edgeSize,
	GUISizeUnit edgeSizeUnit,
	FrameBorderEdges edges)
{
	Widget::SetBorder(edgeTex, cornerTex, color, edgeSize, edgeSizeUnit, edges);
}

inline void Panel::SetListener(PanelListener* listener)
{
	m_listener = listener;
}

inline void Panel::OnTouchDown(const InputEvent& event)
{
	if (m_listener != NULL)
	{
		m_listener->PanelInput(GetId(), event);
	}
}

inline void Panel::OnTouchUp(const InputEvent& event)
{
	if (m_listener != NULL)
	{
		m_listener->PanelInput(GetId(), event);
	}
}

inline void Panel::OnTouchIn(const InputEvent& event)
{
	if (m_listener != NULL)
	{
		m_listener->PanelInput(GetId(), event);
	}
}

inline void Panel::OnTouchOut(const InputEvent& event)
{
	if (m_listener != NULL)
	{
		m_listener->PanelInput(GetId(), event);
	}
}

inline void Panel::OnTouchMove(const InputEvent& event)
{
	if (m_listener != NULL)
	{
		m_listener->PanelInput(GetId(), event);
	}
}

#endif // NANAKA_GUI_PANEL_H
