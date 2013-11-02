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

#ifndef NANAKA_RENDERER_RENDERDATA_H
#define NANAKA_RENDERER_RENDERDATA_H

#include "renderer/RenderElement.h"
#include "utils/ObjectPool.h"

/**
 * RenderData packs the information needed by a RenderContext for rendering a
 * frame as instructed by the user of the renderer. Between StartRender and
 * EndRender a user may get RenderElements in order to fill these out then queue
 * them for rendering.
 */
class RenderData final
{
public:

	std::unique_ptr<RenderElement> GetRenderElement();
	void QueueRenderElement(std::unique_ptr<RenderElement> renderElement);

private:

	friend class RenderContext;

	ObjectPool<RenderElement, RenderElement::Clear> m_renderElementPool;
	std::vector<std::unique_ptr<RenderElement>> m_renderQueue;
};

inline std::unique_ptr<RenderElement> RenderData::GetRenderElement()
{
	return m_renderElementPool.GetObject();
}

inline void RenderData::QueueRenderElement(
	std::unique_ptr<RenderElement> renderElement)
{
	m_renderQueue.push_back(std::move(renderElement));
}

#endif // NANAKA_RENDERER_RENDERDATA_H
