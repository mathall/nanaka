#include "renderer/RenderResourceManager.h"

#include "renderer/FrameBufferRenderResource.h"
#include "renderer/MeshRenderResource.h"
#include "renderer/Renderer.h"
#include "renderer/ShaderRenderResource.h"
#include "renderer/ShaderProgramRenderResource.h"
#include "renderer/TextureRenderResource.h"

RenderResourceHandle RenderResourceManager::GenerateTexture(
	int width,
	int height,
	std::unique_ptr<GLvoid> pixels)
{
	auto renderResource = std::unique_ptr<TextureRenderResource>(
		new TextureRenderResource(width, height, std::move(pixels)));
	auto resourceHandle = renderResource->GetHandle();

	m_renderResources.insert(std::make_pair(
		resourceHandle, std::move(renderResource)));
	m_buildQueue.push_back(resourceHandle);

	return resourceHandle;
}

RenderResourceHandle RenderResourceManager::GenerateFrameBuffer(Vec2f size)
{
	auto renderResource = std::unique_ptr<FrameBufferRenderResource>(
		new FrameBufferRenderResource(size));
	auto resourceHandle = renderResource->GetHandle();

	m_renderResources.insert(std::make_pair(
		resourceHandle, std::move(renderResource)));
	m_buildQueue.push_back(resourceHandle);

	return resourceHandle;
}

RenderResourceHandle RenderResourceManager::GenerateMesh(
	std::unique_ptr<GLfloat[]> vertexBuffer,
	int vertexBufferSize,
	std::unique_ptr<GLfloat[]> texcoordBuffer,
	int texcoordBufferSize,
	std::unique_ptr<GLushort[]> indexBuffer,
	int indexBufferSize)
{
	auto renderResource = std::unique_ptr<MeshRenderResource>(
		new MeshRenderResource(
			std::move(vertexBuffer), vertexBufferSize,
			std::move(texcoordBuffer), texcoordBufferSize,
			std::move(indexBuffer), indexBufferSize));
	auto resourceHandle = renderResource->GetHandle();

	m_renderResources.insert(std::make_pair(
		resourceHandle, std::move(renderResource)));
	m_buildQueue.push_back(resourceHandle);

	return resourceHandle;
}

RenderResourceHandle RenderResourceManager::GenerateShader(
	GLuint type,
	std::string source,
	std::vector<std::string> uniformNames,
	std::vector<std::string> attributeNames)
{
	auto renderResource = std::unique_ptr<ShaderRenderResource>(
		new ShaderRenderResource(type, source, uniformNames, attributeNames));
	auto resourceHandle = renderResource->GetHandle();

	m_renderResources.insert(std::make_pair(
		resourceHandle, std::move(renderResource)));
	m_buildQueue.push_back(resourceHandle);

	return resourceHandle;
}

RenderResourceHandle RenderResourceManager::GenerateShaderProgram(
	RenderResourceHandle vertexShaderHandle,
	RenderResourceHandle fragmentShaderHandle)
{
	auto renderResource = std::unique_ptr<ShaderProgramRenderResource>(
		new ShaderProgramRenderResource(
			vertexShaderHandle, fragmentShaderHandle));
	auto resourceHandle = renderResource->GetHandle();

	m_renderResources.insert(std::make_pair(
		resourceHandle, std::move(renderResource)));
	m_buildQueue.push_back(resourceHandle);

	return resourceHandle;
}

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
