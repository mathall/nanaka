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

#ifndef NANAKA_GUI_IMAGE_H
#define NANAKA_GUI_IMAGE_H

#include <string>

#include "gui/ImageComponent.h"
#include "gui/Widget.h"
#include "math/Vec4f.h"

class Image final : public Widget
{
public:

	Image(
		std::string textureFilePath,
		Vec4f color,
		Vec2f size,
		GUISizeUnit sizeUnit,
		ImageRotation rotation = ImageRotationNone,
		bool flipH = false,
		bool flipV = false);

protected:

	/**
	 * Widget implementation.
	 */
	void OnPlacementUpdated() override;
	void OnDraw(RenderData& renderData) const override;
	void OnAlphaUpdated(float totalAlpha) override;

private:

	ImageComponent m_image;
};

inline void Image::OnPlacementUpdated()
{
	m_image.SetPlacement(m_position, m_size, m_depth);
}

inline void Image::OnDraw(RenderData& renderData) const
{
	m_image.Draw(renderData);
}

inline void Image::OnAlphaUpdated(float totalAlpha)
{
	m_image.SetAlpha(totalAlpha);
}

#endif // NANAKA_GUI_IMAGE_H
