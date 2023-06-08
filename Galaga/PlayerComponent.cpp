#include "PlayerComponent.h"
#include <GalagaMath.h>
#include <TextureComponent.h>
#include <ValuesComponent.h>
#include <Timer.h>
//#include <Minigin.h>

void PlayerComponent::Update()
{
    if (HasDied) {
        auto deltaTime{ Timer::GetInstance().GetDeltaTime() };
        DeathTimer -= deltaTime;
        if (DeathTimer <= 0) {
            DeathTimer = DefaultDeathTimer;
            HasDied = false;
            auto player{ GetGameObject() };
            player->GetComponent<TextureComponent>()->SetIsVisible(true);
            player->GetComponent<TextureComponent>()->SetPosition(WindowSizeX / 2 - Margin, WindowSizeY - SubMargin * 2);

        }
    }
    else {
        auto childen{ m_Scene->GetGameObjects("Enemy") };
        auto rect1{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
        for (auto enemy : childen) {
            if (enemy->IsMarkedForDestroy()) continue;
            auto rect2{ enemy->GetComponent<TextureComponent>("Enemy")->GetRect() };
            if (GalagaMath::IsOverlapping(rect1, rect2)) {
                Die();
                enemy->MarkForDestroy();
                return;
            }
        }
    }
}

void PlayerComponent::Die()
{
    HasDied = true;
    auto player{ GetGameObject() };
    player->GetComponent<TextureComponent>()->SetIsVisible(false);
    auto values{ player->GetComponent<ValuesComponent>() };
    values->Damage();
}
