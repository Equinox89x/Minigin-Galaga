#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "Component.h"
#include "Texture2D.h"
#include <SDL_ttf.h>

namespace dae
{
	class Font;
	class Texture2D;
	class TextObjectComponent : public Component
	{
	public:
		TextObjectComponent(const std::string& text, std::shared_ptr<Font> font);
		virtual ~TextObjectComponent() = default;
		TextObjectComponent(const TextObjectComponent& other) = delete;
		TextObjectComponent(TextObjectComponent&& other) = delete;
		TextObjectComponent& operator=(const TextObjectComponent& other) = delete;
		TextObjectComponent& operator=(TextObjectComponent&& other) = delete;

		//virtual void Initialize() override;
		//virtual void FixedUpdate() override;
		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetPosition(const float x, const float y);
		void SetName(std::string name) { m_ComponentName = name; };
		void SetColor(SDL_Color color) { m_Color = color; };

		void SetIsVisible(bool isvisible) { Isvisible = isvisible; };
		bool GetIsVisible() { return Isvisible; };

	private:
		bool m_needsUpdate;
		std::string m_text;
		glm::vec3 m_Position{};
		std::shared_ptr<Font> m_font;
		std::unique_ptr<Texture2D> m_textTexture;
		std::string m_ComponentName;
		SDL_Color m_Color{ 255,255,255 };
		bool Isvisible;
	};
}
