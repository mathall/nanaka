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

#include "gui/RenderTargetPanel.h"

#include "math/Rect.h"
#include "renderer/Renderer.h"
#include "renderer/RenderTarget.h"

RenderTargetPanel::RenderTargetPanel()
	: m_renderContextId(UUID::Invalid)
	, m_listener(NULL)
	, m_lastSize(Vec2f::Zero())
	, m_targetBillboard("nanaka/models/gui_billboard.nmdl")
{
	m_renderContextId =
		g_renderer->GenerateRenderContext(OffScreenRenderTargetType);
}

void RenderTargetPanel::OnPlacementUpdated()
{
	m_targetBillboard.SetPosition(Vec3f(
		m_position.x + m_size.x * 0.5f,
		m_position.y + m_size.y * 0.5f,
		m_depth));
	m_targetBillboard.SetScale(Vec3f(m_size.x, m_size.y, 1.0f));

	if (m_size.Equals(m_lastSize))
	{
		return;
	}

	m_lastSize = m_size;

	g_renderer->SetViewportRect(m_renderContextId, Rect(Vec2f::Zero(), m_size));

	m_frameBuffer = std::unique_ptr<FrameBuffer>(new FrameBuffer(m_size));

	g_renderer->SetFrameBufferHandle(
		m_renderContextId, m_frameBuffer->GetFrameBufferHandle());

	auto material = m_targetBillboard.GetMaterial();
	material.SetTexture(Texture(m_frameBuffer->GetFrameBufferHandle()));
	m_targetBillboard.SetMaterial(material);

	if (m_listener != NULL)
	{
		m_listener->OnNewSize(GetId());
	}
}

void RenderTargetPanel::OnDraw(RenderData& renderData) const
{
	auto RE = renderData.GetRenderElement();
	m_targetBillboard.PrepRender(*RE);
	renderData.QueueRenderElement(std::move(RE));
}
