#include "ShootComponent.h"
#include "TransformComponent.h"
#include "Timer.h"
#include <Minigin.h>
#include "EnemyComponent.h"
#include <ValuesComponent.h>
#include "PlayerComponent.h"
#include "Callback.h"
#include "EnemyManager.h"
#include "CapturedComponent.h"
#include "OpposerComponent.h"


void ShootComponent::Shoot()
{
	if (auto comp{ GetGameObject()->GetComponent<PlayerComponent>() }) {
		if (comp->IsDead()) return;
	}

	if(IsOpposer){
		auto childen{ m_Scene->GetGameObjects(EnumStrings[EnemyBullet] + StrId) };
		if (static_cast<int>(childen.size()) < m_MaxBullets) {
			CreateBullet(EnumStrings[EnemyBullet], EnumStrings[EnemyBullet], "png");
		}
	}
	else if (IsEnemy) {
		CreateBullet(EnumStrings[EnemyBullet], EnumStrings[EnemyBullet], "png");
	}
	else {
		auto childen{ m_Scene->GetGameObjects(EnumStrings[NormalBullet] + StrId) };
		if (static_cast<int>(childen.size()) < m_MaxBullets) {
			CreateBullet("bulletPlayer", EnumStrings[NormalBullet], "png");
		}
	}
}

void ShootComponent::CreateBullet(std::string fileName, std::string name, std::string extention) {
	auto bullet{ std::make_shared<GameObject>() };
	bullet->AddComponent(new TextureComponent());
	bullet->GetComponent<TextureComponent>()->SetTexture(fileName + "." + extention);
	bullet->GetComponent<TextureComponent>()->Scale(3, 3);
	bullet->SetName(name + StrId);

	auto pos{ GetGameObject()->GetTransform()->GetPosition() };
	auto positionY{ IsOpposer || IsEnemy ? 60 : 0 };
	auto rect = GetGameObject()->GetComponent<TextureComponent>()->GetRect();
	auto rect2 = bullet->GetComponent<TextureComponent>()->GetRect();
	bullet->GetTransform()->Translate(pos.x + ((rect.w/2)-(rect2.w/2)), pos.y + positionY);
	if (IsOpposer || IsEnemy) {
		bullet->GetTransform()->Rotate(0, 180, 0);
	}
	m_Scene->Add(bullet);
	if (!IsOpposer && !IsEnemy) {
		if (auto go{ m_Scene->GetGameObject(EnumStrings[Values]) }) {
			go->GetComponent<ValuesComponent>()->IncreaseShots();
		}
	}
}

void ShootComponent::Update()
{
	float deltaTime{ Timer::GetInstance().GetDeltaTime() };
	std::vector<std::shared_ptr<GameObject>> childen{};
	if (IsEnemy || IsOpposer) {
		childen = m_Scene->GetGameObjects(EnumStrings[EnemyBullet] + StrId);
	}
	else {
		childen = m_Scene->GetGameObjects(EnumStrings[NormalBullet] + StrId);
	}

	for (auto bullet : childen) {
		auto pos{ bullet->GetTransform()->GetPosition() };

		float movement{ deltaTime * m_MoveSpeed };
		if (!IsEnemy && !IsOpposer) {
			auto newPos{ pos.y - movement };
			if (newPos < 0) {
				bullet->MarkForDestroy();
				continue;
			}
			else {
				bullet->GetTransform()->AddTranslate(0, -movement);
			}
		}
		else {
			auto newPos{ pos.y + movement };
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
		if (!IsEnemy && !IsOpposer) HandleCapturedFigherOverlap(bullet);
	}
}

void ShootComponent::HandleOpposerOverlap(std::shared_ptr<dae::GameObject>& bullet)
{
	auto opposers{ m_Scene->GetOverlappingObjects(bullet.get(), EnumStrings[Opposer], "", true, true) };
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
	auto players{ m_Scene->GetOverlappingObjects(bullet.get(), EnumStrings[PlayerGeneral], "", false, true, true) };

	for (auto player : players) {
		//if (auto comp1{ player->GetComponent<ValuesComponent>() }) {
		if (auto comp{ player->GetComponent<PlayerComponent>() }) {
			if (auto comp1{ m_Scene->GetGameObject(EnumStrings[Values])->GetComponent<ValuesComponent>() }) {
				comp->Die();
			}
		}
		bullet->MarkForDestroy();
	}
}

void ShootComponent::HandleCapturedFigherOverlap(std::shared_ptr<dae::GameObject>& bullet)
{
	auto players{ m_Scene->GetOverlappingObjects(bullet.get(), EnumStrings[CapturedFighter], "", false, true, true) };

	for (auto player : players)
	{
		if (auto comp{ player->GetComponent<CapturedComponent>() }) {
			if (auto comp1{ m_Scene->GetGameObject(EnumStrings[Values])->GetComponent<ValuesComponent>() }) {
				comp->Die();
			}
		}
		bullet->MarkForDestroy();
		m_Scene->GetGameObject(EnumStrings[EnemyHolder])->GetComponent<EnemyManager>()->CheckStatus();
	}
}

void ShootComponent::HandleEnemyOverlap(std::shared_ptr<dae::GameObject>& bullet)
{
	auto enemies{ m_Scene->GetOverlappingObjects(bullet.get(), EnumStrings[Enemy], EnumStrings[EnemyHolder], true, true, true) };
	for (auto enemy : enemies) {
		if (auto comp{ enemy->GetComponent<EnemyComponent>() }) {
			if (comp->GetLives() <= 1) {
				comp->DestroyEnemy();
			}
			else {
				comp->Damage();
			}
			bullet->MarkForDestroy();
			m_Scene->GetGameObject(EnumStrings[EnemyHolder])->GetComponent<EnemyManager>()->CheckStatus();

			m_Scene->GetGameObject(EnumStrings[Values])->GetComponent<ValuesComponent>()->IncreaseScore(comp->GetScore());
			m_Scene->GetGameObject(EnumStrings[Values])->GetComponent<ValuesComponent>()->IncreaseHits();

			m_Scene->GetGameObject(EnumStrings[EnemyHolder])->GetComponent<EnemyManager>()->CheckStatus();
			break;
		}
	}
}

void ShootComponent::Render() const
{
}