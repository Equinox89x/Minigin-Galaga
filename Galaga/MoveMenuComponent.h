#pragma once
#include "Component.h"
#include "Minigin.h"
#include "TransformComponent.h"
#include <glm/ext/vector_float3.hpp>

namespace dae
{
	class MoveMenuComponent final : public Component
	{

	public:
		MoveMenuComponent(float moveSpeed) : m_MoveSpeed{ moveSpeed } {};
		~MoveMenuComponent() = default;
		MoveMenuComponent(const MoveMenuComponent&) = delete;
		MoveMenuComponent(MoveMenuComponent&&) noexcept = delete;
		MoveMenuComponent& operator=(const MoveMenuComponent&) = delete;
		MoveMenuComponent& operator=(MoveMenuComponent&&) noexcept = delete;

	private:
		bool m_CanMove{ true };
		float m_MoveSpeed{ 1 };
		float m_Angle{ 0 };
		glm::vec3 m_OriginalPoint{ 0,0,0 };

		virtual void Initialize() override;
		virtual void Update() override;
		//virtual void Render() const override;
		//virtual void FixedUpdate() override;
		void SetOriginPoint() { m_OriginalPoint = GetGameObject()->GetTransform()->GetPosition(); };
	};
}

