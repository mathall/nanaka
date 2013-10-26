#include "renderer/RenderResourceManager.h"

#include "renderer/FrameBufferRenderResource.h"
#include "renderer/MeshRenderResource.h"
#include "renderer/Renderer.h"
#include "renderer/TextureRenderResource.h"

RenderResourceHandle RenderResourceManager::GenerateTexture(
	int width,
	int height,
	std::unique_ptr<GLvoid> pixels)
{
	auto renderResource = std::make_shared<TextureRenderResource>(
		width, height, std::move(pixels));
	auto resourceHandle = renderResource->GetHandle();

	m_renderResources.insert(std::make_pair(resourceHandle, renderResource));
	g_renderer->QueueGLResourceForBuild(renderResource);

	return resourceHandle;
}

RenderResourceHandle RenderResourceManager::GenerateFrameBuffer()
{
	auto renderResource = std::make_shared<FrameBufferRenderResource>();
	auto resourceHandle = renderResource->GetHandle();

	m_renderResources.insert(std::make_pair(resourceHandle, renderResource));
	g_renderer->QueueGLResourceForBuild(renderResource);

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
	auto renderResource = std::make_shared<MeshRenderResource>(
		std::move(vertexBuffer), vertexBufferSize,
		std::move(texcoordBuffer), texcoordBufferSize,
		std::move(indexBuffer), indexBufferSize);
	auto resourceHandle = renderResource->GetHandle();

	m_renderResources.insert(std::make_pair(resourceHandle, renderResource));
	g_renderer->QueueGLResourceForBuild(renderResource);

	return resourceHandle;
}

void RenderResourceManager::DestroyResource(RenderResourceHandle resourceHandle)
{
	auto renderResourceIt = m_renderResources.find(resourceHandle);
	if (renderResourceIt != m_renderResources.end())
	{
		auto resource = renderResourceIt->second;
		m_renderResources.erase(renderResourceIt);
		g_renderer->QueueGLResourceForDestruction(resource);
	}
}

std::shared_ptr<RenderResource> RenderResourceManager::Get(
	RenderResourceHandle resourceHandle) const
{
	auto renderResourceIt = m_renderResources.find(resourceHandle);
	if (renderResourceIt != m_renderResources.end())
	{
		return renderResourceIt->second;
	}

	return nullptr;
}
