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

LayoutProperties LayoutAlgorithm::CalculateLayoutPropertiesLinear(
	const std::vector<std::shared_ptr<Widget>>& children,
	const LayoutProperties layoutProps,
	const DisplayProperties displayProps)
{
	LayoutProperties newLayoutProps = layoutProps;

	float desiredW = 0.0f;
	float maxDesiredW = 0.0f;
	float desiredH = 0.0f;
	float maxDesiredH = 0.0f;

	newLayoutProps.m_childGrowHorizontally = false;
	newLayoutProps.m_childGrowVertically = false;

	for (auto child : children)
	{
		if (child->IsVisible())
		{
			auto& childLayoutProps = child->GetLayoutPropertiesConstRef();

			newLayoutProps.m_childGrowHorizontally |= childLayoutProps.GrowH();
			newLayoutProps.m_childGrowVertically |= childLayoutProps.GrowV();

			auto childDesiredPixelSize = childLayoutProps.m_desiredPixelSize;
			switch (m_layoutMethod.m_orientation)
			{
			case LayoutMethod::Horizontal:
				desiredW += childDesiredPixelSize.x;
				maxDesiredH = std::max(maxDesiredH, childDesiredPixelSize.y);
				break;

			case LayoutMethod::Vertical:
				maxDesiredW = std::max(maxDesiredW, childDesiredPixelSize.x);
				desiredH += childDesiredPixelSize.y;
				break;
			}
		}
	}

	desiredW += maxDesiredW;
	desiredH += maxDesiredH;

	newLayoutProps.m_desiredPixelSize = Vec2f(desiredW, desiredH);
	return newLayoutProps;
}

void LayoutAlgorithm::CalculateChildrenPlacementsLinear(
	const std::vector<std::shared_ptr<Widget>>& children,
	const Vec2f position,
	const Vec2f size,
	const Vec2f depthRange,
	std::unordered_map<UUID, WidgetPlacement>& childrenPlacements)
{
	float nextPos = 0.0f;
	float spaceLeft = 0.0f;
	float spacer = 0.0f;

	Vec2f childDepthRange(
		(depthRange.x - depthRange.y) / 2.0f + depthRange.y, depthRange.y);

	m_childSizes.clear();

	// Distribute the available height/width among the widgets.
	switch (m_layoutMethod.m_orientation)
	{
	case LayoutMethod::Horizontal:
		spaceLeft = DistributeSize(children, size, false, m_childSizes);

		switch (m_layoutMethod.m_HAlignment)
		{
		case LayoutMethod::Left:
			nextPos = position.x;
			break;

		case LayoutMethod::HCenter:
			nextPos = position.x + spaceLeft * 0.5f;
			break;

		case LayoutMethod::Right:
			nextPos = position.x + spaceLeft;
			break;

		case LayoutMethod::HFillout:
			spacer = spaceLeft / NumVisibleChildren(children);
			nextPos = position.x + spacer * 0.5f;
			break;
		}

		break;

	case LayoutMethod::Vertical:
		spaceLeft = DistributeSize(children, size, true, m_childSizes);

		switch (m_layoutMethod.m_VAlignment)
		{
		case LayoutMethod::Top:
			nextPos = position.y;
			break;

		case LayoutMethod::VCenter:
			nextPos = position.y + spaceLeft * 0.5f;
			break;

		case LayoutMethod::Bottom:
			nextPos = position.y + spaceLeft;
			break;

		case LayoutMethod::VFillout:
			spacer = spaceLeft / NumVisibleChildren(children);
			nextPos = position.y + spacer * 0.5f;
			break;
		}

		break;
	}

	// Place the widgets according to the orientation.
	for (auto child : children)
	{
		WidgetPlacement childPlacement;

		if (child->IsVisible())
		{
			auto childSize = m_childSizes[child->GetId()];

			float x = 0.0f;
			float y = 0.0f;

			switch (m_layoutMethod.m_orientation)
			{
			case LayoutMethod::Horizontal:
				x = nextPos;
				nextPos += childSize.x + spacer;
				switch (m_layoutMethod.m_VAlignment)
				{
				case LayoutMethod::Top:
					y = position.y;
					break;

				case LayoutMethod::Bottom:
					y = position.y + (size.y - childSize.y);
					break;

				case LayoutMethod::HCenter:
				case LayoutMethod::VFillout:
					y = position.y + (size.y - childSize.y) * 0.5f;
					break;
				}
				break;

			case LayoutMethod::Vertical:
				y = nextPos;
				nextPos += childSize.y + spacer;
				switch (m_layoutMethod.m_HAlignment)
				{
				case LayoutMethod::Left:
					x = position.x;
					break;

				case LayoutMethod::Right:
					x = position.x + (size.x - childSize.x);
					break;

				case LayoutMethod::VCenter:
				case LayoutMethod::HFillout:
					x = position.x + (size.x - childSize.x) * 0.5f;
					break;
				}
				break;
			}

			childPlacement.m_position = Vec2f(x, y);
			childPlacement.m_size = Vec2f(childSize.x, childSize.y);
		}

		childPlacement.m_depthRange = childDepthRange;
		childPlacement.m_depth = childDepthRange.x;
		childrenPlacements.insert(
			std::make_pair(child->GetId(), childPlacement));
	}
}

