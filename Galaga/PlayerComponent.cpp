#include "PlayerComponent.h"
#include <GalagaMath.h>
#include <TextureComponent.h>
#include <ValuesComponent.h>
#include <Timer.h>
#include "EnemyManager.h"
//#include <Minigin.h>

void PlayerComponent::Update()
{
    if (HasDied) {
        auto deltaTime{ Timer::GetInstance().GetDeltaTime() };
        DeathTimer -= deltaTime;
        if (DeathTimer < DefaultDeathTimer-0.3f) {
            HandleDeathEnd();
        }
        if (DeathTimer <= 0) {
            HandleRespawn();

        }
    }
    else {
        HandleEnemyOverlap();
    }
}

void PlayerComponent::HandleEnemyOverlap()
{
    auto go{ m_Scene->GetGameObject(EnumStrings[EnemyHolder]) };
    if (go) {
        auto children{ go->GetChildren(EnumStrings[Enemy]) };
        auto rect1{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
        for (auto enemy : children) {
            if (enemy->IsMarkedForDestroy()) continue;
            auto rect2{ enemy->GetComponent<TextureComponent>(EnumStrings[Enemy])->GetRect() };
            if (GalagaMath::IsOverlapping(rect1, rect2)) {
                Die();
                enemy->MarkForDestroy();
                m_Scene->GetGameObject(EnumStrings[EnemyHolder])->GetComponent<EnemyManager>()->CheckStatus();
                return;
            }
        }
    }
}

void PlayerComponent::HandleDeathEnd()
{
    auto player{ GetGameObject() };
    player->GetComponent<TextureComponent>()->SetIsVisible(false);
    player->GetComponent<TextureComponent>()->Scale(0.7f, 0.7f);
    player->GetComponent<TextureComponent>()->SetTexture(player->GetName() == EnumStrings[Player0] ? "galaga.png" : "galagaRed.png");
    auto rect = player->GetComponent<TextureComponent>()->GetRect();
    player->GetComponent<TextureComponent>()->SetOffset({ (rect.w * 1.5f) - 1.5f, 0.f });
}

void PlayerComponent::HandleRespawn()
{
    DeathTimer = DefaultDeathTimer;
    HasDied = false;
    auto player{ GetGameObject() };
    player->GetComponent<TextureComponent>()->SetIsVisible(true);
    player->GetComponent<TextureComponent>()->SetPosition((GameWindowSizeX) / 2 - Margin, WindowSizeY - Margin * 3);
    GetGameObject()->EnableCollision(true);
    CanBeGrabbed = true;
}

void PlayerComponent::Die()
{
    HasDied = true;
    auto player{ GetGameObject() };
    player->GetComponent<TextureComponent>()->Scale(2.5f,2.5f);
    player->GetComponent<TextureComponent>()->SetTexture("playerExplosion.png", 0.1f, 4);
    auto rect = player->GetComponent<TextureComponent>()->GetRect();
    player->GetComponent<TextureComponent>()->SetOffset({ -rect.w / 2, -rect.h / 2 });
    auto values{ m_Scene->GetGameObject(EnumStrings[Values])->GetComponent<ValuesComponent>() };
    values->Damage();

    player->EnableCollision(false);

    m_Scene->GetGameObject(EnumStrings[EnemyHolder])->GetComponent<EnemyManager>()->DisableEnemies(DefaultDeathTimer + 1);
}

void PlayerComponent::Grab()
{
    HasDied = true;
    auto player{ GetGameObject() };
    player->GetComponent<TextureComponent>()->SetIsVisible(false);
    auto values{ m_Scene->GetGameObject(EnumStrings[Values])->GetComponent<ValuesComponent>()};
    values->Damage();
    CanBeGrabbed = false;
    //player->EnableCollision(false);
}
