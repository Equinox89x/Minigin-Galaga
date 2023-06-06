#include "RotatorComponent.h"
#include "TransformComponent.h"
#include "Timer.h"

void dae::RotatorComponent::Initialize()
{
	SetOriginPoint();
}

void dae::RotatorComponent::Update()
{
	m_Angle += Timer::GetInstance().GetDeltaTime() * m_MoveSpeed;
	glm::vec3 pos{ Rotate(10, -130, m_Angle, m_OriginalPoint) };
	GameObject* go{ GetGameObject() };
	go->GetTransform()->Translate(pos);
}

glm::vec3 dae::RotatorComponent::Rotate(const float cx, const float cy, float angle, glm::vec3 point)
{
	float s = sin(angle);
	float c = cos(angle);

	// translate point back to origin:
	point.x -= cx;
	point.y -= cy;

	// rotate point
	float xnew = point.x * c - point.y * s;
	float ynew = point.x * s + point.y * c;

	// translate point back:
	point.x = xnew + cx;
	point.y = ynew + cy;
	return point;
}