// Distributes a given height with respect to a certain width among the widgets
// ..or the other way around.
float LayoutAlgorithm::DistributeSize(
	const std::vector<std::shared_ptr<Widget>>& children,
	const Vec2f size,
	bool distributingHeight,
	std::unordered_map<UUID, Vec2f>& childSizes)
{
	m_lockedSizes.clear();

	// Initialize and sum up weight of widgets which will receive size.
	float sumWeight = 0.0f;
	for (auto child : children)
	{
		m_lockedSizes[child->GetId()] = true;
		childSizes[child->GetId()] = Vec2f::Zero();
		if (child->IsVisible())
		{
			auto& childLayoutProps = child->GetLayoutPropertiesConstRef();
			sumWeight += childLayoutProps.m_sizeWeight;
			m_lockedSizes[child->GetId()] = false;
		}
	}

	float spareSize = distributingHeight ? size.y : size.x;

	// Distribute the available size among the widgets.
	while (spareSize > 0.001f && sumWeight > 0.0f)
	{
		float sizeSlot = spareSize / sumWeight;

		for (auto child : children)
		{
			auto& childLayoutProps = child->GetLayoutPropertiesConstRef();

			auto locked = m_lockedSizes[child->GetId()];
			float childSize = distributingHeight ?
				childSizes[child->GetId()].y : childSizes[child->GetId()].x;

			// Distribute slots to those widgets who still wants to be bigger
			if (!locked)
			{
				float addSize = sizeSlot * childLayoutProps.m_sizeWeight;

				childSize += addSize;
				spareSize -= addSize;

				// See if the widget has grown enough.
				float desiredSize = childSize;
				if (distributingHeight)
				{
					float desiredHeightAtWidth =
						childLayoutProps.DesiredHeightAtWidth(size.x);
					if (desiredHeightAtWidth > 0.0f)
					{
						desiredSize = std::min(
							desiredSize, desiredHeightAtWidth);
					}
				}
				else
				{
					float desiredWidthAtHeight =
						childLayoutProps.DesiredWidthAtHeight(size.y);
					if (desiredWidthAtHeight > 0.0f)
					{
						desiredSize = std::min(
							desiredSize, desiredWidthAtHeight);
					}
				}

				// Lock widget growth and give back excess width if their
				// maximum has been reached.
				if (desiredSize < childSize)
				{
					float excessSize = childSize - desiredSize;

					childSize -= excessSize;
					spareSize += excessSize;

					locked = true;
					sumWeight -= childLayoutProps.m_sizeWeight;
				}
			}

			m_lockedSizes[child->GetId()] = locked;
			childSizes[child->GetId()] = Vec2f(childSize);
		}
	}

	// Set the widgets' widths according to the heights handed to them.
	// ..or the other way around.
	for (auto child : children)
	{
		auto& childLayoutProps = child->GetLayoutPropertiesConstRef();

		auto childSize = childSizes[child->GetId()];

		if (distributingHeight)
		{
			childSize.x = size.x;
			if (childLayoutProps.m_aspectRatio > 0.0f)
			{
				childSize.x = childSize.y * childLayoutProps.m_aspectRatio;
			}
			else if (!childLayoutProps.FillH())
			{
				childSize.x = std::min(
					childSize.x, childLayoutProps.m_desiredPixelSize.x);
			}
		}
		else
		{
			childSize.y = size.y;
			if (childLayoutProps.m_aspectRatio > 0.0f)
			{
				childSize.y = childSize.x / childLayoutProps.m_aspectRatio;
			}
			else if (!childLayoutProps.FillV())
			{
				childSize.y = std::min(
					childSize.y, childLayoutProps.m_desiredPixelSize.y);
			}
		}

		childSizes[child->GetId()] = childSize;
	}

	return spareSize;
}

int LayoutAlgorithm::NumVisibleChildren(
	const std::vector<std::shared_ptr<Widget>>& children)
{
	return std::count_if(children.begin(), children.end(),
		[](const std::shared_ptr<Widget>& child){ return child->IsVisible(); });
}
