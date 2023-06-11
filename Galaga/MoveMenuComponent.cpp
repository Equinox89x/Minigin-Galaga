#include "Timer.h"
#include "MoveMenuComponent.h"

void dae::MoveMenuComponent::Update()
{
	if (m_CanMove) {
		auto dt{ Timer::GetInstance().GetDeltaTime() };
		auto move{ dt * m_MoveSpeed };
		GetGameObject()->GetTransform()->AddTranslate(0, -move);
		m_Scene->GetGameObject(EnumStrings[Logo])->GetTransform()->AddTranslate(0, -move);
		if (GetGameObject()->GetTransform()->GetPosition().y <= 0){
			m_CanMove = false;
		}
	}
}

void dae::MoveMenuComponent::Reset()
{
	m_CanMove = true;
	GetGameObject()->GetTransform()->Translate(0, WindowSizeY);
	GetGameObject()->SetIsHidden(false);
}

void dae::MoveMenuComponent::Initialize()
{
	SetOriginPoint();
}

