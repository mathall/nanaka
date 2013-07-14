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

#ifndef NANAKA_GUI_BUTTON_H
#define NANAKA_GUI_BUTTON_H

#include <memory>

#include "gui/Widget.h"

class Panel;

class ButtonListener
{
public:

	virtual void ButtonPushed(UUID buttonId) = 0;
};

class Button final : public Widget
{
public:

	Button(
		std::shared_ptr<Panel> releasedState,
		std::shared_ptr<Panel> pressedState);

	void SetListener(ButtonListener* listener);

protected:

	/**
	 * Widget implementation.
	 */
	void OnReset() override;
	void OnPointerLost(int pointerId) override;
	void OnTouchDown(const InputEvent& event) override;
	void OnTouchUp(const InputEvent& event) override;
	void OnTouchIn(const InputEvent& event) override;
	void OnTouchOut(const InputEvent& event) override;

private:

	void SetPressed(bool pressed);

	ButtonListener* m_listener;
	std::shared_ptr<Panel> m_releasedState;
	std::shared_ptr<Panel> m_pressedState;
	int m_activePointerId;
};

inline void Button::SetListener(ButtonListener* listener)
{
	m_listener = listener;
}

#endif // NANAKA_GUI_BUTTON_H
