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

#include <algorithm>

#include "gui/Widget.h"

LayoutProperties LayoutAlgorithm::CalculateLayoutPropertiesRelative(
	const std::vector<std::shared_ptr<Widget>>& children,
	const LayoutProperties layoutProps,
	const DisplayProperties displayProps)
{
	LayoutProperties newLayoutProps = layoutProps;

	auto desiredPixelSize = layoutProps.DesiredPixelSize(displayProps.m_dpi);
	float maxDesiredW = desiredPixelSize.x;
	float maxDesiredH = desiredPixelSize.y;

	for (auto child : children)
	{
		if (child->IsVisible())
		{
			auto& childLayoutProps = child->GetLayoutPropertiesConstRef();
			auto childDesiredPixelSize = childLayoutProps.m_desiredPixelSize;
			maxDesiredW = std::max(maxDesiredW, childDesiredPixelSize.x);
			maxDesiredH = std::max(maxDesiredH, childDesiredPixelSize.y);
		}
	}

	newLayoutProps.m_childGrowHorizontally = false;
	newLayoutProps.m_childGrowVertically = false;
	newLayoutProps.m_desiredPixelSize = Vec2f(maxDesiredW, maxDesiredH);
	return newLayoutProps;
}

void LayoutAlgorithm::CalculateChildrenPlacementsRelative(
	const std::vector<std::shared_ptr<Widget>>& children,
	const Vec2f position,
	const Vec2f size,
	const Vec2f depthRange,
	std::unordered_map<UUID, WidgetPlacement>& childrenPlacements)
{
	const float childDepthRangeSize =
		(depthRange.x - depthRange.y) / (children.size() + 1);
	float childDepthRangeStart = depthRange.x - childDepthRangeSize;

	auto zSortedChildren = children;

	std::sort(zSortedChildren.begin(), zSortedChildren.end(),
	[](const std::shared_ptr<Widget>& a, const std::shared_ptr<Widget>& b)
	{
		return a->GetLayoutPropertiesConstRef().m_z
			< b->GetLayoutPropertiesConstRef().m_z;
	});

	for (auto child : zSortedChildren)
	{
		WidgetPlacement childPlacement;

		if (child->IsVisible())
		{
			auto& childLayoutProps = child->GetLayoutPropertiesConstRef();

			Vec2f childSize = childLayoutProps.m_relSize * size;

			if (!childLayoutProps.m_desiredSize.AnyEquals(0.0f))
			{
				if (childLayoutProps.m_desiredPixelSize.x < childSize.x
					&& childLayoutProps.m_desiredPixelSize.y < childSize.y)
				{
					childSize = childLayoutProps.m_desiredPixelSize;
				}
			}

			childPlacement.m_position =
				position + childLayoutProps.m_relPosition * size;
			childPlacement.m_size = childSize;
		}

		float maxDepth = childDepthRangeStart;
		float minDepth = childDepthRangeStart - childDepthRangeSize;

		childPlacement.m_depthRange = Vec2f(maxDepth, minDepth);
		childPlacement.m_depth = maxDepth;
		childrenPlacements.insert(
			std::make_pair(child->GetId(), childPlacement));

		childDepthRangeStart -= childDepthRangeSize;
	}
}
