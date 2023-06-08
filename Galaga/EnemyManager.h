#pragma once
#include "Component.h"
#include "Scene.h"
using namespace dae;


class EnemyManager final : public Component
{
public:
	EnemyManager(Scene* scene, float startDelay) : m_Scene{ scene }, StartDelayTimer{ startDelay } {};
	~EnemyManager() = default;
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager(EnemyManager&&) noexcept = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;
	EnemyManager& operator=(EnemyManager&&) noexcept = delete;

	void Update() override;
	void SetProceedToNextStage() {
		CanProceedToNextStage = true;
	}

	void CheckStatus();

private:
	Scene* m_Scene{};
	bool CanProceedToNextStage{ true };
	bool WasCleared{ false };

	float StartDelayTimer{ 0 };
	float DefaultMoveTimer{ 1 }, MoveTimer{ DefaultMoveTimer };
	bool IsStart{ true };
	int MoveModifier{ 1 };
};

