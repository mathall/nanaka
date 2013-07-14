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

#ifndef NANAKA_ENTITY_ENTITYMODULE_H
#define NANAKA_ENTITY_ENTITYMODULE_H

class Entity;

enum EntityModuleType
{
	SceneEntityModuleType,
	PhysicsEntityModuleType,
	GameEntityModuleType,
	NumEntityModuleType,
};

/**
 * Depending on the EntityType, an Entity may have different EntityModules
 * attached to it. EntityModule must be implemented by a subsystem (graphics,
 * physics, etc.) which may then use an instance of it to perform operations
 * specific to the subsystem on the Entity it belong to.
 */
class EntityModule
{
public:

	EntityModule(EntityModuleType type, Entity& entity);
	virtual ~EntityModule(){}

	Entity& GetEntity() const;
	EntityModuleType GetType() const;

private:

	Entity& m_entity;
	EntityModuleType m_type;
};

inline EntityModule::EntityModule(EntityModuleType type, Entity& entity)
	: m_entity(entity)
	, m_type(type)
{
}

inline Entity& EntityModule::GetEntity() const
{
	return m_entity;
}

inline EntityModuleType EntityModule::GetType() const
{
	return m_type;
}

#endif // NANAKA_ENTITY_ENTITYMODULE_H
