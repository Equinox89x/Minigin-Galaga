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
            auto player{ GetGameObject() };
            player->GetComponent<TextureComponent>()->SetIsVisible(false);
            player->GetComponent<TextureComponent>()->SetTexture("galaga.png");
            player->GetComponent<TextureComponent>()->Scale(0.7f, 0.7f);
            player->GetComponent<TextureComponent>()->SetOffset({ 40, 25 });

        }
        if (DeathTimer <= 0) {
            DeathTimer = DefaultDeathTimer;
            HasDied = false;
            auto player{ GetGameObject() };
            player->GetComponent<TextureComponent>()->SetIsVisible(true);
            player->GetComponent<TextureComponent>()->SetPosition(WindowSizeX / 2 - Margin, WindowSizeY - SubMargin * 2);
            GetGameObject()->EnableCollision(true);

        }
    }
    else {
        auto go{ m_Scene->GetGameObject("EnemyHolder") };
        if (go) {
            auto children{ go->GetChildren("Enemy") };
            auto rect1{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
            for (auto enemy : children) {
                if (enemy->IsMarkedForDestroy()) continue;
                auto rect2{ enemy->GetComponent<TextureComponent>("Enemy")->GetRect() };
                if (GalagaMath::IsOverlapping(rect1, rect2)) {
                    Die();
                    enemy->MarkForDestroy();
                    m_Scene->GetGameObject("EnemyHolder")->GetComponent<EnemyManager>()->CheckStatus();
                    return;
                }
            }
        }
    }
}

void PlayerComponent::Die()
{
    HasDied = true;
    auto player{ GetGameObject() };
    //player->GetComponent<TextureComponent>()->SetIsVisible(false);
    player->GetComponent<TextureComponent>()->SetTexture("playerExplosion.png", 0.1f, 4);
    player->GetComponent<TextureComponent>()->SetOffset({ -40, -25 });
    player->GetComponent<TextureComponent>()->Scale(2.5f,2.5f);
    auto values{ player->GetComponent<ValuesComponent>() };
    values->Damage();

    GetGameObject()->EnableCollision(false);
}
