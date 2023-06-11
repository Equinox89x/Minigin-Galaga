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
			MoveSpeed = GalagaMath::CalculateChance(MinMoveSpeed, MaxMoveSpeed);
			MoveSpeedChangeTimer = GalagaMath::CalculateChance(MinMoveSpeedChangeTimer, MaxMoveSpeedChangeTimer);
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
			double chance = GalagaMath::CalculateChance();
			if (chance <= 0.5f) {
				GetGameObject()->GetComponent<ShootComponent>()->Shoot();

				m_pAudioService->SetEffectVolume(60);
				int soundId;
				soundId = m_pAudioService->LoadSound("../Data/Sound/ShootSound.wav");
				m_pAudioService->PlaySound(soundId);
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

	m_pAudioService->SetEffectVolume(60);
	int soundId;
	soundId = m_pAudioService->LoadSound("../Data/Sound/PlayerDeathSound.wav");
	m_pAudioService->PlaySound(soundId);
}