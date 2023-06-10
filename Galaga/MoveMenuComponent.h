#pragma once
#include "Component.h"
#include "Minigin.h"
#include "TransformComponent.h"
#include <glm/ext/vector_float3.hpp>
#include "Scene.h"

namespace dae
{
	class MoveMenuComponent final : public Component
	{

	public:
		MoveMenuComponent(float moveSpeed, Scene* scene) : m_MoveSpeed{ moveSpeed }, m_Scene{ scene } {};
		~MoveMenuComponent() = default;
		MoveMenuComponent(const MoveMenuComponent&) = delete;
		MoveMenuComponent(MoveMenuComponent&&) noexcept = delete;
		MoveMenuComponent& operator=(const MoveMenuComponent&) = delete;
		MoveMenuComponent& operator=(MoveMenuComponent&&) noexcept = delete;

		void Reset();

	private:
		bool m_CanMove{ true };
		float m_MoveSpeed{ 1 };
		float m_Angle{ 0 };
		glm::vec3 m_OriginalPoint{ 0,0,0 };
		Scene* m_Scene;

		virtual void Initialize() override;
		virtual void Update() override;
		//virtual void Render() const override;
		//virtual void FixedUpdate() override;
		void SetOriginPoint() { m_OriginalPoint = GetGameObject()->GetTransform()->GetPosition(); };
	};
}

