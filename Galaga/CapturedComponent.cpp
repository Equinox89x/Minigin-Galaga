#include "CapturedComponent.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "TextureComponent.h"
#include "GalagaMath.h"
#include "ShootComponent.h"
#include <random>

void CapturedComponent::Initialize()
{
	GetGameObject()->GetTransform()->Translate(Player->GetTransform()->GetPosition());
}

void CapturedComponent::Update()
{
	float deltaTime{ Timer::GetInstance().GetDeltaTime() };
	if (HasDied) {
		DeathTimer -= deltaTime;
		if (DeathTimer <= 0) {
			GetGameObject()->MarkForDestroy();
		}
	}

	if (IsTurned) {
		MoveSpeedChangeTimer -= deltaTime;
		if (MoveSpeedChangeTimer <= 0) {
			std::random_device randomDevice;

			std::mt19937 generatedNr(randomDevice());
			std::uniform_real_distribution<float> distributeVal(MinMoveSpeed, MaxMoveSpeed);
			MoveSpeed = distributeVal(generatedNr);

			std::mt19937 generatedNr2(randomDevice());
			std::uniform_real_distribution<float> distributeVal2(MinMoveSpeedChangeTimer, MaxMoveSpeedChangeTimer);
			MoveSpeedChangeTimer = distributeVal2(generatedNr2);
		}


		auto futurePos{ GetGameObject()->GetTransform()->GetPosition() };
		auto movement{ (deltaTime * MoveSpeed) * MoveModifier };

		IsGoingRight ? MoveModifier = 1 : MoveModifier = -1;

		if (futurePos.x < 0) {
			IsGoingRight = true;
		}
		else if (futurePos.x > (GameWindowSizeX) - 60) {
			IsGoingRight = false;
		}

		GetGameObject()->GetTransform()->AddTranslate(movement, 0);


		AttackTimer -= deltaTime;
		if (AttackTimer <= 0) {
			std::random_device randomDevice;
			std::mt19937 generatedNr(randomDevice());
			std::uniform_real_distribution<double> distributeVal(0.0, 1.0);
			double chance = distributeVal(generatedNr);
			if (chance <= 0.5f) {
				GetGameObject()->GetComponent<ShootComponent>()->Shoot();
			}
			AttackTimer = DefaultAttackTimer;
		}

	}
	else {
		SDL_Rect rect1{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
		SDL_Rect rect2{ EnemyObj->GetComponent<TextureComponent>(EnumStrings[Enemy])->GetRect()};
		rect2.y += 10;

		if (GalagaMath::IsOverlapping(rect1, rect2)) {
			ReachedOverlap = true;
		}
		else {
			auto newInterpPos{ GalagaMath::MoveRectTowards(rect1, rect2, 0.9f) };
			GetGameObject()->GetTransform()->AddTranslate(newInterpPos.x, newInterpPos.y);
		}

		if(ReachedOverlap) GetGameObject()->GetTransform()->Translate(static_cast<float>(rect2.x), static_cast<float>(rect2.y + rect1.h));
	}
}

void CapturedComponent::Die()
{
	HasDied = true;
	auto player{ GetGameObject() };
	player->GetComponent<TextureComponent>()->Scale(2.5f, 2.5f);
	player->GetComponent<TextureComponent>()->SetTexture("playerExplosion.png", 0.1f, 4);
	auto rect = player->GetComponent<TextureComponent>()->GetRect();
	player->GetComponent<TextureComponent>()->SetOffset({ -rect.w / 2, rect.h / 4 });

	player->EnableCollision(false);

}