#include "Timer.h"
#include "MoveMenuComponent.h"

void dae::MoveMenuComponent::Update()
{
	if (m_CanMove) {
		/*	m_Angle += Timer::GetInstance().GetDeltaTime() * m_MoveSpeed;
			glm::vec3 pos{ Move(0, m_Angle, m_OriginalPoint) };
			GameObject* go{ GetGameObject() };
			go->GetTransform()->Translate(pos);

			if (pos.y <= -m_OriginalPoint.y) {
				m_CanMove = false;
			}*/

		auto dt{ Timer::GetInstance().GetDeltaTime() };
		auto move{ dt * m_MoveSpeed };
		GetGameObject()->GetTransform()->AddTranslate(0, -move);
		if (GetGameObject()->GetTransform()->GetPosition().y <= 0){
			m_CanMove = false;
		}
	}
}

void dae::MoveMenuComponent::Reset()
{
	m_CanMove = true;
	GetGameObject()->GetTransform()->Translate(0, 720);
	GetGameObject()->SetIsHidden(false);
}

void dae::MoveMenuComponent::Initialize()
{
	SetOriginPoint();
}

