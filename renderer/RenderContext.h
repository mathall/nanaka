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

#ifndef NANAKA_RENDERER_RENDERCONTEXT_H
#define NANAKA_RENDERER_RENDERCONTEXT_H

#include "renderer/Projection.h"
#include "renderer/RenderData.h"
#include "renderer/RenderTarget.h"
#include "renderer/RenderPipeline.h"
#include "utils/Sem.h"
#include "utils/UUID.h"

class RenderContext final
{
public:

	explicit RenderContext(RenderTargetType renderTargetType);

	RenderTarget& GetRenderTarget();
	const Projection& GetProjection() const;
	void SetProjection(Projection projection);
	RenderPipeline& GetRenderPipeline();
	Sem& GetRenderPermit();
	UUID GetId() const;

	std::unique_ptr<RenderData> GetRenderData();
	void CommitRenderData(std::unique_ptr<RenderData> renderData);
	void CompileRenderLists();
	void ClearRenderQueue();

private:

	UUID m_Id;
	RenderTarget m_renderTarget;
	Projection m_projection;
	std::unique_ptr<RenderData> m_renderData;
	RenderPipeline m_pipeline;
	Sem m_renderPermit;
};

inline RenderContext::RenderContext(RenderTargetType renderTargetType)
	: m_Id(UUID::New())
	, m_renderTarget(renderTargetType)
	, m_renderData(new RenderData())
	, m_renderPermit(1)
{
}

inline RenderTarget& RenderContext::GetRenderTarget()
{
	return m_renderTarget;
}

inline const Projection& RenderContext::GetProjection() const
{
	return m_projection;
}

inline void RenderContext::SetProjection(Projection projection)
{
	m_projection = projection;
}

inline RenderPipeline& RenderContext::GetRenderPipeline()
{
	return m_pipeline;
}

inline Sem& RenderContext::GetRenderPermit()
{
	return m_renderPermit;
}

inline UUID RenderContext::GetId() const
{
	return m_Id;
}

inline std::unique_ptr<RenderData> RenderContext::GetRenderData()
{
	return std::move(m_renderData);
}

inline void RenderContext::CommitRenderData(
	std::unique_ptr<RenderData> renderData)
{
	m_renderData = std::move(renderData);
}

inline void RenderContext::CompileRenderLists()
{
	m_pipeline.CompileRenderLists(m_renderData->m_renderQueue);
}

#endif // NANAKA_RENDERER_RENDERCONTEXT_H
