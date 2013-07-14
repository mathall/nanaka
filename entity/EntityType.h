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

#ifndef NANAKA_ENTITY_ENTITYTYPE_H
#define NANAKA_ENTITY_ENTITYTYPE_H

#include <initializer_list>
#include <vector>

#include "entity/EntityModule.h"
#include "utils/UUID.h"

class Entity;

/**
 * EntityModuleParameters may be implemented and supplied to the EntitySystem
 * when spawning an Entity in order to help initialize it.
 */
class EntityModuleParameters{};

/**
 * EntityType is implemented by game level code to define how a certain type of
 * entity in the game should behave. It is registered on the EntitySystem in
 * order to spawn entities conforming to the defined type.
 */
class EntityType
{
public:

	explicit EntityType(std::initializer_list<EntityModuleType> moduleTypes);
	virtual ~EntityType(){}

	UUID GetTypeId() const;
	const std::vector<EntityModuleType>& GetModuleTypes() const;

private:

	// EntitySystem need to initialize entity.
	friend class EntitySystem;

	virtual void InitializeEntity(
		Entity& entity,
		const EntityModuleParameters* params) const =0;

	UUID m_typeId;
	std::vector<EntityModuleType> m_moduleTypes;
};

inline EntityType::EntityType(
	std::initializer_list<EntityModuleType> moduleTypes)
	: m_typeId(UUID::New())
	, m_moduleTypes(moduleTypes)
{
}

inline UUID EntityType::GetTypeId() const
{
	return m_typeId;
}

inline const std::vector<EntityModuleType>& EntityType::GetModuleTypes() const
{
	return m_moduleTypes;
}

#endif // NANAKA_ENTITY_ENTITYTYPE_H
