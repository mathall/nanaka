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

#include "gui/LayoutAlgorithm.h"

void LayoutAlgorithm::CalculateChildrenPlacements(
	const std::vector<std::shared_ptr<Widget>>& children,
	const Vec2f position,
	const Vec2f size,
	const Vec2f depthRange,
	std::unordered_map<UUID, WidgetPlacement>& childrenPlacements)
{
	switch (m_layoutMethod.m_layoutAlgorithm)
	{
	case LayoutMethod::Relative:
		CalculateChildrenPlacementsRelative(
			children, position, size, depthRange, childrenPlacements);
		break;
	case LayoutMethod::Linear:
		CalculateChildrenPlacementsLinear(
			children, position, size, depthRange, childrenPlacements);
		break;
	}
}

LayoutProperties LayoutAlgorithm::CalculateLayoutProperties(
	const std::vector<std::shared_ptr<Widget>>& children,
	const LayoutProperties layoutProps,
	const DisplayProperties displayProps)
{
	LayoutProperties newLayoutProps;

	switch (m_layoutMethod.m_layoutAlgorithm)
	{
	case LayoutMethod::Relative:
		newLayoutProps = CalculateLayoutPropertiesRelative(
			children, layoutProps, displayProps);
		break;
	case LayoutMethod::Linear:
		newLayoutProps = CalculateLayoutPropertiesLinear(
			children, layoutProps, displayProps);
		break;
	}

	return newLayoutProps;
}
