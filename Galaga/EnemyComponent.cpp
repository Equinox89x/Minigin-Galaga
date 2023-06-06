#include "EnemyComponent.h"
#include "TransformComponent.h"
#include <Timer.h>
#include <random>
#include <Minigin.h>
#include <TextureComponent.h>

void EnemyComponent::Initialize()
{
	InitialPosition = GetGameObject()->GetTransform()->GetPosition();
}

void EnemyComponent::Update()
{
	float deltaTime{ Timer::GetInstance().GetDeltaTime() };
	if (CanDie) {
		DeathTimer -= deltaTime;
		if (DeathTimer <= 0) {
			GetGameObject()->MarkForDestroy();
		}
		return;
	}
	
	if (IsBeaming) {
		BeamTinmer -= deltaTime;
		if (BeamTinmer <= 0) {
			IsBeaming = false;
			CanReturn = true;
			GetGameObject()->GetComponent<TextureComponent>("Weapon")->SetIsVisible(false);
			GetGameObject()->GetComponent<TextureComponent>("enemy")->SetPaused(false);
			GetGameObject()->GetComponent<TextureComponent>("enemy")->SetFrame(1);
		}
	}

	if (IsDiving) {
		float movement{ deltaTime * 300 };
		if (EnemyType != EnemyType::BOSS) {
			if (GetGameObject()->GetTransform()->GetPosition().y >= WindowSizeY - 60) {
				CanReturn = true;
			}
		}
		else {
			if (GetGameObject()->GetTransform()->GetPosition().y >= WindowSizeY/2 && !CanReturn) {
				GetGameObject()->GetComponent<TextureComponent>("Weapon")->SetIsVisible(true);
				GetGameObject()->GetComponent<TextureComponent>("enemy")->SetPaused(true);
				GetGameObject()->GetComponent<TextureComponent>("enemy")->SetFrame(2);
				IsBeaming = true;
			}
		}
		if(!IsBeaming) GetGameObject()->GetTransform()->AddTranslate(0, CanReturn ? -movement : movement);

		if (GetGameObject()->GetTransform()->GetPosition().y <= InitialPosition.y && CanReturn) {
			IsDiving = false;
			CanReturn = false;
			GetGameObject()->GetTransform()->Translate(InitialPosition.x, InitialPosition.y);
		}
	}
	else {
		DiveTimer -= deltaTime;
		if (DiveTimer <= 0) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<double> dis(0.0, 1.0);

			double chance = dis(gen);

			if (chance <= DesiredChance) {
				IsDiving = true;
			}
			DiveTimer = DefaultDiveTimer;
		}
	}
}

void EnemyComponent::DestroyEnemy()
{
	GetGameObject()->EnableCollision(false);
	CanDie = true;
}
