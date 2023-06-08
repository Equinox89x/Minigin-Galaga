#include "EnemyManager.h"
#include "Callback.h"
#include "Timer.h"
#include "TransformComponent.h"
#include "EnemyComponent.h"

void EnemyManager::Update()
{
	auto children{ m_Scene->GetGameObject("EnemyHolder")->GetChildren("Enemy") };
	WasCleared = children.size() <= 1;

	auto dt{ Timer::GetInstance().GetDeltaTime() };
	MoveTimer -= dt;
	if (MoveTimer <= 0) {
		MoveTimer = DefaultMoveTimer;
		MoveModifier = -MoveModifier;
	}

	auto movement{ (dt * 30) * MoveModifier };

	if (IsStart) {
		if (StartDelayTimer > 0) {
			StartDelayTimer -= dt;
		}
		else {
			IsStart = false;
		}

		for (auto enemy : children) {
			enemy->GetTransform()->AddTranslate(movement, 0);
		}
		//GetGameObject()->GetTransform()->AddTranslate(movement, 0);
	}
	else {
		for (auto enemy : children) {
			if (enemy->GetTransform()->GetPosition().x <= WindowSizeX / 3) {
				enemy->GetComponent<EnemyComponent>()->TranslateInitialPosition({ -movement, 0 });
				enemy->GetTransform()->AddTranslate(-movement, 0);
			}
			else {
				enemy->GetComponent<EnemyComponent>()->TranslateInitialPosition({ movement, 0 });
				enemy->GetTransform()->AddTranslate(movement, 0);
			}
		}
	}
}

void EnemyManager::CheckStatus()
{
	if (WasCleared && CanProceedToNextStage) {
		m_pCallback->Notify(GetGameObject(), Event::StageCleared);
		CanProceedToNextStage = false;
	}
}