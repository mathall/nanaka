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

#include "renderer/Projection.h"
#include "renderer/RenderElement.h"
#include "renderer/RenderList.h"

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

	void CompileRenderLists(
		const std::vector<std::unique_ptr<RenderElement>>& renderQueue);

	void ProcessAllRenderLists(
		const RenderResourceManager& renderResourceManager) const;
	void ProcessRenderList(
		RenderList renderList,
		const RenderResourceManager& renderResourceManager) const;

	void SetDepthSortAxis(DepthSortAxis axis);
	void SetProjection(Projection projection);

private:

	GLuint GetGLHandle(
		RenderResourceHandle renderResourceHandle,
		const RenderResourceManager& renderResourceManager) const;

	std::vector<RenderElement*> m_renderLists[RenderListNum];
	DepthSortAxis m_depthSortAxis;
	Projection m_projection;
};

inline void RenderPipeline::SetDepthSortAxis(DepthSortAxis axis)
{
	m_depthSortAxis = axis;
}

inline void RenderPipeline::SetProjection(Projection projection)
{
	m_projection = projection;
}

#endif // NANAKA_RENDERER_RENDERPIPELINE_H
