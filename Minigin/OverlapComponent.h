#pragma once
#include "Component.h"
#include "Scene.h"
#include <SDL_rect.h>

namespace dae
{
	class OverlapComponent final : public Component
	{

	public:
		OverlapComponent(Scene* scene) : m_Scene{ scene } {};
		~OverlapComponent() = default;
		OverlapComponent(const OverlapComponent&) = delete;
		OverlapComponent(OverlapComponent&&) noexcept = delete;
		OverlapComponent& operator=(const OverlapComponent&) = delete;
		OverlapComponent& operator=(OverlapComponent&&) noexcept = delete;

		std::vector<GameObject*> GetOverlappingObjects(bool hasDimensions = false);
		bool IsOverlap(const SDL_Rect& square1, const SDL_Rect& square2);

		void Update() override;

	private:
		Scene* m_Scene{ nullptr };
	};
}
