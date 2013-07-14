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

#include "gui/DPad.h"

#include "gui/Image.h"
#include "input/InputEvent.h"

DPad::DPad(std::string backgroundTex, Vec4f color)
	: m_listener(NULL)
	, m_activePointerId(-1)
{
	AddWidget(std::make_shared<Image>(
		backgroundTex, color, Vec2f(0.0f, 0.0f), pxGUISizeUnit));

	SetSwallowsInput(true);
}

void DPad::OnPointerLost(int pointerId)
{
	if (pointerId == m_activePointerId)
	{
		DispatchInput(m_lastEventPosition, DPadActionRelease);
		m_activePointerId = -1;
	}
}

void DPad::OnTouchDown(const InputEvent& event)
{
	if (m_activePointerId == -1)
	{
		DispatchInput(event.m_position, DPadActionGrab);
		m_activePointerId = event.m_pointerId;
	}
}

void DPad::OnTouchUp(const InputEvent& event)
{
	if (m_activePointerId == event.m_pointerId)
	{
		DispatchInput(event.m_position, DPadActionRelease);
		m_activePointerId = -1;
	}
}

void DPad::OnTouchMove(const InputEvent& event)
{
	if (m_activePointerId == event.m_pointerId)
	{
		DispatchInput(event.m_position, DPadActionMove);
	}
}

void DPad::DispatchInput(Vec2f eventPosition, DPadAction action)
{
	if (m_listener != NULL)
	{
		m_listener->DPadInput(
			GetId(), action, (eventPosition - m_position) / m_size);
	}
	m_lastEventPosition = eventPosition;
}
