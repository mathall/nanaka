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

#ifndef NANAKA_GUI_GUI_H
#define NANAKA_GUI_GUI_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "gui/DisplayProperties.h"
#include "gui/Font.h"
#include "gui/View.h"
#include "input/InputListener.h"
#include "utils/UUID.h"

class InputEvent;
class Widget;

/**
 * GUI is a manager of Views and Fonts. One View may be active at a time, and
 * that View will receive all input events sent to the GUI.
 */
class GUI final : public InputListener
{
public:

	GUI();

	void Initialize();
	void SetDisplayProperties(DisplayProperties displayProps);
	void Draw();

	UUID CreateView();
	void SetViewContent(UUID viewId, std::shared_ptr<Widget> content);
	void SetActiveView(UUID viewId);

	UUID RegisterFont(std::string fontFilePath);
	const Font* GetFont(UUID fontId) const;

	/**
	 * InputListener implementation.
	 */
	void HandleEvent(const InputEvent& event) override;

private:


	UUID m_renderContextId;
	std::unordered_map<UUID, std::unique_ptr<Font>> m_fonts;
	std::unordered_map<UUID, std::unique_ptr<View>> m_views;
	View* m_activeView;
	DisplayProperties m_displayProperties;
};

inline void GUI::SetDisplayProperties(DisplayProperties displayProps)
{
	m_displayProperties = displayProps;
	g_renderer->SetViewportRect(m_renderContextId,
		Rect(Vec2f::Zero(), m_displayProperties.m_realSize));
}

inline const Font* GUI::GetFont(UUID fontId) const
{
	return m_fonts.at(fontId).get();
}

#endif // NANAKA_GUI_GUI_H
