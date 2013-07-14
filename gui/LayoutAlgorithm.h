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

#ifndef NANAKA_GUI_LAYOUTALGORITHM_H
#define NANAKA_GUI_LAYOUTALGORITHM_H

#include <unordered_map>
#include <memory>
#include <vector>

#include "gui/DisplayProperties.h"
#include "gui/LayoutMethod.h"
#include "gui/LayoutProperties.h"
#include "math/Vec2f.h"
#include "utils/UUID.h"

class Widget;

class WidgetPlacement
{
public:

	Vec2f m_position;
	Vec2f m_size;
	Vec2f m_depthRange;
	float m_depth;
};

class LayoutAlgorithm final
{
public:

	void CalculateChildrenPlacements(
		const std::vector<std::shared_ptr<Widget>>& children,
		const Vec2f position,
		const Vec2f size,
		const Vec2f depthRange,
		std::unordered_map<UUID, WidgetPlacement>& childrenPlacements);

	LayoutProperties CalculateLayoutProperties(
		const std::vector<std::shared_ptr<Widget>>& children,
		const LayoutProperties layoutProps,
		const DisplayProperties displayProps);

	LayoutMethod m_layoutMethod;

private:

	/**
	 * Relative placement will set widget size and position relative to its
	 * parent. A child widget's desired size will take precedence over its
	 * relative layout properties. Children will be sorted by their z value, and
	 * given a depth range accordingly.
	 */
	void CalculateChildrenPlacementsRelative(
		const std::vector<std::shared_ptr<Widget>>& children,
		const Vec2f position,
		const Vec2f size,
		const Vec2f depthRange,
		std::unordered_map<UUID, WidgetPlacement>& childrenPlacements);

	LayoutProperties CalculateLayoutPropertiesRelative(
		const std::vector<std::shared_ptr<Widget>>& children,
		const LayoutProperties layoutProps,
		const DisplayProperties displayProps);

	/**
	 * Linear placement will take the widget's available size, vertical or
	 * horizontal depending on linear orientation, and distribute it over its
	 * children while taking their weights into account and respecting their
	 * desired sizes.
	 */
	void CalculateChildrenPlacementsLinear(
		const std::vector<std::shared_ptr<Widget>>& children,
		const Vec2f position,
		const Vec2f size,
		const Vec2f depthRange,
		std::unordered_map<UUID, WidgetPlacement>& childrenPlacements);

	LayoutProperties CalculateLayoutPropertiesLinear(
		const std::vector<std::shared_ptr<Widget>>& children,
		const LayoutProperties layoutProps,
		const DisplayProperties displayProps);

	float DistributeSize(
		const std::vector<std::shared_ptr<Widget>>& children,
		const Vec2f size,
		const bool distributingHeight,
		std::unordered_map<UUID, Vec2f>& childSizes);

	int NumVisibleChildren(
		const std::vector<std::shared_ptr<Widget>>& children);

	// Kept as members to avoid unnecessary heap allocations.
	std::unordered_map<UUID, Vec2f> m_childSizes;
	std::unordered_map<UUID, bool> m_lockedSizes;
};

#endif // NANAKA_GUI_LAYOUTALGORITHM_H
