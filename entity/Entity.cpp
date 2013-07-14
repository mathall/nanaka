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

#include "entity/Entity.h"

#include <algorithm>
#include <cassert>

#include "entity/EntityModuleManager.h"
#include "entity/EntityTree.h"

Entity::Entity(UUID typeId)
	: m_modules(NumEntityModuleType)
	, m_parent(NULL)
	, m_tree(NULL)
	, m_Id(UUID::New())
	, m_typeId(typeId)
	, m_position(Vec3f::Zero())
	, m_rotation(Quat::NoRot())
{
}

Entity::~Entity()
{
	std::vector<std::shared_ptr<Entity>> children;
	for (auto& child : m_children)
	{
		children.push_back(child.second);
	}
	for (auto child : children)
	{
		child->DetachFromParent();
	}
	ExtractOutOfTreeRecursively();
}

void Entity::AttachEntity(std::shared_ptr<Entity> entity)
{
	if (entity->m_parent)
	{
		entity->DetachFromParent();
	}

	m_children.insert(std::make_pair(entity->GetId(), entity));
	entity->m_parent = this;
	entity->InsertIntoTreeRecursively(m_tree);
}

void Entity::DetachFromParent()
{
	assert(m_parent
		&& m_parent->m_children.find(GetId()) != m_parent->m_children.end());

	m_parent->m_children.erase(GetId());
	m_parent = NULL;
	ExtractOutOfTreeRecursively();
}

void Entity::InsertIntoTreeRecursively(EntityTree* tree)
{
	m_tree = tree;

	if (m_tree)
	{
		for (auto& module : m_modules)
		{
			if (module)
			{
				m_tree->GetModuleManager(
					module->GetType())->ManageModule(module.get());
			}
		}
	}

	for (auto child : m_children)
	{
		child.second->InsertIntoTreeRecursively(m_tree);
	}
}

void Entity::ExtractOutOfTreeRecursively()
{
	if (m_tree)
	{
		for (auto& module : m_modules)
		{
			if (module)
			{
				m_tree->GetModuleManager(
					module->GetType())->UnmanageModule(*module);
			}
		}
	}

	for (auto child : m_children)
	{
		child.second->ExtractOutOfTreeRecursively();
	}

	m_tree = NULL;
}

void Entity::SetPosition(Vec3f pos)
{
	for (auto& child : m_children)
	{
		child.second->SetPosition(pos + child.second->GetRelativePosition());
	}

	m_position = pos;
}

void Entity::SetRotation(Quat rot)
{
	for (auto& child : m_children)
	{
		child.second->SetRotation(rot * child.second->GetRelativeRotation());
	}

	m_rotation = rot;
}
