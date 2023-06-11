#pragma once
#include "Component.h"
#include <glm/ext/vector_float2.hpp>
namespace dae {

	class BackgroundComponent final : public Component
	{
	public:
		BackgroundComponent(float moveSpeed, glm::vec2 startPosition, glm::vec2 initialPosition) : m_MoveSpeed{ moveSpeed }, m_StartPosition{ startPosition }, m_InitialPosition{ initialPosition } {};
		~BackgroundComponent() = default;
		BackgroundComponent(const BackgroundComponent&) = delete;
		BackgroundComponent(BackgroundComponent&&) noexcept = delete;
		BackgroundComponent& operator=(const BackgroundComponent&) = delete;
		BackgroundComponent& operator=(BackgroundComponent&&) noexcept = delete;

		void Initialize() override;
		void Update() override;

	private:
		float m_MoveSpeed{ 0 };
		glm::vec2 m_InitialPosition{ 0,0 };
		glm::vec2 m_StartPosition{ 0,0 };
	};
}
