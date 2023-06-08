#pragma once
#include "Component.h"
#include <Scene.h>
using namespace dae;

class PlayerComponent final : public Component
{

public:
    PlayerComponent(Scene* scene) : m_Scene{ scene } {};
    ~PlayerComponent() = default;
    PlayerComponent(const PlayerComponent&) = delete;
    PlayerComponent(PlayerComponent&&) noexcept = delete;
    PlayerComponent& operator=(const PlayerComponent&) = delete;
    PlayerComponent& operator=(PlayerComponent&&) noexcept = delete;
    virtual void Update() override;

    void Die();
    bool IsDead() { return HasDied; };

private:

    Scene* m_Scene{};
    bool HasDied{ false };
    float DefaultDeathTimer{ 4 }, DeathTimer{ DefaultDeathTimer };
};