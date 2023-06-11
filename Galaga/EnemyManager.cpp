#include "EnemyManager.h"
#include "Callback.h"
#include "Timer.h"
#include "TransformComponent.h"
#include "EnemyComponent.h"

void dae::EnemyManager::Update()
{
	if (IsVersus) return;

	auto children{ m_Scene->GetGameObject(EnumStrings[EnemyHolder])->GetChildren(EnumStrings[Enemy]) };
	auto children2{ m_Scene->GetGameObjects(EnumStrings[CapturedFighter], false) };
	WasCleared = children.size() <= 0 && children2.size() <= 0;
	if (WasCleared) {
		CheckStatus();
	}

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
			enemy->GetComponent<EnemyComponent>()->SetManagerMovement({ movement, 0 });
			enemy->GetComponent<EnemyComponent>()->TranslateInitialPosition({ movement, 0 });
		}
	}
	else {
		for (auto enemy : children) {
			if (enemy->GetTransform()->GetPosition().x <= (GameWindowSizeX) / 2.4f) {
				enemy->GetComponent<EnemyComponent>()->SetManagerMovement({ -movement, 0 });
				enemy->GetComponent<EnemyComponent>()->TranslateInitialPosition({ -movement, 0 });
				enemy->GetTransform()->AddTranslate(-movement, 0);
			}
			else {
				enemy->GetComponent<EnemyComponent>()->SetManagerMovement({ movement, 0 });
				enemy->GetComponent<EnemyComponent>()->TranslateInitialPosition({ movement, 0 });
				enemy->GetTransform()->AddTranslate(movement, 0);
			}
		}
	}
}

void dae::EnemyManager::CheckStatus()
{
	if (WasCleared && CanProceedToNextStage) {
		m_pCallback->Notify(GetGameObject(), Event::StageCleared);
		CanProceedToNextStage = false;
	}
}

void dae::EnemyManager::DisableEnemies(float disabledTime)
{
	for (auto enemy : m_Scene->GetGameObject(EnumStrings[EnemyHolder])->GetChildren(EnumStrings[Enemy])) {
		enemy->GetComponent<EnemyComponent>()->Disable(disabledTime);
	}
}
