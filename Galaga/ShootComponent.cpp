#include "ShootComponent.h"
#include "TransformComponent.h"
#include "Timer.h"
#include <Minigin.h>
#include "EnemyComponent.h"
#include <ValuesComponent.h>
#include "PlayerComponent.h"
#include "Callback.h"
#include "EnemyManager.h"
#include "OpposerComponent.h"


void ShootComponent::Shoot()
{
	if (auto comp{ GetGameObject()->GetComponent<PlayerComponent>() }) {
		if (comp->IsDead()) return;
	}

	if(IsOpposer){
		auto childen{ m_Scene->GetGameObjects("enemyBullet" + StrId) };
		if (static_cast<int>(childen.size()) < m_MaxBullets) {
			CreateBullet("enemyBullet", "enemyBullet", "png");
		}
	}
	else if(IsEnemy){
		CreateBullet("enemyBullet", "enemyBullet", "png");
	}
	else {
		auto childen{ m_Scene->GetGameObjects("bullet" + StrId) };
		if (static_cast<int>(childen.size()) < m_MaxBullets) {
			CreateBullet("bulletPlayer", "bullet", "png");
		}
	}

	/*auto childen{ m_Scene->GetGameObjects("bullet"+ StrId) };
	if (static_cast<int>(childen.size()) < m_MaxBullets) {
		auto bullet{ std::make_shared<GameObject>()};
		bullet->AddComponent(new TextureComponent());
		bullet->GetComponent<TextureComponent>()->SetTexture("bulletPlayer.png");
		bullet->GetComponent<TextureComponent>()->Scale(3, 3);
		bullet->SetName("bullet"+ StrId);

		auto pos {GetGameObject()->GetTransform()->GetPosition() };
		bullet->GetTransform()->Translate(pos.x + 23, pos.y);
		if (IsOpposer) {
			bullet->GetTransform()->Rotate(0, 180, 0);
		}
		m_Scene->Add(bullet);
		GetGameObject()->GetComponent<ValuesComponent>()->IncreaseShots();
	}*/

	//auto childen{ m_Scene->GetGameObjects("enemyBullet" + StrId) };
	//if (static_cast<int>(childen.size()) < m_MaxBullets) {
	//	auto bullet{ std::make_shared<GameObject>() };
	//	bullet->AddComponent(new TextureComponent());
	//	bullet->GetComponent<TextureComponent>()->SetTexture("enemyBullet.png");
	//	bullet->GetComponent<TextureComponent>()->Scale(3, 3);
	//	bullet->SetName("enemyBullet" + StrId);

	//	auto pos{ GetGameObject()->GetTransform()->GetPosition() };
	//	bullet->GetTransform()->Translate(pos.x + 23, pos.y + 60);
	//	bullet->GetTransform()->Rotate(0, 180, 0);
	//	m_Scene->Add(bullet);
	//	GetGameObject()->GetComponent<ValuesComponent>()->IncreaseShots();
	//}
}

void ShootComponent::CreateBullet(std::string fileName, std::string name, std::string extention) {
	auto bullet{ std::make_shared<GameObject>() };
	bullet->AddComponent(new TextureComponent());
	bullet->GetComponent<TextureComponent>()->SetTexture(fileName + "." + extention);
	bullet->GetComponent<TextureComponent>()->Scale(3, 3);
	bullet->SetName(name + StrId);

	auto pos{ GetGameObject()->GetTransform()->GetPosition() };
	auto position{ IsOpposer || IsEnemy ? 60 : 0 };
	bullet->GetTransform()->Translate(pos.x + 23, pos.y + position);
	if (IsOpposer || IsEnemy) {
		bullet->GetTransform()->Rotate(0, 180, 0);
	}
	m_Scene->Add(bullet);
	if (!IsOpposer && !IsEnemy) {
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
	std::vector<std::shared_ptr<GameObject>> childen{};
	if (IsEnemy || IsOpposer) {
		childen = m_Scene->GetGameObjects("enemyBullet"+ StrId);
	}
	else {
		childen = m_Scene->GetGameObjects("bullet"+ StrId);
	}

	for (auto bullet : childen) {
		auto pos{ bullet->GetTransform()->GetPosition() };

		float movement{ deltaTime * m_MoveSpeed };
		if (!IsEnemy && !IsOpposer) {
			auto newPos{ pos.y - movement};
			if (newPos < 0) {
				bullet->MarkForDestroy();
				continue;
			}
			else {
				bullet->GetTransform()->AddTranslate(0, -movement);
			}
		}
		else {
			auto newPos{ pos.y + movement};
			if (newPos > 720) {
				bullet->MarkForDestroy();
				continue;
			}
			else {
				bullet->GetTransform()->AddTranslate(0, movement);
			}
		}



		if (!IsVersus) {
			IsEnemy ? HandlePlayerOverlap(bullet) : HandleEnemyOverlap(bullet);
		}
		else {
			IsOpposer ? HandlePlayerOverlap(bullet) : HandleOpposerOverlap(bullet);
		}
	}
}

void ShootComponent::HandleOpposerOverlap(std::shared_ptr<dae::GameObject>& bullet)
{
	auto opposers{ m_Scene->GetOverlappingObjects(bullet.get(), "Opposer", "", true) };
	if (opposers.size() <= 0) return;

	if (auto opposer{ opposers[0] }) {
		if (auto comp{ opposer->GetComponent<OpposerComponent>() }) {
			if (comp->GetLives() <= 1) {
				comp->DestroyOpposer();
			}
			else {
				comp->Damage();
			}
		}
		bullet->MarkForDestroy();
	}
}

void ShootComponent::HandlePlayerOverlap(std::shared_ptr<dae::GameObject>& bullet)
{
	auto players{ m_Scene->GetOverlappingObjects(bullet.get(), "Player0", "", true) };
	if (players.size() <= 0) return;

	if (auto player{ players[0] }) {
		if (auto comp1{ player->GetComponent<ValuesComponent>() }) {
			if (auto comp{ player->GetComponent<PlayerComponent>() }) {
				comp->Die();
			}
		}
		bullet->MarkForDestroy();
	}
}

void ShootComponent::HandleEnemyOverlap(std::shared_ptr<dae::GameObject>& bullet)
{
	auto enemies{ m_Scene->GetOverlappingObjects(bullet.get(), "Enemy", "EnemyHolder", true) };
	for (auto enemy : enemies) {
		if (auto comp{ enemy->GetComponent<EnemyComponent>() }) {
			bullet->GetComponent<TextureComponent>()->Scale(1, 1);
			//bullet->GetComponent<TextureComponent>()->SetOffset({ -75, -50 });
			if (comp->GetLives() <= 1) {
				comp->DestroyEnemy();
			}
			else {
				comp->Damage();
			}
			bullet->MarkForDestroy();

			GetGameObject()->GetComponent<ValuesComponent>()->IncreaseScore(comp->GetScore());
			GetGameObject()->GetComponent<ValuesComponent>()->IncreaseHits();

			m_Scene->GetGameObject("EnemyHolder")->GetComponent<EnemyManager>()->CheckStatus();
			break;
		}
	}
}

void ShootComponent::Render() const
{
}