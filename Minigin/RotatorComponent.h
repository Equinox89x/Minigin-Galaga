#pragma once
#include <string>
#include <memory>
#include "TextObjectComponent.h"
#include "TransformComponent.h"

namespace dae
{
	class Font;
	class Texture2D;
	class RotatorComponent final : public Component
	{
	public:

		RotatorComponent() {};
		~RotatorComponent() {};
		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;

		virtual void Initialize() override;
		void Update() override;
		//virtual void FixedUpdate() override;
		//void Render() const override;

		glm::vec3 Rotate(const float cx, const float cy, float angle, glm::vec3 point);
		void SetOriginPoint() { m_OriginalPoint = GetGameObject()->GetTransform()->GetPosition(); };

	private:

		float m_MoveSpeed{ 1 };
		float m_Angle{0};
		glm::vec3 m_OriginalPoint{0,0,0};

	};
}