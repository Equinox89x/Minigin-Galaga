#pragma once
#include "Component.h"
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
	EnemyComponent(EnemyType enemyType, float score, float divingScore = 0) :
		EnemyType{ enemyType }, 
		Score{ score } 
	{
		DivingScore = divingScore != 0 ? divingScore : Score * 2;
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
	float GetScore() { return IsDiving ? DivingScore : Score; };

private:
	EnemyType EnemyType{};
	bool CanDie{ false };
	float DeathTimer{ .4f }, DefaultDiveTimer{ 5 }, DiveTimer{ DefaultDiveTimer };
	bool IsDiving{ false }, CanReturn{ false };
	float Score, DivingScore;
	glm::vec3 InitialPosition;
	float DesiredChance{ 0.2f };

	bool IsBeaming{ false };
	float BeamTinmer{ 5.f };

};

