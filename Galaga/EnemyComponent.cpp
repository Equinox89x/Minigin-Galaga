#include "EnemyComponent.h"
#include "TransformComponent.h"
#include <Timer.h>
#include <random>
#include <Minigin.h>
#include <glm/gtx/rotate_vector.hpp>
#include "ShootComponent.h"
#include "Renderer.h"
#include "CapturedComponent.h"
#include "PlayerComponent.h"
#include "EnemyManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include "AudioComponent.h"

void EnemyComponent::Initialize()
{
	InitialPosition = GetGameObject()->GetTransform()->GetPosition();

	auto p2conv{ (WindowSizeY - 525.24638f) };
	auto p3conv{ (WindowSizeY - 477.21614f) };
	auto p4conv{ (WindowSizeY - 342.75319) };
	auto p5conv{ (WindowSizeY - 285.9912) };
	glm::vec2 P0{ 400, 0 };
	glm::vec2 P1{ 400, 100 };
	glm::vec2 P2{ 361.38993, p2conv };
	glm::vec2 P3{ 220.83272, p3conv };
	glm::vec2 P4{ 199.75248, p4conv };
	glm::vec2 P5{ 400.74034, p5conv };
	glm::vec2 P6{ EndPosition };
	std::vector<glm::vec2> vec1{ P0, P1, P2, P3, P4, P5, P6 };
	
	glm::vec2 P01{ 720-P0.x, 0 };
	glm::vec2 P11{ 720-P1.x, 100 };
	glm::vec2 P21{ 720-P2.x, p2conv };
	glm::vec2 P31{ 720-P3.x, p3conv };
	glm::vec2 P41{ 720-P4.x, p4conv };
	glm::vec2 P51{ 720-P5.x, p5conv };
	glm::vec2 P61{ EndPosition };
	std::vector<glm::vec2> vec2{ P01, P11, P21, P31, P41, P51, P61 };	
	
	auto p02conv{ (WindowSizeY - 300) };
	auto p12conv{ (WindowSizeY - 569) };
	auto p22conv{ (WindowSizeY + 115) };
	glm::vec2 P02{ 0 , p02conv };
	glm::vec2 P12{ 862, p12conv };
	glm::vec2 P22{ 143 , p22conv };
	glm::vec2 P32{ EndPosition };
	std::vector<glm::vec2> vec3{ P02, P12, P22, P32 };	
	
	glm::vec2 P03{ 720 - P02.x, p02conv };
	glm::vec2 P13{ 720 - P12.x, p12conv };
	glm::vec2 P23{ 720 - P22.x, p22conv };
	glm::vec2 P33{ EndPosition };
	std::vector<glm::vec2> vec4{ P03, P13, P23, P33 };

	Paths.push_back(vec1);
	Paths.push_back(vec2);	
	Paths.push_back(vec3);
	Paths.push_back(vec4);

	glm::vec2 P04{ EndPosition };
	glm::vec2 P14{ P04.x - 300, P04.y + 200 };
	glm::vec2 P24{ P04.x + 300, P04.y + 150 };
	glm::vec2 P34{ 0, 0 };
	glm::vec2 P44{ P34.x-150, P34.y+108 };
	glm::vec2 P54{ P44.x+250, P44.y+102 };
	glm::vec2 P64{ P54.x + 150, P54.y - 455 };
	glm::vec2 P74{ EndPosition };
	std::vector<glm::vec2> vec5{ P04, P14, P24, P34, P44, P54, P64, P74 };
	ZakoBombingPath = vec5;

	glm::vec2 P05{ 0, -20 };
	glm::vec2 P15{ -200, 120 };
	glm::vec2 P25{ 200, 220 };
	glm::vec2 P35{ -200, 320 };
	glm::vec2 P45{ 200, 420 };
	glm::vec2 P55{ -200, 520 };
	glm::vec2 P65{ 200, 620 };
	glm::vec2 P75{ -200, 720 };
	std::vector<glm::vec2> vec6{ P05, P15, P25, P35, P45, P55, P65, P75 };
	OiginalBombingPath = vec6;
	BombingPath = OiginalBombingPath;

	GetGameObject()->EnableCollision(true);
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

	Paths[PathNr][Paths[PathNr].size() - 1] = EndPosition;

	//if -> game movement, else -> Spawn movement
	if (!IsStart) {
		if (IsBeaming) {
			BeamTimer -= deltaTime;
			if (BeamTimer <= 0) {
				ResetBeaming();
			}
		}

		if (IsDiving || IsTractorDiving) {
			float movement{ deltaTime * 450 };
			glm::vec3 pos{ GetGameObject()->GetTransform()->GetPosition() };
			if (!IsBeaming) {
				StartTime += deltaTime / 2;

				if (EnemyType == EnemyType::ZAKO) {
					HandleZakoDive();
				}

				if (EnemyType == EnemyType::GOEI) {
					HandleGeneralDive(pos, movement);
				}

				if (EnemyType == EnemyType::BOSS) {
					HandleBossDive(pos, movement);
				}
			}
			else {
				HandleGrabbing();
			}

			pos = GetGameObject()->GetTransform()->GetPosition();
			HandleCaptureAndReturn(pos);
		}
		else {
			//prevent enemies from attacking when player died
			CannotAttackTime -= deltaTime;
			if (CannotAttackTime > 0) return;

			HandleChanceCalculation(deltaTime);
		}
	}
	else {
		TimeBeforeStart -= deltaTime;
		if (TimeBeforeStart <= 0) {
			StartTime += deltaTime * 1.5f;

			//move along bezier
			if (StartTime <= 1.f) {
				glm::vec2 point{ GalagaMath::CalculateBezierPoint(StartTime, Paths[PathNr]) };
				GetGameObject()->GetTransform()->Translate(point);
			}
			else {
				auto comp{ GetGameObject()->GetComponent<TextureComponent>() };
				GetGameObject()->EnableCollision(true);
				comp->SetPosition(EndPosition.x, EndPosition.y);
				IsStart = false;
			}
		}
	}
}

