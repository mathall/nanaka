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

#ifndef NANAKA_GUI_GAUGE_H
#define NANAKA_GUI_GAUGE_H

#include <memory>
#include <string>

#include "gui/ImageComponent.h"
#include "gui/Widget.h"

class Panel;
class Image;

class Gauge final : public Widget
{
public:

	Gauge(
		bool vertical,
		float width,
		float endWidth,
		GUISizeUnit sizeUnit,
		std::string fillEndTex,
		std::string emptyEndTex,
		std::string middleFullTex,
		std::string middleEmptyTex,
		Vec4f color);

	void SetLoad(float load);

protected:

	/**
	 * Widget implementation.
	 */
	void OnPlacementUpdated() override;
	void OnDraw(RenderPipeline& renderPipeline) const override;
	void OnAlphaUpdated(float totalAlpha) override;

private:

	void UpdateMiddle();

	std::shared_ptr<Image> m_endFull;
	std::shared_ptr<Image> m_endEmpty;

	bool m_vertical;
	Vec2f m_endSize;
	ImageComponent m_middleFull;
	ImageComponent m_middleEmpty;

	float m_load;
};

inline void Gauge::OnPlacementUpdated()
{
	UpdateMiddle();
}

#endif // NANAKA_GUI_GAUGE_H
