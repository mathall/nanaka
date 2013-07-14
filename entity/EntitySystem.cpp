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

#include "entity/EntitySystem.h"

#include "entity/EntityTree.h"

std::shared_ptr<Entity> EntitySystem::SpawnEntity(
	EntityTypeName entityTypeName,
	const EntityModuleParameters* spawnParams,
	EntityTree* tree)
{
	const auto& entityTypeIt = m_entityTypes.find(entityTypeName);

	if (entityTypeIt == m_entityTypes.end())
	{
		return nullptr;
	}

	const auto& entityType = entityTypeIt->second;

	auto entity = std::make_shared<Entity>(entityType->GetTypeId());

	for (const auto& moduleType : entityType->GetModuleTypes())
	{
		if (const auto& moduleFactory = m_moduleFactories[moduleType])
		{
			entity->SetModule(std::unique_ptr<EntityModule>(
				moduleFactory->CreateModule(*entity)));
		}
	}

	entityType->InitializeEntity(*entity, spawnParams);

	if (tree)
	{
		tree->AttachToRoot(entity);
	}

	return entity;
}
