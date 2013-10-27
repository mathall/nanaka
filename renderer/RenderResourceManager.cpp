#include "renderer/RenderResourceManager.h"

#include <cassert>

void RenderResourceManager::DestroyResource(RenderResourceHandle resourceHandle)
{
	m_destroyQueue.push_back(resourceHandle);
}

void RenderResourceManager::QueueBuiltResourcesForRebuild()
{
	for (auto it = m_renderResources.begin();
		it != m_renderResources.end(); ++it)
	{
		if (it->second->IsBuilt())
		{
			it->second->ResetBuildStatus();
			m_buildQueue.push_back(it->second->GetHandle());
		}
	}
}

void RenderResourceManager::ProcessQueues()
{
	// Process build queue until no resource is built through one iteration.
	bool continueBuilding = true;
	while (continueBuilding)
	{
		continueBuilding = false;

		m_failedBuilds.clear();

		for (auto resourceHandle : m_buildQueue)
		{
			auto renderResourceIt = m_renderResources.find(resourceHandle);
			if (renderResourceIt == m_renderResources.end())
			{
				assert(!"Resource does not exist, already destroyed?");
			}
			else if (renderResourceIt->second->IsBuilt())
			{
				assert(!"Resource was already built.");
			}
			else
			{
				renderResourceIt->second->BuildInternal(*this);

				if (renderResourceIt->second->IsBuilt())
				{
					continueBuilding = true;
				}
				else
				{
					m_failedBuilds.push_back(resourceHandle);
				}
			}
		}

		m_buildQueue.clear();
		m_buildQueue = m_failedBuilds;
	}

	for (auto resourceHandle : m_destroyQueue)
	{
		auto renderResourceIt = m_renderResources.find(resourceHandle);
		if (renderResourceIt != m_renderResources.end())
		{
			assert(!"Resource does not exist, already destroyed?");
		}
		else if (renderResourceIt->second->IsBuilt())
		{
			renderResourceIt->second->DestroyInternal();
			m_renderResources.erase(renderResourceIt);
		}
		else
		{
			assert(!"Resource is not built.");
		}
	}
	m_destroyQueue.clear();
}

RenderResource* RenderResourceManager::Get(
	RenderResourceHandle resourceHandle) const
{
	auto renderResourceIt = m_renderResources.find(resourceHandle);
	if (renderResourceIt != m_renderResources.end())
	{
		return renderResourceIt->second.get();
	}

	return NULL;
}
