#include "OpposerComponent.h"
#include "TextureComponent.h"
#include "PlayerComponent.h"
#include "Callback.h"
#include "GalagaMath.h"
#include <random>
#include "Timer.h"

void OpposerComponent::Initialize()
{
	EndPosition = GetGameObject()->GetTransform()->GetPosition();
}

void OpposerComponent::Update()
{
	float deltaTime{ Timer::GetInstance().GetDeltaTime() };
	if (CanDie) {
		DeathTimer -= deltaTime;
		if (DeathTimer <= 0) {
			CanDie = false;
			m_pCallback->Notify(GetGameObject(), Event::GameOver);
			return;
		}
	}
	else {
		if (IsBeaming) {
			BeamingTimer -= deltaTime;
			if (BeamingTimer <= 0) {
				IsBeaming = false;
				CanReturn = true;
				GetGameObject()->GetComponent<TextureComponent>("Weapon")->SetIsVisible(false);
				GetGameObject()->GetComponent<TextureComponent>("Opposer")->SetPaused(false);
				GetGameObject()->GetComponent<TextureComponent>("Opposer")->SetFrame(1);
			}
		}

		if (IsDiving) {
			float movement{ deltaTime * MaxMoveSpeed };
			auto pos{ GetGameObject()->GetTransform()->GetPosition() };
			if (pos.y >= WindowSizeY / 2 && !CanReturn) {
				GetGameObject()->GetComponent<TextureComponent>("Weapon")->SetIsVisible(true);
				GetGameObject()->GetComponent<TextureComponent>("Opposer")->SetPaused(true);
				GetGameObject()->GetComponent<TextureComponent>("Opposer")->SetFrame(2);
				IsBeaming = true;
			}
			if (!IsBeaming) { 
				GetGameObject()->GetTransform()->AddTranslate(0, CanReturn ? -movement : movement);
			}
			else {
				auto player{ m_Scene->GetGameObject("Player0") };
				if (player) {
					if (player->IsMarkedForDestroy()) return;
					auto rect1{ GetGameObject()->GetComponent<TextureComponent>("Weapon")->GetRect() };
					auto rect2{ player->GetComponent<TextureComponent>()->GetRect() };
					if (GalagaMath::IsOverlapping(rect1, rect2)) {
						player->GetComponent<PlayerComponent>()->Die();
					}
				}
			}

			pos = GetGameObject()->GetTransform()->GetPosition();
			if (pos.y <= EndPosition.y && CanReturn) {
				IsDiving = false;
				CanReturn = false;
				GetGameObject()->GetTransform()->Translate(EndPosition.x, EndPosition.y);
			}
		}
		else {
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
			else if (futurePos.x > GameWindowSizeX-60) {
				IsGoingRight = false;
			}

			GetGameObject()->GetTransform()->AddTranslate(movement, 0);
		}
	}
}

void OpposerComponent::DestroyOpposer()
{
	CanDie = true;
	GetGameObject()->EnableCollision(false);
	GetGameObject()->GetComponent<TextureComponent>("Opposer")->SetTexture("explosion.png", 0.1f, 4);
	GetGameObject()->GetComponent<TextureComponent>("Opposer")->SetOffset({ -40, -25 });
}

void OpposerComponent::ExecuteBeam()
{
	IsDiving = true;
	BeamingTimer = DefaultBeamingTimer;
}
