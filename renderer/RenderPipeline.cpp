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

#include "renderer/FrameBufferRenderResource.h"
#include "renderer/GL.h"
#include "renderer/MeshRenderResource.h"
#include "renderer/Projection.h"
#include "renderer/RenderResourceManager.h"
#include "renderer/ShaderProgramRenderResource.h"
#include "renderer/TextureRenderResource.h"

RenderPipeline::RenderPipeline()
	: m_depthSortAxis(DepthSortAxisY)
{
}

void RenderPipeline::ProcessAllRenderLists(
	Projection& projection,
	const RenderResourceManager& renderResourceManager)
{
	ProcessRenderList(RenderListGeneral, projection, renderResourceManager);
	ProcessRenderList(RenderListTransparent, projection, renderResourceManager);
}

void RenderPipeline::ProcessRenderList(
	RenderList renderList,
	Projection& projection,
	const RenderResourceManager& renderResourceManager)
{
	for (auto& renderElement : m_renderLists[renderList])
	{
		auto mesh = renderResourceManager.Get<MeshRenderResource>(
			renderElement->m_meshHandle);

		if (!mesh || mesh->m_EBO == 0)
		{
			continue;
		}

		auto shaderProgram =
			renderResourceManager.Get<ShaderProgramRenderResource>(
				renderElement->m_shaderProgramHandle);

		if (!shaderProgram)
		{
			continue;
		}

		glUseProgram(shaderProgram->m_program);

		for (auto uniform : renderElement->m_uniforms)
		{
			auto location = shaderProgram->GetUniformLocation(
				uniform.m_desc.m_identifier);
			switch (uniform.m_desc.m_type)
			{
			case ModelUniformType:
				glUniformMatrix4fv(location, 1, GL_FALSE,
					renderElement->m_modelTransform.data());
				break;
			case ViewUniformType:
				glUniformMatrix4fv(
					location, 1, GL_FALSE, projection.GetView().data());
				break;
			case ProjUniformType:
				glUniformMatrix4fv(
					location, 1, GL_FALSE, projection.GetProj().data());
				break;
			case TexUniformType:
				glActiveTexture(GL_TEXTURE0 + uniform.m_value.m_tex.m_unit);
				glBindTexture(GL_TEXTURE_2D,
					GetGLHandle(
						uniform.m_value.m_tex.m_handle, renderResourceManager));
				glUniform1i(location, uniform.m_value.m_tex.m_unit);
				break;
			case Vec2UniformType:
				glUniform2fv(location, 1, uniform.m_value.m_vec2.data());
				break;
			case Vec4UniformType:
				glUniform4fv(location, 1, uniform.m_value.m_vec4.data());
				break;
			default:
				break;
			}
		}

		for (auto attribute : renderElement->m_attributes)
		{
			auto location = shaderProgram->GetAttributeLocation(
				attribute.m_desc.m_identifier);
			glBindBuffer(GL_ARRAY_BUFFER,
				mesh->GetAttributeBuffer(attribute.m_desc.m_identifier));
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(
				location, attribute.m_desc.m_num, GL_FLOAT, GL_FALSE, 0, 0);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_EBO);
		glDrawElements(
			GL_TRIANGLES, mesh->m_indexBufferSize, GL_UNSIGNED_SHORT, 0);

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

GLuint RenderPipeline::GetGLHandle(
	RenderResourceHandle renderResourceHandle,
	const RenderResourceManager& renderResourceManager)
{
	GLuint handle = 0;

	if (auto renderResource = renderResourceManager.Get(renderResourceHandle))
	{
		switch (renderResource->GetType())
		{
		case TextureRenderResourceType:
			handle = static_cast<TextureRenderResource*>(
				renderResource)->m_texHandle;
			break;
		case FrameBufferRenderResourceType:
			handle = static_cast<FrameBufferRenderResource*>(
				renderResource)->m_colorBuffer;
			break;
		default:
			break;
		}
	}

	return handle;
}
