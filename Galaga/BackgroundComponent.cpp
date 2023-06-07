#include "BackgroundComponent.h"
#include "TransformComponent.h"
#include <Timer.h>

void BackgroundComponent::Initialize()
{
	GetGameObject()->GetTransform()->Translate(m_InitialPosition);
}

void BackgroundComponent::Update()
{
	auto deltaTime{ Timer::GetInstance().GetDeltaTime() };
	auto go{ GetGameObject() };
	go->GetTransform()->AddTranslate(0, (m_MoveSpeed * deltaTime));
	if (go->GetTransform()->GetPosition().y >= 720*2) {
		go->GetTransform()->Translate(m_StartPosition);
	}

}
