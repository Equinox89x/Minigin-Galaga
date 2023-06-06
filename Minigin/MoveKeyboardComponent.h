#pragma once
#include <string>
#include <memory>
#include "TransformComponent.h"

namespace dae
{
	class MoveKeyboardComponent final : public Component
	{
	public:

		MoveKeyboardComponent(glm::vec3 startPos) : m_StartPos{ startPos }, m_Movespeed { 0,0,0 } {}
		~MoveKeyboardComponent() {};
		MoveKeyboardComponent(const MoveKeyboardComponent& other) = delete;
		MoveKeyboardComponent(MoveKeyboardComponent&& other) = delete;
		MoveKeyboardComponent& operator=(const MoveKeyboardComponent& other) = delete;
		MoveKeyboardComponent& operator=(MoveKeyboardComponent&& other) = delete;

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