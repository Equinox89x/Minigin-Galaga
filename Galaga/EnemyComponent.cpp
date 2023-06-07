#include "EnemyComponent.h"
#include "TransformComponent.h"
#include <Timer.h>
#include <random>
#include <Minigin.h>
#include <TextureComponent.h>
#include <glm/gtx/rotate_vector.hpp>

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

	Paths.push_back(vec1);
	Paths.push_back(vec2);

	auto go1{ m_Scene->GetGameObject("test") };
	auto go2{ m_Scene->GetGameObject("test1") };
	auto go3{ m_Scene->GetGameObject("test2") };
	auto go4{ m_Scene->GetGameObject("test3") };
	go1->GetComponent<TextureComponent>()->SetPosition(P0.x, P0.y);
	go2->GetComponent<TextureComponent>()->SetPosition(P1.x, P1.y);
	go3->GetComponent<TextureComponent>()->SetPosition(P2.x, P2.y);
	go4->GetComponent<TextureComponent>()->SetPosition(P3.x, P3.y);

	RandomPath = std::rand() % static_cast<int>(Paths.size());
}

//int BinomialCoeff(int n, int i) {
//	if (i == 0 || i == n)
//		return 1;
//
//	int numerator = 1;
//	int denominator = 1;
//
//	for (int j = 1; j <= i; j++) {
//		numerator *= (n - j + 1);
//		denominator *= j;
//	}
//
//	return numerator / denominator;
//}
//
//glm::vec2 CalculateBezierPoint(float t, const std::vector<glm::vec2>& controlPoints) {
//	int n{ static_cast<int>(controlPoints.size()) - 1 };
//	float x{ 0.0 };
//	float y{ 0.0 };
//
//	for (int i = 0; i <= n; i++) {
//		float blend{ static_cast<float>(BinomialCoeff(n, i) * pow(1 - t, n - i) * pow(t, i)) };
//		x += controlPoints[i].x * blend;
//		y += controlPoints[i].y * blend;
//	}
//
//	return { x, y };
//}

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
				if (GetGameObject()->GetTransform()->GetPosition().y >= WindowSizeY / 2 && !CanReturn) {
					GetGameObject()->GetComponent<TextureComponent>("Weapon")->SetIsVisible(true);
					GetGameObject()->GetComponent<TextureComponent>("enemy")->SetPaused(true);
					GetGameObject()->GetComponent<TextureComponent>("enemy")->SetFrame(2);
					IsBeaming = true;
				}
			}
			if (!IsBeaming) GetGameObject()->GetTransform()->AddTranslate(0, CanReturn ? -movement : movement);

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
	else {
		TimeBeforeStart -= deltaTime;
		if (TimeBeforeStart <= 0) {
			StartTime += deltaTime;
			if (StartTime <= 1) {
				int randomNumber{ std::rand() % static_cast<int>(Paths.size()) };
				glm::vec2 point{ GalagaMath::CalculateBezierPoint(StartTime, Paths[randomNumber]) };
				GetGameObject()->GetTransform()->Translate(point);

				//glm::vec2 nextBezierPoint = GalagaMath::CalculateBezierPoint(StartTime + 0.01f, Paths[0]);
				//glm::vec2 forwardVector = glm::normalize(glm::vec2(nextBezierPoint.x - point.x, nextBezierPoint.y - point.y));
				//float rotationAngle = glm::degrees(std::atan2(forwardVector.y, forwardVector.x));

				//// Print the rotated point
				//GetGameObject()->GetComponent<TextureComponent>("enemy")->Rotate(rotationAngle);

				SDL_Rect rect1{ static_cast<int>(EndPosition.x), static_cast<int>(EndPosition.y), 2,2 };
				SDL_Rect rect2{ static_cast<int>(point.x), static_cast<int>(point.y), 2,2 };
				if (GalagaMath::IsOverlapping(rect1, rect2)) {
					IsStart = false;
				}
			}
		}
	}
}

void EnemyComponent::DestroyEnemy()
{
	GetGameObject()->EnableCollision(false);
	CanDie = true;
}
