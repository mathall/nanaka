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

#include "renderer/FrameBufferRenderResource.h"
#include "renderer/RenderResourceManager.h"

RenderTarget::RenderTarget(RenderTargetType type)
	: m_frameBufferHandle(RenderResourceHandle::Invalid)
	, m_type(type)
	, m_viewportRect(0.0f, 0.0f, 0.0f, 0.0f)
{
}

bool RenderTarget::IsValid(
	const RenderResourceManager& renderResourceManager) const
{
	return m_viewportRect.w > 0.0f && m_viewportRect.h > 0.0f
		&& (IsOnScreen()
			|| renderResourceManager.Get<FrameBufferRenderResource>(
				m_frameBufferHandle));
}

void RenderTarget::Setup(
	const RenderResourceManager& renderResourceManager) const
{
	if (IsOnScreen())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
	{
		if (auto frameBuffer =
			renderResourceManager.Get<FrameBufferRenderResource>(
				m_frameBufferHandle))
		{
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->m_FBO);
		}
		else
		{
			assert(!"No frame buffer to bind.");
		}
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
	if (!IsOnScreen())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
