#include "Timer.h"
#include "MoveMenuComponent.h"

void dae::MoveMenuComponent::Update()
{
	if (m_CanMove) {
		m_Angle += Timer::GetInstance().GetDeltaTime() * m_MoveSpeed;
		glm::vec3 pos{ Move(0, m_Angle, m_OriginalPoint) };
		GameObject* go{ GetGameObject() };
		go->GetTransform()->Translate(pos);

		if (pos.y <= -m_OriginalPoint.y) {
			m_CanMove = false;
		}
	}
}

void dae::MoveMenuComponent::Initialize()
{
	SetOriginPoint();
}


glm::vec3 dae::MoveMenuComponent::Move(const float cy, float angle, glm::vec3 point)
{
	float c = cosf(angle);

	// translate point back to origin:
	point.y -= cy;

	// move point
	float ynew = point.y * c;

	// translate point back:
	point.y = ynew + cy;
	return point;
}

