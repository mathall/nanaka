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

#include "gui/Button.h"

#include "gui/Panel.h"
#include "input/InputEvent.h"

Button::Button(
	std::shared_ptr<Panel> releasedState,
	std::shared_ptr<Panel> pressedState)
	: m_listener(NULL)
	, m_releasedState(releasedState)
	, m_pressedState(pressedState)
	, m_activePointerId(-1)
{
	AddWidget(m_releasedState);
	AddWidget(m_pressedState);

	SetSwallowsInput(true);
}

void Button::OnReset()
{
	SetPressed(false);
	m_activePointerId = -1;
}

void Button::OnPointerLost(int pointerId)
{
	if (pointerId == m_activePointerId)
	{
		m_activePointerId = -1;
	}
}

void Button::OnTouchDown(const InputEvent& event)
{
	if (m_activePointerId == -1)
	{
		m_activePointerId = event.m_pointerId;
		SetPressed(true);
	}
}

void Button::OnTouchUp(const InputEvent& event)
{
	if (m_activePointerId == event.m_pointerId)
	{
		if (m_listener)
		{
			m_listener->ButtonPushed(GetId());
		}
		SetPressed(false);
	}
}

void Button::OnTouchIn(const InputEvent& event)
{
	if (m_activePointerId == event.m_pointerId)
	{
		SetPressed(true);
	}
}

void Button::OnTouchOut(const InputEvent& event)
{
	if (m_activePointerId == event.m_pointerId)
	{
		SetPressed(false);
	}
}

void Button::SetPressed(bool pressed)
{
	if (pressed)
	{
		m_pressedState->SetVisible(true);
		m_releasedState->SetVisible(false);
	}
	else
	{
		m_pressedState->SetVisible(false);
		m_releasedState->SetVisible(true);
	}
}
