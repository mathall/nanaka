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
#include "renderer/IndexBufferRenderResource.h"
#include "renderer/Projection.h"
#include "renderer/RenderResourceManager.h"
#include "renderer/ShaderProgramRenderResource.h"
#include "renderer/TextureRenderResource.h"
#include "renderer/VertexBufferRenderResource.h"

RenderPipeline::RenderPipeline()
	: m_depthSortAxis(DepthSortAxisY)
{
}

void RenderPipeline::ProcessAllRenderLists(
	const RenderResourceManager& renderResourceManager) const
{
	ProcessRenderList(RenderListGeneral, renderResourceManager);
	ProcessRenderList(RenderListTransparent, renderResourceManager);
}

void RenderPipeline::ProcessRenderList(
	RenderList renderList,
	const RenderResourceManager& renderResourceManager) const
{
	for (auto& renderElement : m_renderLists[renderList])
	{
		auto indexBuffer = renderResourceManager.Get<IndexBufferRenderResource>(
			renderElement->m_indexBufferHandle);

		if (!indexBuffer || indexBuffer->m_EBO == 0)
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

		for (auto& uniform : renderElement->m_uniforms)
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
					location, 1, GL_FALSE, m_projection.GetView().data());
				break;
			case ProjUniformType:
				glUniformMatrix4fv(
					location, 1, GL_FALSE, m_projection.GetProj().data());
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

		for (auto& attribute : renderElement->m_attributes)
		{
			auto location = shaderProgram->GetAttributeLocation(
				attribute.m_desc.m_identifier);
			auto vertexBuffer =
				renderResourceManager.Get<VertexBufferRenderResource>(
					attribute.m_handle);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_VBO);
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(
				location, attribute.m_desc.m_num, GL_FLOAT, GL_FALSE, 0, 0);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->m_EBO);
		glDrawElements(
			GL_TRIANGLES, indexBuffer->m_bufferDataSize, GL_UNSIGNED_SHORT, 0);

	}
}

void RenderPipeline::CompileRenderLists(
	const std::vector<std::unique_ptr<RenderElement>>& renderQueue)
{
	m_renderLists[RenderListGeneral].clear();
	m_renderLists[RenderListTransparent].clear();

	for (auto& renderElement : renderQueue)
	{
		m_renderLists[renderElement->m_renderList].push_back(
			renderElement.get());
	}

	auto transpElementOrderCmp = [&](
		const RenderElement* e1,
		const RenderElement* e2)
	{
		return e1->m_modelTransform[3][m_depthSortAxis]
			> e2->m_modelTransform[3][m_depthSortAxis];
	};

	std::sort(
		m_renderLists[RenderListTransparent].begin(),
		m_renderLists[RenderListTransparent].end(),
		transpElementOrderCmp);
}

GLuint RenderPipeline::GetGLHandle(
	RenderResourceHandle renderResourceHandle,
	const RenderResourceManager& renderResourceManager) const
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
