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

#ifndef NANAKA_SCENE_SCENEEM_H
#define NANAKA_SCENE_SCENEEM_H

#include <cassert>
#include <unordered_map>
#include <memory>
#include <vector>

#include "entity/EntityModule.h"
#include "scene/SceneEMAttachment.h"

class RenderData;
class Time;

typedef int AttachmentId;

class SceneEM final : public EntityModule
{
public:

	static EntityModuleType GetModuleType();

	explicit SceneEM(Entity& entity);

	template<typename A>
	void SetAttachment(AttachmentId id, A attachment);

	template<typename A>
	A GetAttachment(AttachmentId id) const;

	void PrepRender(RenderData& renderData) const;
	void Animate(const Time& time) const;
	void SetVisibility(AttachmentId id, bool visible) const;
	void ResetAnimation(AttachmentId id) const;

private:

	std::unordered_map<AttachmentId, size_t> m_attachmentIdToIdx;
	std::vector<std::unique_ptr<SceneEMAttachment>> m_attachments;
};

inline EntityModuleType SceneEM::GetModuleType()
{
	static const EntityModuleType s_moduleType = SceneEntityModuleType;
	return s_moduleType;
}

inline SceneEM::SceneEM(Entity& entity)
	: EntityModule(GetModuleType(), entity)
{
}

template<typename A>
void SceneEM::SetAttachment(AttachmentId id, A attachment)
{
	assert(m_attachmentIdToIdx.find(id) == m_attachmentIdToIdx.end());
	m_attachmentIdToIdx.insert(std::make_pair(id, m_attachments.size()));
	m_attachments.push_back(std::unique_ptr<SceneEMAttachment>(
		new SceneEMAttachmentImpl<A>(attachment)));
}

template<typename A>
A SceneEM::GetAttachment(AttachmentId id) const
{
	return static_cast<SceneEMAttachmentImpl<A>*>(m_attachments.at(
		m_attachmentIdToIdx.at(id)).get())->m_attachment;
}

inline void SceneEM::SetVisibility(AttachmentId id, bool visible) const
{
	m_attachments.at(m_attachmentIdToIdx.at(id))->SetVisible(visible);
}

inline void SceneEM::ResetAnimation(AttachmentId id) const
{
	m_attachments.at(m_attachmentIdToIdx.at(id))->ResetAnimation();
}

#endif // NANAKA_SCENE_SCENEEM_H
