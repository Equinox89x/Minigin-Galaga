#include <stdexcept>
#include "TextObjectComponent.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "Font.h"
#include <iostream>

dae::TextObjectComponent::TextObjectComponent(const std::string& text, std::shared_ptr<Font> font)
	: m_needsUpdate(true), m_text(text), m_font(std::move(font)), m_textTexture(nullptr)
{ }

void dae::TextObjectComponent::Update()
{
	if (m_needsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_Color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_textTexture = std::make_unique<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void dae::TextObjectComponent::Render() const
{
	if (m_textTexture != nullptr)
	{
		auto pos = GetGameObject()->GetTransform()->GetWorldPosition();
		//const auto& pos2 = GetGameObject()->GetTransform()->GetPosition();
		//pos.x += pos2.x;
		//pos.y += pos2.y;
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextObjectComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextObjectComponent::SetPosition(const float x, const float y)
{
	GetGameObject()->GetTransform()->Translate(x, y, 0.0f);
	m_needsUpdate = true;
}


