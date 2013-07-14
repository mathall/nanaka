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

#include "graphics/AnimatedBillboard.h"

AnimatedBillboard::AnimatedBillboard(std::string animationFilePath)
	: m_billboard("nanaka/models/billboard.nmdl")
	, m_asset(g_resourceManager->Acquire<BillboardAnimationResource>(
		animationFilePath))
{
	if (!m_asset.IsValid())
	{
		return;
	}

	Material material;
	material.SetShaderProgram(ShaderProgram("nanaka/shaders/scaleoffset.nsp"));
	material.SetTexture(Texture(m_asset.GetResource()->m_textureAsset));
	material.SetTransparent(true);
	material.SetColor(Vec4f(1.0f));

	for (size_t i = 0; i < m_asset.GetResource()->m_times.size(); ++i)
	{
		material.SetTexcoordsScale(
			m_asset.GetResource()->m_texcoordsScales.at(i));
		material.SetTexcoordsOffset(
			m_asset.GetResource()->m_texcoordsOffsets.at(i));
		auto ms = m_asset.GetResource()->m_times.at(i);
		m_materialAnimation.AddKeyframe(
			ms, material, Interpolation<Material>::Flat());
	}

	m_materialAnimation.ResetAnimation();
}

void AnimatedBillboard::Animate(long unsigned int ms)
{
	m_materialAnimation.Animate(ms);
	m_billboard.SetMaterial(m_materialAnimation.GetValue());
}
