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

#include "renderer/RenderPipeline.h"

#include <algorithm>

#include "renderer/GL.h"
#include "renderer/Projection.h"

RenderPipeline::RenderPipeline()
	: m_depthSortAxis(DepthSortAxisY)
{
}

void RenderPipeline::ProcessAllRenderLists(Projection& projection)
{
	ProcessRenderList(RenderListGeneral, projection);
	ProcessRenderList(RenderListTransparent, projection);
}

void RenderPipeline::ProcessRenderList(
	RenderList renderList,
	Projection& projection)
{
	for (auto& renderElement : m_renderLists[renderList])
	{
		if (renderElement->m_EBO == 0 || renderElement->m_shaderProgram == 0)
		{
			continue;
		}

		glUseProgram(renderElement->m_shaderProgram);

		for (auto uniform : renderElement->m_uniforms)
		{
			switch (uniform.m_type)
			{
			case ModelUniformType:
				glUniformMatrix4fv(uniform.m_location, 1, GL_FALSE,
					renderElement->m_modelTransform.data());
				break;
			case ViewUniformType:
				glUniformMatrix4fv(uniform.m_location, 1, GL_FALSE,
					projection.GetView().data());
				break;
			case ProjUniformType:
				glUniformMatrix4fv(uniform.m_location, 1, GL_FALSE,
					projection.GetProj().data());
				break;
			case TexUniformType:
				glActiveTexture(GL_TEXTURE0 + uniform.m_value.m_tex.m_unit);
				glBindTexture(GL_TEXTURE_2D, uniform.m_value.m_tex.m_handle);
				glUniform1i(uniform.m_location, uniform.m_value.m_tex.m_unit);
				break;
			case Vec2UniformType:
				glUniform2fv(
					uniform.m_location, 1, uniform.m_value.m_vec2.data());
				break;
			case Vec4UniformType:
				glUniform4fv(
					uniform.m_location, 1, uniform.m_value.m_vec4.data());
				break;
			default:
				break;
			}
		}

		for (auto attribute : renderElement->m_attributes)
		{
			glBindBuffer(GL_ARRAY_BUFFER, attribute.m_buffer);
			glEnableVertexAttribArray(attribute.m_location);
			glVertexAttribPointer(attribute.m_location, attribute.m_num,
				GL_FLOAT, GL_FALSE, 0, 0);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderElement->m_EBO);
		glDrawElements(
			GL_TRIANGLES, renderElement->m_numIndices, GL_UNSIGNED_SHORT, 0);

		m_renderElementPool.PutObject(std::move(renderElement));
	}

	m_renderLists[renderList].clear();
}

void RenderPipeline::QueueRE(std::unique_ptr<RenderElement> renderElement)
{
	if (renderElement->m_renderList == RenderListTransparent)
	{
		auto& renderList = m_renderLists[RenderListTransparent];

		auto transpElementOrderCmp = [&](
			const std::unique_ptr<RenderElement>& e1,
			const std::unique_ptr<RenderElement>& e2)
		{
			return e1->m_modelTransform[3][m_depthSortAxis]
				> e2->m_modelTransform[3][m_depthSortAxis];
		};

		auto pos = std::lower_bound(renderList.begin(), renderList.end(),
			renderElement, transpElementOrderCmp);

		renderList.insert(pos, std::move(renderElement));
	}
	else
	{
		m_renderLists[RenderListGeneral].push_back(std::move(renderElement));
	}
}
