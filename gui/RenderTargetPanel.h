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

#ifndef NANAKA_GUI_RENDERTARGETPANEL_H
#define NANAKA_GUI_RENDERTARGETPANEL_H

#include "graphics/Model.h"
#include "gui/Panel.h"
#include "renderer/RenderTarget.h"
#include "utils/UUID.h"

class RenderTargetPanelListener
{
public:

	virtual void OnNewSize(UUID panelId) = 0;
};

class RenderTargetPanel final : public Panel, public RenderTargetClient
{
public:

	RenderTargetPanel();

	void SetRenderTargetPanelListener(RenderTargetPanelListener* listener);
	float GetAspectRatio() const;
	UUID GetRenderContextId() const;

	/**
	 * RenderTargetClient implementation.
	 */
	bool IsActive() const override;
	Rect GetRect() const override;
	void NewColorBuffer(GLuint colorBuffer) override;

protected:

	/**
	 * Widget implementation.
	 */
	void OnPlacementUpdated() override;
	void OnDraw(RenderPipeline& renderPipeline) const override;

private:

	UUID m_renderContextId;
	RenderTargetPanelListener* m_listener;
	Vec2f m_lastSize;

	Model m_targetBillboard;
};

inline float RenderTargetPanel::GetAspectRatio() const
{
	return m_size.y == 0.0f ? 1.0f : (m_size.x / m_size.y);
}

inline Rect RenderTargetPanel::GetRect() const
{
	return Rect(Vec2f(0.0f, 0.0f), m_size);
}

inline bool RenderTargetPanel::IsActive() const
{
	return IsVisible() && IsViewActive();
}

inline UUID RenderTargetPanel::GetRenderContextId() const
{
	return m_renderContextId;
}

inline void RenderTargetPanel::SetRenderTargetPanelListener(
	RenderTargetPanelListener* listener)
{
	m_listener = listener;
}

#endif // NANAKA_GUI_RENDERTARGETPANEL_H
