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

#ifndef NANAKA_ENTITY_ENTITY_H
#define NANAKA_ENTITY_ENTITY_H

#include <unordered_map>
#include <memory>
#include <vector>

#include "entity/EntityModule.h"
#include "math/Quat.h"
#include "math/Vec3f.h"
#include "utils/UUID.h"

class EntityTree;

/**
 * Entity uses modules to bind together the various aspects of what makes an
 * entity in a game. Entities form a tree structure, and all operations on an
 * entity affects all of its children.
 */
class Entity final
{
public:

	explicit Entity(UUID typeId);
	~Entity();

	Entity(Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	template<typename M>
	M* GetModule() const;

	// Attaching an entity will detach from its previous parent.
	void AttachEntity(std::shared_ptr<Entity> entity);
	void DetachFromParent();

	UUID GetId() const;
	UUID GetEntityTypeID() const;

	void SetPosition(Vec3f pos);
	Vec3f GetPosition() const;
	Vec3f GetRelativePosition() const;

	void SetRotation(Quat rot);
	Quat GetRotation() const;
	Quat GetRelativeRotation() const;

private:

	// EntitySystem need to set module for the entity when it is being spawned.
	friend class EntitySystem;

	// EntityTree sets itself as the tree of its root entity.
	friend class EntityTree;

	void SetModule(std::unique_ptr<EntityModule> module);

	void InsertIntoTreeRecursively(EntityTree* tree);
	void ExtractOutOfTreeRecursively();

	std::vector<std::unique_ptr<EntityModule>> m_modules;

	Entity* m_parent;
	std::unordered_map<UUID, std::shared_ptr<Entity>> m_children;

	EntityTree* m_tree;

	UUID m_Id;
	UUID m_typeId;

	Vec3f m_position;
	Quat m_rotation;
};

template<typename M>
inline M* Entity::GetModule() const
{
	return static_cast<M*>(m_modules[M::GetModuleType()].get());
}

inline void Entity::SetModule(std::unique_ptr<EntityModule> module)
{
	m_modules[module->GetType()] = std::move(module);
}

inline UUID Entity::GetEntityTypeID() const
{
	return m_typeId;
}

inline UUID Entity::GetId() const
{
	return m_Id;
}

inline Vec3f Entity::GetPosition() const
{
	return m_position;
}

inline Vec3f Entity::GetRelativePosition() const
{
	return m_position - (m_parent ? m_parent->GetPosition() : Vec3f::Zero());
}

inline Quat Entity::GetRotation() const
{
	return m_rotation;
}

inline Quat Entity::GetRelativeRotation() const
{
	return (m_parent ? m_parent->GetRotation().Inverted() : Quat::NoRot())
		* m_rotation;
}

#endif // NANAKA_ENTITY_ENTITY_H
