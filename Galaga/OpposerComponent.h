#pragma once
#include "Component.h"
#include <Scene.h>
#include <glm/ext/vector_float2.hpp>

using namespace dae;

class OpposerComponent final : public Component
{
public:
	OpposerComponent(Scene* scene, int lives) : m_Scene{ scene }, Lives{ lives } {};
	~OpposerComponent() = default;
	OpposerComponent(const OpposerComponent&) = delete;
	OpposerComponent(OpposerComponent&&) noexcept = delete;
	OpposerComponent& operator=(const OpposerComponent&) = delete;
	OpposerComponent& operator=(OpposerComponent&&) noexcept = delete;

	void Initialize() override;
	void Update() override;


	void DestroyOpposer();
	int GetLives() { return Lives; };
	void Damage() { Lives--; };
	void ExecuteBeam();

private:
	int Lives{ 2 };
	bool CanDie{ false };
	float DefaultDeathTimer{ .4f }, DeathTimer{ DefaultDeathTimer };
	Scene* m_Scene;

	bool IsBeaming{ false }, IsDiving{ false }, CanReturn{ false };
	float DefaultBeamingTimer{ 4 }, BeamingTimer{ DefaultBeamingTimer };

	float DefaultMoveTimer{ 3 }, MoveTimer{ 1.5 };
	int MoveModifier{ 1 };

	float MaxMoveSpeedChangeTimer{ 1 }, MinMoveSpeedChangeTimer{ 0.2f }, MoveSpeedChangeTimer{ MaxMoveSpeedChangeTimer };
	float MaxMoveSpeed{ 500 }, MinMoveSpeed{ 100 }, MoveSpeed{ MaxMoveSpeed };
	bool IsGoingRight{ true };

	glm::vec2 EndPosition;

	bool ShipCaptured{ false }, CanGrab{ true };
	std::string CurrentCapturedFighter{ "" };

	void CaptureFighter(GameObject* player, GameObject* enemy);
	void HandleBeaming(glm::vec3& pos);
	void ResetBeaming();
	void HandleGrabbing();
	void HandleRandomMovement(float deltaTime);
	void HandleCaptureAndReturn(glm::vec3& pos);
};


