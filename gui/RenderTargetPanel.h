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

#include "graphics/FrameBuffer.h"
#include "graphics/Model.h"
#include "gui/Panel.h"
#include "utils/UUID.h"

class RenderTargetPanelListener
{
public:

	virtual ~RenderTargetPanelListener(){}

	virtual void OnNewSize(UUID panelId) = 0;
};

class RenderTargetPanel final : public Panel
{
public:

	RenderTargetPanel();
	~RenderTargetPanel();

	RenderTargetPanel(RenderTargetPanel&) = delete;
	RenderTargetPanel& operator=(const RenderTargetPanel&) = delete;

	void SetRenderTargetPanelListener(RenderTargetPanelListener* listener);
	float GetAspectRatio() const;
	UUID GetRenderContextId() const;

protected:

	/**
	 * Widget implementation.
	 */
	void OnPlacementUpdated() override;
	void OnDraw(RenderData& renderData) const override;

private:

	UUID m_renderContextId;
	RenderTargetPanelListener* m_listener;
	Vec2f m_lastSize;

	std::unique_ptr<FrameBuffer> m_frameBuffer;
	Model m_targetBillboard;
};

inline RenderTargetPanel::~RenderTargetPanel()
{
	if (g_renderer)
	{
		g_renderer->DestroyRenderContext(m_renderContextId);
	}
}

inline float RenderTargetPanel::GetAspectRatio() const
{
	return m_size.y == 0.0f ? 1.0f : (m_size.x / m_size.y);
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
