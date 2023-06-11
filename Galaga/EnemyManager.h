#pragma once
#include "Component.h"
#include "Scene.h"

namespace dae {

	class EnemyManager final : public Component
	{
	public:
		EnemyManager(Scene* scene, float startDelay, bool isVersus = false) : m_Scene{ scene }, StartDelayTimer{ startDelay }, IsVersus{ isVersus } {};
		~EnemyManager() = default;
		EnemyManager(const EnemyManager&) = delete;
		EnemyManager(EnemyManager&&) noexcept = delete;
		EnemyManager& operator=(const EnemyManager&) = delete;
		EnemyManager& operator=(EnemyManager&&) noexcept = delete;

		void Update() override;
		void SetProceedToNextStage() { CanProceedToNextStage = true; }
		int GetNrOfCapturedFighters() { return NrOfCapturedFighers; }
		void AcknowledgeFighterCaptured() { NrOfCapturedFighers++; }
		void CheckStatus();

		void DisableEnemies(float disabledTime);

	private:
		Scene* m_Scene{};
		bool CanProceedToNextStage{ true };
		bool WasCleared{ false };

		float StartDelayTimer{ 0 };
		float DefaultMoveTimer{ 1 }, MoveTimer{ DefaultMoveTimer };
		bool IsStart{ true };
		int MoveModifier{ 1 }, NrOfCapturedFighers{ 0 };
		bool IsVersus{ false };

		//void MoveEnemy(GameObject* enemy, float movement);
	};
}
