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

#include "renderer/Renderer.h"

#include <algorithm>

Renderer::Renderer()
	: m_killThreadRequested(false)
	, m_contextLost(true)
	, m_contextChanged(true)
	, m_GLContextManager(GLContextManager::Create())
	, m_nativeWindow(NULL)
	, m_contextLock(0)
	, m_runPermit(0)
{
}

void Renderer::OnWindowCreated(NanakaNativeWindow* nativeWindow)
{
	ScopedMonitorLock lock(this);

	m_nativeWindow = nativeWindow;
	m_contextLost = true;
	m_contextChanged = true;
	Signal(m_contextLock);
}

void Renderer::OnWindowDestroyed()
{
	ScopedMonitorLock lock(this);

	WaitFor(m_contextLock);
	m_nativeWindow = NULL;
}

void Renderer::OnWindowChanged()
{
	ScopedMonitorLock lock(this);

	m_contextChanged = true;
}

void Renderer::DestroyRenderResource(RenderResourceHandle renderResourceHandle)
{
	ScopedMonitorLock lock(this);

	m_renderResourceManager.DestroyResource(renderResourceHandle);
}

void Renderer::SetViewportRect(UUID renderContextId, Rect viewportRect)
{
	ScopedMonitorLock lock(this);

	auto& renderContext = m_renderContexts[renderContextId];
	renderContext->GetRenderTarget().SetViewportRect(viewportRect);
}

void Renderer::SetFrameBufferHandle(
	UUID renderContextId,
	RenderResourceHandle frameBufferHandle)
{
	ScopedMonitorLock lock(this);

	auto& renderContext = m_renderContexts[renderContextId];
	renderContext->GetRenderTarget().SetFrameBufferHandle(frameBufferHandle);
}

bool Renderer::StartRender(UUID renderContextId)
{
	ScopedMonitorLock lock(this);

	auto& renderContext = m_renderContexts[renderContextId];
	if (renderContext->GetRenderTarget().IsValid(m_renderResourceManager))
	{
		WaitFor(renderContext->GetRenderPermit());
		return true;
	}
	return false;
}

void Renderer::EndRender(UUID renderContextId)
{
	ScopedMonitorLock lock(this);

	m_endRenderRequests.push_back(renderContextId);
	if (m_endRenderRequests.size() == 1)
	{
		Signal(m_runPermit);
	}
}

UUID Renderer::GenerateRenderContext(RenderTargetType renderTargetType)
{
	ScopedMonitorLock lock(this);

	auto renderContext = std::unique_ptr<RenderContext>(
		new RenderContext(renderTargetType));
	auto renderContextId = renderContext->GetId();
	m_renderContexts.insert(
		std::make_pair(renderContextId, std::move(renderContext)));

	return renderContextId;
}

RenderResourceHandle Renderer::GenerateTexture(
	int width,
	int height,
	std::unique_ptr<GLvoid> pixels)
{
	ScopedMonitorLock lock(this);

	return m_renderResourceManager.GenerateTexture(
		width, height, std::move(pixels));
}

RenderResourceHandle Renderer::GenerateFrameBuffer(Vec2f size)
{
	ScopedMonitorLock lock(this);

	return m_renderResourceManager.GenerateFrameBuffer(size);
}

RenderResourceHandle Renderer::GenerateMesh(
	std::unique_ptr<GLfloat[]> vertexBuffer,
	int vertexBufferSize,
	std::unique_ptr<GLfloat[]> texcoordBuffer,
	int texcoordBufferSize,
	std::unique_ptr<GLushort[]> indexBuffer,
	int indexBufferSize)
{
	ScopedMonitorLock lock(this);

	return m_renderResourceManager.GenerateMesh(
		std::move(vertexBuffer), vertexBufferSize,
		std::move(texcoordBuffer), texcoordBufferSize,
		std::move(indexBuffer), indexBufferSize);
}

RenderResourceHandle Renderer::GenerateShader(
	GLuint type,
	std::string source,
	std::vector<std::string> uniformNames,
	std::vector<std::string> attributeNames)
{
	ScopedMonitorLock lock(this);

	return m_renderResourceManager.GenerateShader(
		type, source, uniformNames, attributeNames);
}

RenderResourceHandle Renderer::GenerateShaderProgram(
	RenderResourceHandle vertexShaderHandle,
	RenderResourceHandle fragmentShaderHandle)
{
	ScopedMonitorLock lock(this);

	return m_renderResourceManager.GenerateShaderProgram(
		vertexShaderHandle, fragmentShaderHandle);
}

void Renderer::RunThread()
{
	while (!m_killThreadRequested)
	{
		EnterCriticalSection();
		WaitFor(m_runPermit);
		WaitFor(m_contextLock);

		if (m_killThreadRequested)
		{
			Signal(m_contextLock);
			break;
		}

		if (m_contextLost)
		{
			assert(m_nativeWindow);
			m_GLContextManager->CreateContext(*m_nativeWindow);
			m_renderResourceManager.QueueBuiltResourcesForRebuild();
			m_contextLost = false;
		}

		if (m_contextChanged)
		{
			assert(m_nativeWindow);
			m_GLContextManager->CreateSurface(*m_nativeWindow);
			m_contextChanged = false;
		}

		m_renderResourceManager.ProcessQueues();

		auto endRenderRequests = m_endRenderRequests;
		ExitCriticalSection();
		for (auto renderContextId : endRenderRequests)
		{
			Render(renderContextId);
		}
		EnterCriticalSection();
		for (auto ended : endRenderRequests)
		{
			Signal(m_renderContexts[ended]->GetRenderPermit());
			m_endRenderRequests.erase(std::remove(
				m_endRenderRequests.begin(), m_endRenderRequests.end(), ended),
				m_endRenderRequests.end());
		}
		if (m_endRenderRequests.size() > 0)
		{
			Signal(m_runPermit);
		}

		Signal(m_contextLock);
		ExitCriticalSection();
	}

	if (m_nativeWindow)
	{
		m_GLContextManager->DestroyContext(*m_nativeWindow);
	}
}

void Renderer::OnKillThread()
{
	ScopedMonitorLock lock(this);

	m_killThreadRequested = true;
	Signal(m_contextLock);
	Signal(m_runPermit);
}

void Renderer::Render(UUID renderContextId)
{
	auto& renderContext = m_renderContexts[renderContextId];
	auto& renderTarget = renderContext->GetRenderTarget();

	renderTarget.Setup(m_renderResourceManager);

	renderContext->GetRenderPipeline().ProcessAllRenderLists(
		renderContext->GetProjection(), m_renderResourceManager);

	renderTarget.Finalize();

	if (renderTarget.IsOnScreen())
	{
		assert(m_nativeWindow);
		m_GLContextManager->Swap(*m_nativeWindow);
	}
}