void EnemyComponent::HandleZakoDive()
{
	//move along bezier
	if (StartTime <= 1.f) {
		ZakoBombingPath[7] = EndPosition;
		glm::vec2 point{ GalagaMath::CalculateBezierPoint(StartTime, ZakoBombingPath) };
		GetGameObject()->GetTransform()->Translate(point);
	}
}

void EnemyComponent::HandleGeneralDive(glm::vec3& pos, float movement)
{
	if (!ShouldMoveUp) {
		if (pos.y >= WindowSizeY) {
			GetGameObject()->GetTransform()->Translate(EndPosition.x, -Cellsize);
			ShouldMoveUp = true;
		}
		else {
			if (StartTime <= 1.f) {
				glm::vec2 point{ GalagaMath::CalculateBezierPoint(StartTime, BombingPath) };
				GetGameObject()->GetTransform()->Translate(point);
			}
		}
	}
	else {
		GetGameObject()->GetTransform()->AddTranslate(ManagerMovement.x, movement);
		auto rect1 = GetGameObject()->GetComponent<TextureComponent>()->GetRect();
		SDL_Rect rect2{ static_cast<int>(EndPosition.x), static_cast<int>(EndPosition.y + Cellsize), static_cast<int>(Cellsize), static_cast<int>(Cellsize )};
		if (GalagaMath::IsOverlapping(rect1, rect2)) {
			GetGameObject()->GetTransform()->Translate(EndPosition);
			IsDiving = false;
			ShouldMoveUp = false;
			BombingPath = OiginalBombingPath;

			m_Scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->StopSound();
		}
	}
}

void EnemyComponent::HandleBossDive(glm::vec3& pos, float movement)
{
	if (IsTractorDiving) {
		if (pos.y >= WindowSizeY / 1.7f && !CanReturn) {
			GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Weapon])->SetIsVisible(true);
			GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetPaused(true);
			GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetFrame(2);
			IsBeaming = true;

			m_Scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayTractorBeamSound(false);

		}
		else {
			GetGameObject()->GetTransform()->AddTranslate(ManagerMovement.x, CanReturn ? -movement : movement);
			if (pos.y < EndPosition.y) {
				GetGameObject()->GetTransform()->Translate(EndPosition.x, EndPosition.y);
				IsTractorDiving = false;
			}
		}
	}
	else {
		HandleGeneralDive(pos, movement);
	}
}

void EnemyComponent::HandleChanceCalculation(float deltaTime)
{
	//check chance to start a dive/attack/shot
	DiveTimer -= deltaTime;
	if (DiveTimer <= 0) {
		double chance = GalagaMath::CalculateChance();

		if (chance <= DesiredChance) {

			switch (EnemyType)
			{
			case EnemyType::GOEI:
				CalculateGeneralDive();
				break;

			case EnemyType::ZAKO:
				if (GalagaMath::CalculateChance() <= 0.5f) {
					GetGameObject()->GetComponent<ShootComponent>()->Shoot();

					m_Scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayShootSound(false);

				}
				else CalculateZakoDive();
				break;

			case EnemyType::BOSS:
				if (GalagaMath::CalculateChance() <= 0.5f) CalculateGeneralDive();
				else {
					m_Scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayDiveSound(false);
					IsTractorDiving = true;
				}
				break;

			default:
				break;
			}
		}
		DiveTimer = DefaultDiveTimer;
	}
}

