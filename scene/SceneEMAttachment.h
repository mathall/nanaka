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

#ifndef NANAKA_SCENE_SCENEEMATTACHMENT_H
#define NANAKA_SCENE_SCENEEMATTACHMENT_H

#include "graphics/AnimatedBillboard.h"
#include "graphics/Model.h"

/**
 * SceneEMAttachment is used to give graphical entities which may be attached to
 * a SceneEM a common interface.
 */
class SceneEMAttachment
{
public:

	SceneEMAttachment();

	virtual ~SceneEMAttachment(){}

	virtual void PrepRender(RenderElement& RE) const = 0;
	virtual void Animate(long unsigned int ms) = 0;
	virtual void ResetAnimation() = 0;

	void SetVisible(bool visible);
	bool IsVisible() const;

private:

	bool m_visible;
};

inline SceneEMAttachment::SceneEMAttachment()
	: m_visible(true)
{
}

inline void SceneEMAttachment::SetVisible(bool visible)
{
	m_visible = visible;
}

inline bool SceneEMAttachment::IsVisible() const
{
	return m_visible;
}

/**
 * SceneEMAttachmentImpl specify a specific graphical entity which may be
 * attached to a SceneEM.
 */
template<typename A>
class SceneEMAttachmentImpl final : public SceneEMAttachment
{
public:

	explicit SceneEMAttachmentImpl(A attachent);

	/**
	 * SceneEMAttachment implementation.
	 */
	void PrepRender(RenderElement& RE) const override;
	void Animate(long unsigned int ms) override;
	void ResetAnimation() override;

private:

	A m_attachment;
};

template<typename A>
inline SceneEMAttachmentImpl<A>::SceneEMAttachmentImpl(A attachent)
	: SceneEMAttachment()
	, m_attachment(attachent)
{
}

template<typename A>
inline void SceneEMAttachmentImpl<A>::PrepRender(RenderElement& RE) const
{
	m_attachment.PrepRender(RE);
}

template<typename A>
inline void SceneEMAttachmentImpl<A>::Animate(long unsigned int ms)
{
}

template<>
inline void SceneEMAttachmentImpl<AnimatedBillboard>::Animate(
	long unsigned int ms)
{
	m_attachment.Animate(ms);
}

template<typename A>
inline void SceneEMAttachmentImpl<A>::ResetAnimation()
{
}

template<>
inline void SceneEMAttachmentImpl<AnimatedBillboard>::ResetAnimation()
{
	m_attachment.ResetAnimation();
}

#endif // NANAKA_SCENE_SCENEEMATTACHMENT_H
