#include "EndScreenComponent.h"
#include "Timer.h"
#include "Callback.h"

void EndScreenComponent::Update()
{
	float dt{ Timer::GetInstance().GetDeltaTime() };
	EndTimer -= dt;
	if (EndTimer <= 0) {
		m_pCallback->Notify(GetGameObject(), Event::ToMenu);
	}
}