void EnemyComponent::CalculateZakoDive()
{
	auto pos{ m_Scene->GetGameObject(EnumStrings[Player0])->GetTransform()->GetPosition() };
	int Modifier{ 1 };
	if (pos.x < GetGameObject()->GetTransform()->GetPosition().x) Modifier = -1;

	ZakoBombingPath[0] = EndPosition;
	ZakoBombingPath[1] = { ZakoBombingPath[0].x - (300 * Modifier), ZakoBombingPath[0].y + 200 };
	ZakoBombingPath[2] = { ZakoBombingPath[0].x + (300 * Modifier), ZakoBombingPath[0].y + 150 };
	ZakoBombingPath[3] = pos;
	ZakoBombingPath[4] = { ZakoBombingPath[3].x + (50 * Modifier), ZakoBombingPath[3].y + 208 };
	ZakoBombingPath[5] = { ZakoBombingPath[4].x + (250 * Modifier), ZakoBombingPath[4].y + 102 };
	ZakoBombingPath[6] = { ZakoBombingPath[5].x + (150 * Modifier), ZakoBombingPath[5].y - 455 };
	ZakoBombingPath[7] = EndPosition;
	StartTime = 0;

	IsDiving = true;

	m_Scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayDiveSound(false);

}

void EnemyComponent::CalculateGeneralDive()
{
	auto pos1{ GetGameObject()->GetTransform()->GetWorldPosition() };
	for (auto& point : BombingPath)
	{
		point += glm::vec2{ pos1.x, pos1.y };
	}
	auto pos{ m_Scene->GetGameObject(EnumStrings[Player0])->GetTransform()->GetPosition() };
	BombingPath[BombingPath.size() - 2] = pos;
	StartTime = 0;
	IsDiving = true;

	m_Scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayDiveSound(false);
}

void EnemyComponent::HandleCaptureAndReturn(glm::vec3& pos)
{
	if (pos.y <= EndPosition.y && CanReturn) {
		CanReturn = false;

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

void EnemyComponent::HandleGrabbing()
{
	if (CanGrab && EnemyType == EnemyType::BOSS) {
		auto players{ m_Scene->GetGameObjects(EnumStrings[PlayerGeneral], false) };
		for (auto player : players) {
			if (player->GetComponent<PlayerComponent>()->CanBeGrabbed) {
				auto rect1{ GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Weapon])->GetRect() };
				auto rect2{ player->GetComponent<TextureComponent>()->GetRect() };
				if (GalagaMath::IsOverlapping(rect1, rect2)) {
					CaptureFighter(player.get(), GetGameObject());
					player->GetComponent<PlayerComponent>()->Grab();
					BeamTimer = 1;
					CanGrab = false;
				}
			}
		}
	}
}

void EnemyComponent::ResetBeaming()
{
	IsBeaming = false;
	CanReturn = true;
	GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Weapon])->SetIsVisible(false);
	GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetPaused(false);
	GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetFrame(1);
	BeamTimer = DefaultBeamTimer;
	CanGrab = true;
}

void EnemyComponent::CaptureFighter(GameObject* player, GameObject* enemy)
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

	m_Scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayCapturedTractorBeamSound(false);

}

void EnemyComponent::Render() const
{
	//SDL_Rect rect1{ static_cast<int>(EndPosition.x + 10), static_cast<int>(EndPosition.y + 10), 10,10 };
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 255, 255); // Set the color to blue
	//SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rect1); // D
}

void EnemyComponent::DestroyEnemy()
{
	CanDie = true;
	GetGameObject()->EnableCollision(false);
	GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetTexture("explosion.png", 0.1f, 4);
	GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetOffset({ -40, -25 });

	m_Scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayDeathSound(false);
}

void EnemyComponent::TranslateInitialPosition(glm::vec2 addedPos)
{
	EndPosition += addedPos;
}

void EnemyComponent::Disable(float disabledTime)
{
	CannotAttackTime = disabledTime;
	if (!CanReturn) CanReturn = true;
	if (IsBeaming) ResetBeaming();
}
