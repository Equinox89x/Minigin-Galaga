#include "ShootComponent.h"
#include "TransformComponent.h"
#include "Timer.h"
#include <Minigin.h>
#include "EnemyComponent.h"
#include <ValuesComponent.h>
#include "PlayerComponent.h"
#include "Callback.h"


void ShootComponent::Shoot()
{
	if (GetGameObject()->GetComponent<PlayerComponent>()->IsDead()) return;
	auto childen{ m_Scene->GetGameObjects("bullet"+ StrId) };
	if (static_cast<int>(childen.size()) < m_MaxBullets) {
		auto bullet{ std::make_shared<GameObject>()};
		bullet->AddComponent(new TextureComponent());
		bullet->GetComponent<TextureComponent>()->SetTexture("bulletPlayer.png");
		bullet->GetComponent<TextureComponent>()->Scale(5, 5);
		bullet->SetName("bullet"+ StrId);

		auto pos {GetGameObject()->GetTransform()->GetPosition() };
		bullet->GetTransform()->Translate(pos.x + 23, pos.y);
		m_Scene->Add(bullet);
		GetGameObject()->GetComponent<ValuesComponent>()->IncreaseShots();
	}
}

void ShootComponent::Initialize()
{
	//Bullets = std::make_shared<dae::GameObject>();
	//Bullets->SetName("bulletHolder");
	//m_Scene.Add(Bullets);
}

void ShootComponent::FixedUpdate()
{
	//this has to happen in overlap and destroy the selected bullet
	//m_Scene.Remove(m_Bullets[0]);
}

void ShootComponent::Update()
{
	float deltaTime{ Timer::GetInstance().GetDeltaTime() };
	auto childen{ m_Scene->GetGameObjects("bullet"+ StrId) };
	for (auto bullet : childen) {
		auto pos{ bullet->GetTransform()->GetPosition() };
		auto newPos{ pos.y - (deltaTime * m_MoveSpeed) };
		if (newPos < 0) {
			bullet->MarkForDestroy();
			continue;
		}
		else {
			bullet->GetTransform()->Translate(pos.x, newPos, 0);
		}

		auto enemies{ m_Scene->GetOverlappingObjects(bullet.get(), "Enemy", true)};
		for (auto enemy : enemies) {
 			if (auto comp{ enemy->GetComponent<EnemyComponent>() }) {
				enemy->GetComponent<TextureComponent>("Enemy")->SetTexture("explosion.png", 0.1f, 4);
				enemy->GetComponent<TextureComponent>("Enemy")->SetOffset({ -40, -25 });
				bullet->GetComponent<TextureComponent>()->Scale(1, 1);
				//bullet->GetComponent<TextureComponent>()->SetOffset({ -75, -50 });
				comp->DestroyEnemy();
				bullet->MarkForDestroy();
				GetGameObject()->GetComponent<ValuesComponent>()->IncreaseScore(comp->GetScore());
				GetGameObject()->GetComponent<ValuesComponent>()->IncreaseHits();
				break;
			}
		}
	}

	if (m_Scene->GetGameObjects("Enemy").empty()) 
		m_pCallback->Notify(GetGameObject(), Event::StageCleared);
}

void ShootComponent::Render() const
{
}