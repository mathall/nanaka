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

#include "gui/GUI.h"

#include "renderer/Renderer.h"
#include "renderer/RenderPipeline.h"
#include "renderer/RenderTarget.h"

GUI::GUI()
	: m_renderContextId(UUID::Invalid)
	, m_activeView(NULL)
{
}

void GUI::Initialize()
{
	SetActiveView(CreateView());

	m_renderContextId =
		g_renderer->GenerateRenderContext(OnScreenRenderTargetType);
}

void GUI::SetDisplayProperties(DisplayProperties displayProps)
{
	m_displayProperties = displayProps;

	g_renderer->SetViewportRect(m_renderContextId,
		Rect(Vec2f::Zero(), m_displayProperties.m_realSize));

	Projection projection;
	projection.SetViewDimensions(m_displayProperties.m_size);
	projection.SetUpVector(-Vec3f::UnitY());
	projection.SetViewDirection(Vec3f::UnitZ());
	projection.SetPosition(Vec3f(
		m_displayProperties.m_size.x / 2.0f,
		m_displayProperties.m_size.y / 2.0f,
		0.0f));
	g_renderer->SetProjection(m_renderContextId, projection);
}

void GUI::Draw()
{
	if (!g_renderer->StartRender(m_renderContextId))
	{
		return;
	}

	auto& renderPipeline = g_renderer->GetRenderPipeline(m_renderContextId);
	renderPipeline.SetDepthSortAxis(DepthSortAxisZ);
	m_activeView->EnsureLayout(m_displayProperties);
	m_activeView->Draw(renderPipeline);

	g_renderer->EndRender(m_renderContextId);
}

void GUI::HandleEvent(const InputEvent& event)
{
	InputEvent scaledEvent(event);
	scaledEvent.m_position *=
		m_displayProperties.m_size / m_displayProperties.m_realSize;
	m_activeView->HandleEvent(scaledEvent);
}

void GUI::SetActiveView(UUID viewId)
{
	auto viewIt = m_views.find(viewId);

	if (viewIt != m_views.end())
	{
		m_activeView = viewIt->second.get();
		m_activeView->Reset();
	}
}

void GUI::SetViewContent(UUID viewId, std::shared_ptr<Widget> content)
{
	auto viewIt = m_views.find(viewId);

	if (viewIt != m_views.end())
	{
		viewIt->second->SetContent(content);
	}
}

UUID GUI::RegisterFont(std::string fontFilePath)
{
	auto font = std::unique_ptr<Font>(new Font(fontFilePath));
	auto fontId = font->GetId();
	m_fonts.insert(std::make_pair(fontId, std::move(font)));
	return fontId;
}

UUID GUI::CreateView()
{
	auto view = std::unique_ptr<View>(new View());
	auto viewId = view->GetId();
	m_views.insert(std::make_pair(viewId, std::move(view)));
	return viewId;
}
