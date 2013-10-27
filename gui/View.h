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

#ifndef NANAKA_GUI_VIEW_H
#define NANAKA_GUI_VIEW_H

#include <memory>
#include <vector>
#include <array>

#include "gui/DisplayProperties.h"
#include "gui/Widget.h"
#include "input/InputEvent.h"
#include "utils/UUID.h"

class RenderPipeline;

class View final
{
public:

	View();

	void SetContent(std::shared_ptr<Widget> content);
	void EnsureLayout(DisplayProperties displayProps);
	void Draw(RenderPipeline& renderPipeline) const;
	void HandleEvent(const InputEvent& event);
	void Reset();
	UUID GetId() const;

private:

	UUID m_Id;

	WidgetPlacement m_contentPlacement;
	Widget m_content;
	DisplayProperties m_lastDisplayProps;

	static const int s_maxTrackedPointers = 10;
	std::array<bool, s_maxTrackedPointers> m_trackedPointers;
	std::array<std::vector<Widget*>, s_maxTrackedPointers>
		m_lastEventTouchedWidgets;

	// Kept as members to avoid unnecessary heap allocations.
	std::vector<Widget*> m_touchedWidgets;
	std::vector<Widget*> m_touchInWidgets;
	std::vector<Widget*> m_touchOutWidgets;
	std::vector<Widget*> m_touchMoveWidgets;
};

inline void View::Draw(RenderPipeline& renderPipeline) const
{
	m_content.DrawRecursively(renderPipeline);
}

inline UUID View::GetId() const
{
	return m_Id;
}

#endif // NANAKA_GUI_VIEW_H
