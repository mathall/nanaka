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

#include "renderer/RenderTarget.h"

#include "renderer/Renderer.h"

RenderTarget::RenderTarget(
	TargetType type,
	RenderTargetClient* client)
	: m_targetType(type)
	, m_client(client)
	, m_frameBuffer(std::make_shared<FrameBuffer>(this))
	, m_viewportRect(0.0f, 0.0f, 0.0f, 0.0f)
{
	if (m_targetType == OffScreen)
	{
		g_renderer->QueueGLResourceForBuild(m_frameBuffer);
	}
}

void RenderTarget::Setup() const
{
	switch (m_targetType)
	{
	case GUI:
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		break;

	case OffScreen:
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer->m_FBO);
		break;
	}

	glClearColor(128.0f / 255.0f, 168.0f / 255.0f, 248.0f / 255.0f, 1.0f);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glClearDepthf(1.0f);
	glDepthFunc(GL_LEQUAL);

	glViewport(
		m_viewportRect.x, m_viewportRect.y, m_viewportRect.w, m_viewportRect.h);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTarget::Finalize() const
{
	if (m_targetType == OffScreen)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void RenderTarget::UpdateViewport()
{
	Vec2f oldViewportSize(m_viewportRect.w, m_viewportRect.h);
	m_viewportRect = m_client->GetRect();
	Vec2f newViewportSize(m_viewportRect.w, m_viewportRect.h);
	if (m_targetType == OffScreen && !newViewportSize.Equals(oldViewportSize))
	{
		m_frameBuffer->SetSize(newViewportSize);
		g_renderer->QueueGLResourceForDestruction(m_frameBuffer);
		g_renderer->QueueGLResourceForBuild(m_frameBuffer);
	}
}
