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

#ifndef NANAKA_RENDERER_RENDERPIPELINE_H
#define NANAKA_RENDERER_RENDERPIPELINE_H

#include <memory>
#include <vector>

#include "renderer/RenderElement.h"
#include "renderer/RenderList.h"
#include "utils/ObjectPool.h"

class Projection;
class RenderResourceManager;

enum DepthSortAxis
{
	DepthSortAxisX,
	DepthSortAxisY,
	DepthSortAxisZ,
};

class RenderPipeline final
{
public:

	RenderPipeline();

	void ProcessAllRenderLists(
		const Projection& projection,
		const RenderResourceManager& renderResourceManager);
	void ProcessRenderList(
		RenderList renderList,
		const Projection& projection,
		const RenderResourceManager& renderResourceManager);

	void SetDepthSortAxis(DepthSortAxis axis);
	std::unique_ptr<RenderElement> GetRE();
	void QueueRE(std::unique_ptr<RenderElement> renderElement);

private:

	GLuint GetGLHandle(
		RenderResourceHandle renderResourceHandle,
		const RenderResourceManager& renderResourceManager);

	ObjectPool<RenderElement, RenderElement::Clear> m_renderElementPool;
	std::vector<std::unique_ptr<RenderElement>> m_renderLists[RenderListNum];
	DepthSortAxis m_depthSortAxis;
};

inline std::unique_ptr<RenderElement> RenderPipeline::GetRE()
{
	return m_renderElementPool.GetObject();
}

inline void RenderPipeline::SetDepthSortAxis(DepthSortAxis axis)
{
	m_depthSortAxis = axis;
}

#endif // NANAKA_RENDERER_RENDERPIPELINE_H
