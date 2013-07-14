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

#ifndef NANAKA_GAME_GAMEEM_H
#define NANAKA_GAME_GAMEEM_H

#include <cassert>
#include <memory>

#include "entity/EntityModule.h"
#include "game/GameObject.h"

class Time;

class GameEM final : public EntityModule
{
public:

	static EntityModuleType GetModuleType();

	explicit GameEM(Entity& entity);

	void Update(Time& time) const;
	void SetGameObject(std::unique_ptr<GameObject> gameObject);
	GameObject& GetGameObject() const;

	template<typename G>
	G& GetGameObject() const;

private:

	std::unique_ptr<GameObject> m_gameObject;
};

inline EntityModuleType GameEM::GetModuleType()
{
	static const EntityModuleType s_moduleType = GameEntityModuleType;
	return s_moduleType;
}

inline GameEM::GameEM(Entity& entity)
	: EntityModule(GetModuleType(), entity)
{
}

inline void GameEM::Update(Time& time) const
{
	assert(m_gameObject);
	m_gameObject->Update(time);
}

inline void GameEM::SetGameObject(std::unique_ptr<GameObject> gameObject)
{
	assert(!m_gameObject);
	m_gameObject = std::move(gameObject);
	m_gameObject->SetEntity(&GetEntity());
}

inline GameObject& GameEM::GetGameObject() const
{
	assert(m_gameObject);
	return *m_gameObject;
}

template<typename G>
inline G& GameEM::GetGameObject() const
{
	return static_cast<G&>(GetGameObject());
}

#endif // NANAKA_GAME_GAMEEM_H
