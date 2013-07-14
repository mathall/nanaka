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

#ifndef NANAKA_ENTITY_ENTITYSYSTEM_H
#define NANAKA_ENTITY_ENTITYSYSTEM_H

#include <cassert>
#include <unordered_map>
#include <memory>

#include "entity/EntityModule.h"
#include "entity/EntityModuleFactory.h"
#include "entity/EntityType.h"
#include "utils/UUID.h"

class Entity;
class EntityModuleParameters;
class EntityTree;

typedef int EntityTypeName;

/**
 * EntitySystem handles entity types and spawning of entities conforming to
 * these types.
 */
class EntitySystem final
{
public:

	EntitySystem();

	std::shared_ptr<Entity> SpawnEntity(
		EntityTypeName entityTypeName,
		const EntityModuleParameters* spawnParameters,
		EntityTree* tree = NULL);

	template<typename F>
	void RegisterEntityModuleFactory();

	template<typename T>
	void RegisterEntityType(EntityTypeName entityTypeName);

	UUID GetEntityTypeId(EntityTypeName entityTypeName);

private:

	std::unordered_map<
		EntityTypeName, std::unique_ptr<EntityType>> m_entityTypes;
	std::vector<std::unique_ptr<EntityModuleFactory>> m_moduleFactories;
};

inline EntitySystem::EntitySystem()
	: m_moduleFactories(NumEntityModuleType)
{
}

template<typename F>
inline void EntitySystem::RegisterEntityModuleFactory()
{
	m_moduleFactories[F::GetModuleType()] =
		std::unique_ptr<EntityModuleFactory>(new EntityModuleFactoryImpl<F>());
}

template<typename T>
inline void EntitySystem::RegisterEntityType(EntityTypeName entityTypeName)
{
	assert(m_entityTypes.find(entityTypeName) == m_entityTypes.end());
	m_entityTypes.insert(std::make_pair(
		entityTypeName, std::unique_ptr<T>(new T())));
}

inline UUID EntitySystem::GetEntityTypeId(EntityTypeName entityTypeName)
{
	assert(m_entityTypes.find(entityTypeName) != m_entityTypes.end());
	return m_entityTypes.find(entityTypeName)->second->GetTypeId();
}

#endif // NANAKA_ENTITY_ENTITYSYSTEM_H
