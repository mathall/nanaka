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

#include "gui/ImageComponent.h"

#include "math/Quat.h"
#include "renderer/RenderData.h"

ImageComponent::ImageComponent(
	std::string textureFilePath,
	Vec4f color,
	ImageRotation rotation,
	bool flipH,
	bool flipV)
	: m_billboard("nanaka/models/gui_billboard.nmdl")
	, m_rotation(rotation)
{
	Material material = m_billboard.GetMaterial();

	std::string shaderProgramFilePath;
	bool hasTexture = textureFilePath.compare("") != 0;
	if (hasTexture)
	{
		material.SetTexture(Texture(textureFilePath));

		if (flipH || flipV)
		{
			Vec2f texcoordScale(1.0f);
			Vec2f texcoordOffset(0.0f);

			if (flipH)
			{
				texcoordScale.x = -1.0f;
				texcoordOffset.x = 1.0f;
			}

			if (flipV)
			{
				texcoordScale.y = -1.0f;
				texcoordOffset.y = 1.0f;
			}

			material.SetTexcoordsScale(texcoordScale);
			material.SetTexcoordsOffset(texcoordOffset);

			shaderProgramFilePath = "nanaka/shaders/scaleoffset.nsp";
		}
		else
		{
			shaderProgramFilePath = "nanaka/shaders/default.nsp";
		}
	}
	else
	{
		shaderProgramFilePath = "nanaka/shaders/notex.nsp";
	}

	material.SetShaderProgram(ShaderProgram(shaderProgramFilePath));
	material.SetColor(color);
	material.SetTransparent(color.w != 1.0f || hasTexture);
	m_billboard.SetMaterial(material);

	Vec3f rotationAxis = Vec3f(0.0f, 0.0f, 1.0f);
	float rotationAngle = (3.142f * 0.5f * rotation);
	m_billboard.SetRotation(Quat(rotationAxis, rotationAngle));
}

void ImageComponent::SetPlacement(Vec2f position, Vec2f size, float depth)
{
	m_billboard.SetPosition(
		Vec3f(position.x + size.x * 0.5f, position.y + size.y * 0.5f, depth));

	if (m_rotation == ImageRotationNone || m_rotation == ImageRotation180Deg)
	{
		m_billboard.SetScale(Vec3f(size.x, size.y, 1.0f));
	}
	else
	{
		m_billboard.SetScale(Vec3f(size.y, size.x, 1.0f));
	}
}

void ImageComponent::Draw(RenderData& renderData) const
{
	auto RE = renderData.GetRenderElement();
	m_billboard.PrepRender(*RE);
	renderData.QueueRenderElement(std::move(RE));
}

void ImageComponent::SetAlpha(float totalAlpha)
{
	auto material = m_billboard.GetMaterial();
	material.SetAlpha(totalAlpha);
	m_billboard.SetMaterial(material);
}
