#pragma once
#include "Component.h"
#include <glm/ext/vector_float2.hpp>
#include <GalagaMath.h>
#include <Scene.h>
using namespace dae;

namespace dae {
	enum class EnemyType {
		GOEI,
		ZAKO,
		BOSS
	};

}

class EnemyComponent final : public Component
{

public:
	EnemyComponent(Scene* scene, EnemyType enemyType, int index, float delayTimer, int pathNr, glm::vec2 endPosition, int score, int divingScore = 0) :
		m_Scene{ scene },
		EnemyType{ enemyType },
		Score{ score },
		EndPosition{ endPosition },
		PathNr{ pathNr }
	{
		DivingScore = divingScore != 0 ? divingScore : Score * 2;
		Index = index != -1 ? index : 0;
		TimeBeforeStart += (static_cast<float>(Index) / 10) + delayTimer;
	};

	~EnemyComponent() = default;
	EnemyComponent(const EnemyComponent&) = delete;
	EnemyComponent(EnemyComponent&&) noexcept = delete;
	EnemyComponent& operator=(const EnemyComponent&) = delete;
	EnemyComponent& operator=(EnemyComponent&&) noexcept = delete;

	virtual void Initialize() override;
	virtual void Update() override;
	//virtual void Render() const override;
	//virtual void FixedUpdate() override; 

	void DestroyEnemy();
	int GetScore() { return IsDiving ? DivingScore : Score; };
	void SetEndPosition(glm::vec2 endPos) { EndPosition = endPos; };
	EnemyType GetEnemyType() { return EnemyType; };

	void TranslateInitialPosition(glm::vec2 addedPos);

private:
	Scene* m_Scene;

	EnemyType EnemyType{};
	bool CanDie{ false };
	float DeathTimer{ .4f }, DefaultDiveTimer{ 5 }, DiveTimer{ DefaultDiveTimer };
	bool IsDiving{ false }, CanReturn{ false };
	int Score, DivingScore;
	glm::vec3 InitialPosition;
	float DesiredChance{ 0.2f };

	bool IsBeaming{ false };
	float BeamTinmer{ 5.f };
	
	glm::vec2 EndPosition;
	bool IsStart{ true };
	float StartTime{ 0 }, TimeBeforeStart{ 0 };
	int Index{ 0 };

	std::vector<std::vector<glm::vec2>> Paths;
	int PathNr{ 0 };
};

