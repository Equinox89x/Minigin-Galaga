#pragma once
#include "Component.h"
#include <string>
#include <memory>
#include "Transform.h"
#include "Font.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Texture2D.h"
#include "TransformComponent.h"


namespace dae {

	class Font;
	class Texture2D;
	class TextureComponent : public Component
	{
	public:
		TextureComponent() {};
		//TextureComponent(const std::string& filename, glm::vec2 position = { 0,0 }, glm::vec2 scale = { 1,1 }, const float rotation = 0, float animSpeed = 0.3f, int nrOfFrames = 1);
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		void Update();
		//void Initialize();
		void Render() const;

		void SetTexture(const std::string& filename, float animSpeed = 0.3f, int nrOfFrames = 1, bool resetAnim = true);
		void SetPosition(const float x, const float y);
		void AddPosition(const float x, const float y);
		void SetWorldPosition(const float x, const float y);
		void Scale(const float x, const float y);
		void Rotate(const float angle);

		void SetNrOfFrames(int nrOfFrames) { NrOfFrames = nrOfFrames; };
		const SDL_Rect& GetRect() { HandleAnimation(); return m_Rect; };
		void SetIsVisible(bool isvisible) { Isvisible = isvisible; };
		bool GetIsVisible() { return Isvisible; };
		void SetOffset(glm::vec2 offset) { Offset = offset; };

		void SetPaused(bool isPaused) { IsPaused = isPaused; };
		void SetFrame(int frameNr) { CurrentFrame = frameNr; HandleAnimation(); };

	private:
		std::string FileName{};
		std::shared_ptr<Texture2D> m_pTexture{};
		bool m_needsUpdate{ false };
		float Angle{ 0 };
		int NrOfFrames{ 1 };
		int CurrentFrame{ 1 };
		float DefaultAnimTime{ 0.3f };
		float AnimTimer{ DefaultAnimTime };
		SDL_Rect m_Rect{};
		SDL_Rect m_SrcRect{};
		SDL_Rect m_DstRect{};
		glm::vec2 m_Position{};
		glm::vec2 m_Scale{  };

		bool Isvisible{ true }, IsPaused{ false };
		glm::vec2 Offset;

		void HandleAnimation();

	};
}

