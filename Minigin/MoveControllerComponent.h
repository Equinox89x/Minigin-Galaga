#pragma once
#include <string>
#include <memory>
#include "TransformComponent.h"

namespace dae
{
	class MoveControllerComponent final : public Component
	{
	public:

		MoveControllerComponent(glm::vec3 startPos) : m_StartPos{ startPos }, m_Movespeed{ 0,0,0 } {}
		~MoveControllerComponent() {};
		MoveControllerComponent(const MoveControllerComponent& other) = delete;
		MoveControllerComponent(MoveControllerComponent&& other) = delete;
		MoveControllerComponent& operator=(const MoveControllerComponent& other) = delete;
		MoveControllerComponent& operator=(MoveControllerComponent&& other) = delete;

		void Update() override;
		virtual void FixedUpdate() override;
		void Render() const override;

		void SetMoveSpeed(const glm::vec3& movespeed);


	private:
		void UpdatePos(float dt);
		glm::vec3 m_StartPos;
		glm::vec3 m_Movespeed;

	};
}