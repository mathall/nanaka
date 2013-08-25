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

#ifndef NANAKA_GRAPHICS_MODEL_H
#define NANAKA_GRAPHICS_MODEL_H

#include <string>

#include "graphics/Material.h"
#include "graphics/ModelResource.h"
#include "resource/Asset.h"

class RenderElement;

class Model final
{
public:

	Model() = default;
	explicit Model(std::string modelFilePath);

	void PrepRender(RenderElement& RE) const;

	void SetMaterial(Material material);
	const Material& GetMaterial() const;

	void SetPosition(Vec3f position);
	void SetRotation(Quat rotation);
	void SetScale(Vec3f scale);

private:

	GLuint GetAttributeBuffer(AttributeIdentifier identifier) const;

	Asset<ModelResource> m_modelAsset;

	Material m_material;

	Vec3f m_position;
	Quat m_rotation;
	Vec3f m_scale;
};

inline void Model::SetMaterial(Material material)
{
	m_material = material;
}

inline const Material& Model::GetMaterial() const
{
	return m_material;
}

inline void Model::SetPosition(Vec3f position)
{
	m_position = position;
}

inline void Model::SetRotation(Quat rotation)
{
	m_rotation = rotation;
}

inline void Model::SetScale(Vec3f scale)
{
	m_scale = scale;
}

#endif // NANAKA_GRAPHICS_MODEL_H
