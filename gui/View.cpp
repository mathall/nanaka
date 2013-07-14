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

#include "gui/View.h"

#include <algorithm>

#include "gui/GUI.h"
#include "gui/Widget.h"
#include "input/InputEvent.h"

View::View()
	: m_Id(UUID::New())
{
	m_contentPlacement.m_position = Vec2f::Zero();
	m_contentPlacement.m_depthRange = Vec2f(1000.0f, 0.0f);
	m_contentPlacement.m_depth = 1000.0f;

	m_lastDisplayProps.m_realSize = Vec2f::Zero();
	m_lastDisplayProps.m_size = Vec2f::Zero();
	m_lastDisplayProps.m_dpi = 0.0f;
}

void View::EnsureLayout(DisplayProperties displayProps)
{
	bool forceRecalc = displayProps.m_dpi != m_lastDisplayProps.m_dpi;
	m_contentPlacement.m_size = displayProps.m_size;
	m_content.SetPlacement(m_contentPlacement);
	m_content.RecalculateLayoutPropertiesIfNeeded(displayProps, forceRecalc);
	m_content.LayoutIfNeeded();
	m_lastDisplayProps = displayProps;
}

void View::SetContent(std::shared_ptr<Widget> content)
{
	m_content.m_children.clear();
	m_content.AddWidget(content);
}

void View::HandleEvent(const InputEvent& event)
{
	switch (event.m_type)
	{
	case MotionInputEventType:
		m_touchedWidgets.clear();
		m_content.GetTouchedWidgets(m_touchedWidgets, event.m_position);
		std::sort(m_touchedWidgets.begin(), m_touchedWidgets.end());

		const int pointerId = event.m_pointerId;
		switch (event.m_action)
		{
		case PointerDownInputEventAction:
			for (auto touchedWidget : m_touchedWidgets)
			{
				touchedWidget->OnTouchDown(event);
			}
			m_trackedPointers[pointerId] = true;
			break;

		case PointerUpInputEventAction:
			for(auto touchedWidget : m_touchedWidgets)
			{
				touchedWidget->OnTouchUp(event);
			}
			m_trackedPointers[pointerId] = false;
			m_content.PointerLostRecursively(pointerId);
			break;

		case PointerMoveInputEventAction:
			if (m_trackedPointers[pointerId])
			{
				m_touchInWidgets.clear();
				m_touchOutWidgets.clear();
				m_touchMoveWidgets.clear();

				std::set_intersection(
					m_lastEventTouchedWidgets[pointerId].begin(),
					m_lastEventTouchedWidgets[pointerId].end(),
					m_touchedWidgets.begin(), m_touchedWidgets.end(),
					std::back_inserter(m_touchMoveWidgets));

				std::set_difference(
					m_lastEventTouchedWidgets[pointerId].begin(),
					m_lastEventTouchedWidgets[pointerId].end(),
					m_touchMoveWidgets.begin(), m_touchMoveWidgets.end(),
					std::back_inserter(m_touchOutWidgets));

				std::set_difference(
					m_touchedWidgets.begin(), m_touchedWidgets.end(),
					m_touchMoveWidgets.begin(), m_touchMoveWidgets.end(),
					std::back_inserter(m_touchInWidgets));

				for (auto widget : m_touchInWidgets)
				{
					widget->OnTouchIn(event);
				}

				for (auto widget : m_touchOutWidgets)
				{
					widget->OnTouchOut(event);
				}

				for (auto widget : m_touchMoveWidgets)
				{
					widget->OnTouchMove(event);
				}
			}
			break;
		}

		m_lastEventTouchedWidgets[pointerId].clear();
		m_lastEventTouchedWidgets[pointerId].reserve(m_touchedWidgets.size());
		std::move(m_touchedWidgets.begin(), m_touchedWidgets.end(),
			std::back_inserter(m_lastEventTouchedWidgets[pointerId]));
		break;
	}
}

void View::Reset()
{
	for (int i = 0; i < s_maxTrackedPointers; i++)
	{
		if (m_trackedPointers[i])
		{
			m_trackedPointers[i] = false;
			m_content.PointerLostRecursively(i);
		}
	}
	m_content.ResetRecursively();
}
