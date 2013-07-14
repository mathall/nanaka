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

#ifndef NANAKA_GAME_GAMEOBJECT_H
#define NANAKA_GAME_GAMEOBJECT_H

#include <cassert>
#include <cstddef>

class Entity;
class Time;

class GameObject
{
public:

	GameObject();
	virtual ~GameObject(){}

	virtual void Update(Time& time){}

	Entity* GetEntity() const;

private:

	// GameEM sets the Entity when the GameObject is set on the GameEM.
	friend class GameEM;

	void SetEntity(Entity* entity);

	Entity* m_entity;
};

inline GameObject::GameObject()
	: m_entity(NULL)
{
}

inline Entity* GameObject::GetEntity() const
{
	assert(m_entity);
	return m_entity;
}

inline void GameObject::SetEntity(Entity* entity)
{
	assert(!m_entity);
	m_entity = entity;
}

#endif // NANAKA_GAME_GAMEOBJECT_H
