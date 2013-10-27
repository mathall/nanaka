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

#include <cassert>
#include <memory>

#include "math/Rect.h"
#include "renderer/RenderResourceHandle.h"

class FrameBufferRenderResource;
class RenderResourceManager;

enum RenderTargetType
{
	OffScreenRenderTargetType,
	OnScreenRenderTargetType,
};

class RenderTarget final
{
public:

	explicit RenderTarget(RenderTargetType type);

	bool IsOnScreen() const;
	bool IsValid(const RenderResourceManager& renderResourceManager) const;
	void SetFrameBufferHandle(RenderResourceHandle frameBufferHandle);
	void SetViewportRect(Rect viewportRect);
	void Setup(const RenderResourceManager& renderResourceManager) const;
	void Finalize() const;

private:

	RenderResourceHandle m_frameBufferHandle;
	RenderTargetType m_type;
	Rect m_viewportRect;
};

inline void RenderTarget::SetFrameBufferHandle(
	RenderResourceHandle frameBufferHandle)
{
	assert(!IsOnScreen());
	m_frameBufferHandle = frameBufferHandle;
}

inline void RenderTarget::SetViewportRect(Rect viewportRect)
{
	m_viewportRect = viewportRect;
}

inline bool RenderTarget::IsOnScreen() const
{
	return m_type == OnScreenRenderTargetType;
}

#endif // NANAKA_RENDERER_RENDERTARGET_H
