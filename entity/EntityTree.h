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

#ifndef NANAKA_ENTITY_ENTITYTREE_H
#define NANAKA_ENTITY_ENTITYTREE_H

#include <memory>
#include <vector>

#include "entity/Entity.h"
#include "entity/EntityModuleManager.h"

/**
 * EntityTree hold a root Entity and all the EntityModuleManagers which may be
 * used by entities in this tree. The Entity use it to make sure its modules are
 * properly managed by the module managers. Thus entities belonging to different
 * trees do not affect each other.
 */
class EntityTree final
{
public:

	EntityTree();

	template<typename M>
	void RegisterEntityModuleManager();

	EntityModuleManager* GetModuleManager(EntityModuleType moduleType) const;

	template<typename M>
	M* GetModuleManager() const;

	void AttachToRoot(std::shared_ptr<Entity> entity);

private:

	std::vector<std::unique_ptr<EntityModuleManager>> m_moduleManagers;
	std::unique_ptr<Entity> m_rootEntity;
};

inline EntityTree::EntityTree()
	: m_moduleManagers(NumEntityModuleType)
	, m_rootEntity(new Entity(UUID::Invalid))
{
	m_rootEntity->m_tree = this;
}

template<typename M>
inline void EntityTree::RegisterEntityModuleManager()
{
	m_moduleManagers[M::GetModuleType()] =
		std::unique_ptr<EntityModuleManager>(new M());
}

inline EntityModuleManager* EntityTree::GetModuleManager(
	EntityModuleType moduleType) const
{
	return m_moduleManagers[moduleType].get();
}

template<typename M>
inline M* EntityTree::GetModuleManager() const
{
	return static_cast<M*>(GetModuleManager(M::GetModuleType()));
}

inline void EntityTree::AttachToRoot(std::shared_ptr<Entity> entity)
{
	m_rootEntity->AttachEntity(entity);
}

#endif // NANAKA_ENTITY_ENTITYTREE_H
