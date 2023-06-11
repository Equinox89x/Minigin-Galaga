#include "OpposerComponent.h"
#include "TextureComponent.h"
#include "PlayerComponent.h"
#include "Callback.h"
#include "GalagaMath.h"
#include <random>
#include "Timer.h"
#include "EnemyManager.h"
#include "ShootComponent.h"
#include "CapturedComponent.h"

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
				ResetBeaming();
			}
		}

		if (IsDiving) {
			float movement{ deltaTime * MaxMoveSpeed };
			auto pos{ GetGameObject()->GetTransform()->GetPosition() };
			HandleBeaming(pos);
			if (!IsBeaming) { 
				GetGameObject()->GetTransform()->AddTranslate(0, CanReturn ? -movement : movement);
			}
			else {
				HandleGrabbing();
			}

			pos = GetGameObject()->GetTransform()->GetPosition();
			HandleCaptureAndReturn(pos);
		}
		else {
			HandleRandomMovement(deltaTime);
		}
	}
}

void OpposerComponent::HandleCaptureAndReturn(glm::vec3& pos)
{
	if (pos.y <= EndPosition.y && CanReturn) {
		IsDiving = false;
		CanReturn = false;
		GetGameObject()->GetTransform()->Translate(EndPosition.x, EndPosition.y);

		if (ShipCaptured && CurrentCapturedFighter != "") {
			if (auto fighter{ m_Scene->GetGameObject(CurrentCapturedFighter) }) {
				fighter->GetComponent<CapturedComponent>()->SetIsTurned(true);
				fighter->GetComponent<TextureComponent>()->Rotate(180);
				auto pos2{ fighter->GetTransform()->GetPosition() };
				fighter->GetTransform()->Translate(pos2.x, 0);
				ShipCaptured = false;
			}
		}
	}
}

void OpposerComponent::HandleBeaming(glm::vec3& pos)
{
	if (pos.y >= WindowSizeY / 1.8f && !CanReturn) {
		GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Weapon])->SetIsVisible(true);
		GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetPaused(true);
		GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetFrame(2);
		IsBeaming = true;
	}
}

void OpposerComponent::ResetBeaming()
{
	IsBeaming = false;
	CanReturn = true;
	GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Weapon])->SetIsVisible(false);
	GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetPaused(false);
	GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetFrame(1);
	GetGameObject()->EnableCollision(true);
}

void OpposerComponent::HandleGrabbing()
{
	if (CanGrab) {
		if (auto player{ m_Scene->GetGameObject(EnumStrings[Player0]) }) {
			if (player->GetComponent<PlayerComponent>()->CanBeGrabbed) {
				auto rect1{ GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Weapon])->GetRect() };
				auto rect2{ player->GetComponent<TextureComponent>()->GetRect() };
				if (GalagaMath::IsOverlapping(rect1, rect2)) {
					CaptureFighter(player.get(), GetGameObject());
					player->GetComponent<PlayerComponent>()->Grab();
					BeamingTimer = 1;
					CanGrab = false;
				}
			}
		}
	}
}

void OpposerComponent::CaptureFighter(GameObject* player, GameObject* enemy)
{
	auto nr{ m_Scene->GetGameObject(EnumStrings[EnemyHolder])->GetComponent<EnemyManager>()->GetNrOfCapturedFighters() };
	std::string name{ EnumStrings[CapturedFighter] + std::to_string(nr) };
	CurrentCapturedFighter = name;

	auto go = std::make_shared<GameObject>();
	go->SetName(name);
	go->AddComponent(new ShootComponent(m_Scene, nr + 100, false, false, true));

	//Texture
	go->AddComponent(new TextureComponent());
	go->GetComponent<TextureComponent>()->SetName(name);
	go->GetComponent<TextureComponent>()->SetTexture("galagaRed.png");
	go->GetComponent<TextureComponent>()->Scale(0.7f, 0.7f);

	go->AddComponent(new CapturedComponent(m_Scene, player, enemy));
	m_Scene->Add(go);
	ShipCaptured = true;
	m_Scene->GetGameObject(EnumStrings[EnemyHolder])->GetComponent<EnemyManager>()->AcknowledgeFighterCaptured();
}

void OpposerComponent::HandleRandomMovement(float deltaTime)
{
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
	else if (futurePos.x >(GameWindowSizeX)-60) {
		IsGoingRight = false;
	}

	GetGameObject()->GetTransform()->AddTranslate(movement, 0);
}

void OpposerComponent::DestroyOpposer()
{
	CanDie = true;
	GetGameObject()->EnableCollision(false);
	GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetTexture("explosion.png", 0.1f, 4);
	GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetOffset({ -40, -25 });
}

void OpposerComponent::ExecuteBeam()
{
	IsDiving = true;
	BeamingTimer = DefaultBeamingTimer;
}
