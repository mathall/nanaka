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

#ifndef NANAKA_RENDERER_RENDERTARGET_H
#define NANAKA_RENDERER_RENDERTARGET_H

#include <memory>

#include "math/Rect.h"
#include "renderer/RenderResourceHandle.h"

class FrameBufferRenderResource;
class RenderResourceManager;

/**
 * RenderTargetClient is implemented by the GUI element receiving the rendered
 * image.
 */
class RenderTargetClient
{
public:

	virtual ~RenderTargetClient(){}

	virtual bool IsActive() const = 0;
	virtual Rect GetRect() const = 0;
};

class RenderTarget final
{
public:

	RenderTarget(
		RenderResourceHandle frameBufferHandle,
		RenderTargetClient* client);

	bool IsOnScreen() const;
	bool IsActive() const;
	void UpdateViewport(const RenderResourceManager& renderResourceManager);
	void Setup(const RenderResourceManager& renderResourceManager) const;
	void Finalize() const;

private:

	std::shared_ptr<FrameBufferRenderResource> GetFrameBuffer(
		const RenderResourceManager& renderResourceManager) const;

	RenderResourceHandle m_frameBufferHandle;
	RenderTargetClient* m_client;
	Rect m_viewportRect;
};

inline bool RenderTarget::IsActive() const
{
	return m_viewportRect.w > 0.0f && m_viewportRect.h > 0.0f
		&& m_client->IsActive();
}

inline bool RenderTarget::IsOnScreen() const
{
	return m_frameBufferHandle == RenderResourceHandle::Invalid;
}

#endif // NANAKA_RENDERER_RENDERTARGET_H
