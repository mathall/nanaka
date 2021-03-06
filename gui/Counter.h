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

#ifndef NANAKA_GUI_COUNTER_H
#define NANAKA_GUI_COUNTER_H

#include "gui/Widget.h"
#include "gui/DigitField.h"
#include "gui/StaticChar.h"

class Font;
class RenderData;

class Counter final : public Widget
{
public:

	Counter(
		const Font& font,
		float textSize,
		GUISizeUnit sizeUnit,
		unsigned int maxDigits,
		unsigned int decimals,
		char fillChar);

	void SetNumber(float num);
	void UpdateDigitFields();

protected:

	/**
	 * Widget implementation
	 */
	void OnPlacementUpdated() override;
	void OnDraw(RenderData& renderData) const override;

private:

	StaticChar m_dotChar;
	StaticChar m_subChar;
	bool m_printSub;

	DigitField m_NPart;
	DigitField m_FPart;
	bool m_hasFPart;

	int m_decPow;
};

inline void Counter::OnPlacementUpdated()
{
	UpdateDigitFields();
}

#endif // NANAKA_GUI_COUNTER_H
