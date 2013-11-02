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

#ifndef NANAKA_RENDERER_RENDERER_H
#define NANAKA_RENDERER_RENDERER_H

#define g_renderer Renderer::GetInstance()

#include <memory>
#include <unordered_map>
#include <vector>

#include "pi/GLContextManager.h"
#include "pi/NanakaNativeWindow.h"
#include "renderer/RenderContext.h"
#include "renderer/RenderData.h"
#include "renderer/RenderResourceManager.h"
#include "renderer/RenderTarget.h"
#include "utils/Monitor.h"
#include "utils/Thread.h"
#include "utils/YOLO.h"

class Projection;

class Renderer final : public Thread, public Monitor, public YOLO<Renderer>
{
public:

	Renderer();

	void OnWindowCreated(NanakaNativeWindow* nativeWindow);
	void OnWindowDestroyed();
	void OnWindowChanged();

	UUID GenerateRenderContext(RenderTargetType renderTargetType);
	void SetDepthSortAxis(UUID renderContextId, DepthSortAxis axis);
	void SetViewportRect(UUID renderContextId, Rect viewportRect);
	void SetFrameBufferHandle(
		UUID renderContextId,
		RenderResourceHandle frameBufferHandle);
	void SetProjection(UUID renderContextId, Projection projection);
	std::unique_ptr<RenderData> StartRender(UUID renderContextId);
	void EndRender(
		UUID renderContextId,
		std::unique_ptr<RenderData> renderData);
	void DestroyRenderContext(UUID renderContextId);

	RenderResourceHandle GenerateTexture(
		int width,
		int height,
		std::unique_ptr<GLvoid> pixels);
	RenderResourceHandle GenerateFrameBuffer(Vec2f size);
	RenderResourceHandle GenerateVertexBuffer(
		std::unique_ptr<GLfloat[]> bufferData,
		int bufferDataSize);
	RenderResourceHandle GenerateIndexBuffer(
		std::unique_ptr<GLushort[]> bufferData,
		int bufferDataSize);
	RenderResourceHandle GenerateShader(
		GLuint type,
		std::string source,
		std::vector<std::string> uniformNames,
		std::vector<std::string> attributeNames);
	RenderResourceHandle GenerateShaderProgram(
		RenderResourceHandle vertexShaderHandle,
		RenderResourceHandle fragmentShaderHandle);
	void DestroyRenderResource(RenderResourceHandle renderResourceHandle);

protected:

	/**
	 * Thread implementation.
	 */
	void RunThread() override;
	void OnKillThread() override;

private:

	bool m_killThreadRequested;
	bool m_contextLost;
	bool m_contextChanged;

	std::unordered_map<UUID, std::unique_ptr<RenderContext>> m_renderContexts;
	RenderResourceManager m_renderResourceManager;
	std::unique_ptr<GLContextManager> m_GLContextManager;
	NanakaNativeWindow* m_nativeWindow;

	std::vector<UUID> m_endRenderRequests;

	Sem m_contextLock;
	Sem m_runPermit;
};

#endif // NANAKA_RENDERER_RENDERER_H
