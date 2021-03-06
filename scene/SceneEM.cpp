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

#include "scene/SceneEM.h"

#include "entity/Entity.h"
#include "renderer/RenderData.h"
#include "utils/Time.h"

void SceneEM::PrepRender(RenderData& renderData) const
{
	const auto entityTransform = Mat44::MakeTransform(
		GetEntity().GetPosition(), GetEntity().GetRotation(), Vec3f(1.0f));

	for (const auto& attachment : m_attachments)
	{
		if (attachment->IsVisible())
		{
			auto RE = renderData.GetRenderElement();
			attachment->PrepRender(*RE);
			RE->m_modelTransform = entityTransform * RE->m_modelTransform;
			renderData.QueueRenderElement(std::move(RE));
		}
	}
}

void SceneEM::Animate(const Time& time) const
{
	for (const auto& attachment : m_attachments)
	{
		attachment->Animate(time.GetGameStep());
	}
}
