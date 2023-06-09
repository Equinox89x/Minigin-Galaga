#include "EnemyComponent.h"
#include "TransformComponent.h"
#include <Timer.h>
#include <random>
#include <Minigin.h>
#include <TextureComponent.h>
#include <glm/gtx/rotate_vector.hpp>
#include "ShootComponent.h"

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

	//RandomPath = std::rand() % static_cast<int>(Paths.size());
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

	//if -> game movement, else -> Spawn movement
	if (!IsStart) {
		if (IsBeaming) {
			BeamTinmer -= deltaTime;
			if (BeamTinmer <= 0) {
				IsBeaming = false;
				CanReturn = true;
				GetGameObject()->GetComponent<TextureComponent>("Weapon")->SetIsVisible(false);
				GetGameObject()->GetComponent<TextureComponent>("Enemy")->SetPaused(false);
				GetGameObject()->GetComponent<TextureComponent>("Enemy")->SetFrame(1);
			}
		}

		if (IsDiving) {
			float movement{ deltaTime * 450 };
			auto pos{ GetGameObject()->GetTransform()->GetPosition() };
			if (EnemyType != EnemyType::BOSS) {
				if (pos.y >= WindowSizeY - 60*2) {
					CanReturn = true;
				}
			}
			else {
				if (pos.y >= WindowSizeY / 2 && !CanReturn) {
					GetGameObject()->GetComponent<TextureComponent>("Weapon")->SetIsVisible(true);
					GetGameObject()->GetComponent<TextureComponent>("Enemy")->SetPaused(true);
					GetGameObject()->GetComponent<TextureComponent>("Enemy")->SetFrame(2);
					IsBeaming = true;
				}
			}
			if (!IsBeaming) GetGameObject()->GetTransform()->AddTranslate(ManagerMovement.x, CanReturn ? -movement : movement);

			pos = GetGameObject()->GetTransform()->GetPosition();
			if (pos.y <= EndPosition.y && CanReturn) {
				IsDiving = false;
				CanReturn = false;
				GetGameObject()->GetTransform()->Translate(EndPosition.x, EndPosition.y);
			}
		}
		else {
			//check chance to start a dive/attack/shot
			DiveTimer -= deltaTime;
			if (DiveTimer <= 0) {
				
				std::random_device randomDevice;
				std::mt19937 generatedNr(randomDevice());
				std::uniform_real_distribution<double> distributeVal(0.0, 1.0);
				double chance = distributeVal(generatedNr);

				if (chance <= DesiredChance) {
					if (EnemyType == EnemyType::ZAKO) {
						std::mt19937 generatedNr2(randomDevice());
						std::uniform_real_distribution<double> distributeVal2(0.0, 1.0);
						double chance2 = distributeVal2(generatedNr2);
						if (chance2 <= 0.5f) {
							GetGameObject()->GetComponent<ShootComponent>()->Shoot();
						}
						else {
							IsDiving = true;
						}
					}
					else {
						IsDiving = true;
					}
				}
				DiveTimer = DefaultDiveTimer;
			}
		}
	}
	else {
		TimeBeforeStart -= deltaTime;
		if (TimeBeforeStart <= 0) {
			StartTime += deltaTime*1.5f;

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

			//disable start state when the enemy is near it's estimated location.
			auto comp{ GetGameObject()->GetComponent<TextureComponent>()};
			SDL_Rect rect1{ static_cast<int>(EndPosition.x), static_cast<int>(EndPosition.y), 2,2 };
			SDL_Rect rect2{ static_cast<int>(comp->GetRect().x), static_cast<int>(comp->GetRect().y), comp->GetRect().w/2,comp->GetRect().h/2};
			if (GalagaMath::IsOverlapping(rect1, rect2)) {
				comp->SetPosition(EndPosition.x, EndPosition.y);
				IsStart = false;
				
			}
		}
	}
}

void EnemyComponent::DestroyEnemy()
{
	CanDie = true;
	GetGameObject()->EnableCollision(false);
	GetGameObject()->GetComponent<TextureComponent>("Enemy")->SetTexture("explosion.png", 0.1f, 4);
	GetGameObject()->GetComponent<TextureComponent>("Enemy")->SetOffset({ -40, -25 });
}

void EnemyComponent::TranslateInitialPosition(glm::vec2 addedPos)
{
	EndPosition += addedPos;
}
