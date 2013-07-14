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

#ifndef NANAKA_ANIMATION_ANIMATEDBILLBOARD_H
#define NANAKA_ANIMATION_ANIMATEDBILLBOARD_H

#include "graphics/BillboardAnimationResource.h"
#include "graphics/KeyframedAnimation.h"
#include "graphics/Model.h"
#include "resource/Asset.h"

class RenderElement;

class AnimatedBillboard final
{
public:

	AnimatedBillboard() = default;
	explicit AnimatedBillboard(std::string animationFilePath);

	void PrepRender(RenderElement& RE) const;
	void Animate(long unsigned int ms);
	void ResetAnimation();

	void SetPosition(Vec3f position);
	void SetRotation(Quat rotation);
	void SetScale(Vec3f scale);

private:

	Model m_billboard;

	Asset<BillboardAnimationResource> m_asset;
	KeyframedAnimation<Material> m_materialAnimation;
};

inline void AnimatedBillboard::PrepRender(RenderElement& RE) const
{
	m_billboard.PrepRender(RE);
}

inline void AnimatedBillboard::SetPosition(Vec3f position)
{
	m_billboard.SetPosition(position);
}

inline void AnimatedBillboard::SetRotation(Quat rotation)
{
	m_billboard.SetRotation(rotation);
}

inline void AnimatedBillboard::SetScale(Vec3f scale)
{
	m_billboard.SetScale(scale);
}

inline void AnimatedBillboard::ResetAnimation()
{
	m_materialAnimation.ResetAnimation();
}

#endif // NANAKA_ANIMATION_ANIMATEDBILLBOARD_H
