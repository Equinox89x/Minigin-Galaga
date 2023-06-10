#include "EnemyComponent.h"
#include "TransformComponent.h"
#include <Timer.h>
#include <random>
#include <Minigin.h>
#include <TextureComponent.h>
#include <glm/gtx/rotate_vector.hpp>
#include "ShootComponent.h"
#include "Renderer.h"
#include "CapturedComponent.h"
#include "PlayerComponent.h"
#include "EnemyManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

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
	glm::vec2 P6{ EndPosition.x, EndPosition.y };	
	std::vector<glm::vec2> vec1{ P0, P1, P2, P3, P4, P5, P6 };
	
	glm::vec2 P01{ 720-P0.x, 0 };
	glm::vec2 P11{ 720-P1.x, 100 };
	glm::vec2 P21{ 720-P2.x, p2conv };
	glm::vec2 P31{ 720-P3.x, p3conv };
	glm::vec2 P41{ 720-P4.x, p4conv };
	glm::vec2 P51{ 720-P5.x, p5conv };
	glm::vec2 P61{ EndPosition.x, EndPosition.y };
	std::vector<glm::vec2> vec2{ P01, P11, P21, P31, P41, P51, P61 };	
	
	auto p02conv{ (WindowSizeY - 300) };
	auto p12conv{ (WindowSizeY - 569) };
	auto p22conv{ (WindowSizeY + 115) };
	glm::vec2 P02{ 0 , p02conv };
	glm::vec2 P12{ 862, p12conv };
	glm::vec2 P22{ 143 , p22conv };
	glm::vec2 P32{ EndPosition.x, EndPosition.y };
	std::vector<glm::vec2> vec3{ P02, P12, P22, P32 };	
	
	glm::vec2 P03{ 720 - P02.x, p02conv };
	glm::vec2 P13{ 720 - P12.x, p12conv };
	glm::vec2 P23{ 720 - P22.x, p22conv };
	glm::vec2 P33{ EndPosition.x, EndPosition.y };
	std::vector<glm::vec2> vec4{ P03, P13, P23, P33 };

	Paths.push_back(vec1);
	Paths.push_back(vec2);	
	Paths.push_back(vec3);
	Paths.push_back(vec4);

	glm::vec2 P04{ EndPosition.x, EndPosition.y };
	glm::vec2 P14{ P04.x-300, P04.y+200 };
	glm::vec2 P24{ P04.x + 300, P04.y + 300 };
	glm::vec2 P34{ 0, 0 };
	glm::vec2 P44{ P34.x-150, P34.y+108 };
	glm::vec2 P54{ P44.x+250, P44.y+102 };
	glm::vec2 P64{ P54.x + 150, P54.y - 455 };
	glm::vec2 P74{ EndPosition.x, EndPosition.y };
	std::vector<glm::vec2> vec5{ P04, P14, P24, P34, P44, P54, P64, P74 };
	BombingPaths.push_back(vec5);

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

		if (IsDiving) {
			float movement{ deltaTime * 450 };
			glm::vec3 pos{ GetGameObject()->GetTransform()->GetPosition() };
			HandleDive(pos);
			if (!IsBeaming) {
				if (EnemyType == EnemyType::ZAKO) {
					StartTime += deltaTime /3;

					//move along bezier
					if (StartTime <= 1.f) {
						BombingPaths[BombingPathNr][7] = EndPosition;
						glm::vec2 point{ GalagaMath::CalculateBezierPoint(StartTime, BombingPaths[BombingPathNr]) };
						GetGameObject()->GetTransform()->Translate(point);
					}
				}
				else {

					GetGameObject()->GetTransform()->AddTranslate(ManagerMovement.x, CanReturn ? -movement : movement);
				}
				/*if (!CanReturn) {
				}
				else {
					GetGameObject()->GetTransform()->AddTranslate(ManagerMovement.x, CanReturn ? -movement : movement);
				}*/

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

				//glm::vec2 nextBezierPoint = GalagaMath::CalculateBezierPoint(StartTime + 0.01f, Paths[0]);
				//glm::vec2 forwardVector = glm::normalize(glm::vec2(nextBezierPoint.x - point.x, nextBezierPoint.y - point.y));
				//float rotationAngle = glm::degrees(std::atan2(forwardVector.y, forwardVector.x));

				//// Print the rotated point
				//GetGameObject()->GetComponent<TextureComponent>("enemy")->Rotate(rotationAngle);

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

void EnemyComponent::HandleChanceCalculation(float deltaTime)
{
	//check chance to start a dive/attack/shot
	DiveTimer -= deltaTime;
	if (DiveTimer <= 0) {

		std::random_device randomDevice;
		std::mt19937 generatedNr(randomDevice());
		std::uniform_real_distribution<double> distributeVal(0.0, 1.0);
		double chance = distributeVal(generatedNr);

		if (chance <= DesiredChance) {
			if (EnemyType == EnemyType::ZAKO || EnemyType == EnemyType::BOSS) {
				std::mt19937 generatedNr2(randomDevice());
				std::uniform_real_distribution<double> distributeVal2(0.0, 1.0);
				double chance2 = distributeVal2(generatedNr2);
				if (chance2 <= 0.5f) {
					if (EnemyType == EnemyType::ZAKO) GetGameObject()->GetComponent<ShootComponent>()->Shoot();
					if (EnemyType == EnemyType::BOSS) IsFullDiving = true;

				}
				else {
					auto pos{ m_Scene->GetGameObject(EnumStrings[Player0])->GetTransform()->GetPosition() };
					int Modifier{ 1 };
					if (pos.x < GetGameObject()->GetTransform()->GetPosition().x) Modifier = -1;

					BombingPaths[BombingPathNr][0] = EndPosition;
					BombingPaths[BombingPathNr][1] = { BombingPaths[BombingPathNr][0].x - (300* Modifier), BombingPaths[BombingPathNr][0].y + 200 };
					BombingPaths[BombingPathNr][2] = { BombingPaths[BombingPathNr][0].x + (300 * Modifier), BombingPaths[BombingPathNr][0].y + 150 };
					BombingPaths[BombingPathNr][3] = pos;
					BombingPaths[BombingPathNr][4] = { BombingPaths[BombingPathNr][3].x + (50 * Modifier), BombingPaths[BombingPathNr][3].y + 208 };
					BombingPaths[BombingPathNr][5] = { BombingPaths[BombingPathNr][4].x + (250 * Modifier), BombingPaths[BombingPathNr][4].y + 102 };
					BombingPaths[BombingPathNr][6] = { BombingPaths[BombingPathNr][5].x + (150 * Modifier), BombingPaths[BombingPathNr][5].y - 455 };
					BombingPaths[BombingPathNr][7] = EndPosition;
					IsDiving = true;
					StartTime = 0;

				}
			}
			else {
				/*auto pos{ m_Scene->GetGameObject(EnumStrings[Player0])->GetTransform()->GetPosition() };
				BombingPaths[BombingPathNr][0] = EndPosition;
				BombingPaths[BombingPathNr][1] = { BombingPaths[BombingPathNr][0].x - 300, BombingPaths[BombingPathNr][0].y + 200 };
				BombingPaths[BombingPathNr][2] = { BombingPaths[BombingPathNr][0].x + 300, BombingPaths[BombingPathNr][0].y + 300 };
				BombingPaths[BombingPathNr][3] = pos;
				BombingPaths[BombingPathNr][4] = { BombingPaths[BombingPathNr][3].x - 150, BombingPaths[BombingPathNr][3].y + 108 };
				BombingPaths[BombingPathNr][5] = { BombingPaths[BombingPathNr][4].x + 250, BombingPaths[BombingPathNr][4].y + 102 };
				BombingPaths[BombingPathNr][6] = { BombingPaths[BombingPathNr][5].x + 150, BombingPaths[BombingPathNr][5].y - 455 };
				BombingPaths[BombingPathNr][7] = {EndPosition.x, EndPosition.y };*/
				IsDiving = true;
				//StartTime = 0;
			}
		}
		DiveTimer = DefaultDiveTimer;
	}
}

void EnemyComponent::HandleCaptureAndReturn(glm::vec3& pos)
{
	if (pos.y <= EndPosition.y && CanReturn) {
		IsFullDiving = false;
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

void EnemyComponent::HandleDive(glm::vec3& pos)
{
	if (EnemyType != EnemyType::BOSS) {
		if (pos.y >= WindowSizeY - Cellsize) {
			CanReturn = true;
		}
	}
	else {
		if (IsFullDiving) {
			if (pos.y >= WindowSizeY - Cellsize) {
				CanReturn = true;
			}
		}
		else if (!IsFullDiving && pos.y >= WindowSizeY / 1.7f && !CanReturn) {
			GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Weapon])->SetIsVisible(true);
			GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetPaused(true);
			GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetFrame(2);
			IsBeaming = true;
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
}

void EnemyComponent::Render() const
{
	SDL_Rect rect1{ static_cast<int>(EndPosition.x + 10), static_cast<int>(EndPosition.y + 10), 10,10 };
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 255, 255); // Set the color to blue
	SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rect1); // D
}

void EnemyComponent::DestroyEnemy()
{
	CanDie = true;
	GetGameObject()->EnableCollision(false);
	GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetTexture("explosion.png", 0.1f, 4);
	GetGameObject()->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetOffset({ -40, -25 });
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
