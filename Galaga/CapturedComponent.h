#pragma once
#include <Scene.h>
#include <Component.h>
#include "Locator.h"
#include "Audio.h"
using namespace dae;

class CapturedComponent final : public Component
{

public:
    CapturedComponent(Scene* scene, GameObject* player, GameObject* enemy) : m_Scene{ scene }, Player{ player }, EnemyObj{ enemy } 
    {
        m_pAudioService = Locator::getAudio();
    };
    ~CapturedComponent() = default;
    CapturedComponent(const CapturedComponent&) = delete;
    CapturedComponent(CapturedComponent&&) noexcept = delete;
    CapturedComponent& operator=(const CapturedComponent&) = delete;
    CapturedComponent& operator=(CapturedComponent&&) noexcept = delete;

    virtual void Initialize() override;
    virtual void Update() override;

    void SetIsTurned(bool isTurned) { IsTurned = isTurned; };
    void Die();

private:
    Scene* m_Scene{};
    glm::vec3 StartPosition{}, MoveToPosition{};
    GameObject* EnemyObj{};
    GameObject* Player{};
    Audio* m_pAudioService;

    float MaxMoveSpeedChangeTimer{ 1 }, MinMoveSpeedChangeTimer{ 0.2f }, MoveSpeedChangeTimer{ MaxMoveSpeedChangeTimer };
    float MaxMoveSpeed{ 500 }, MinMoveSpeed{ 100 }, MoveSpeed{ MaxMoveSpeed };
    bool IsGoingRight{ true }, IsTurned{ false }, ReachedOverlap{ false };
    int MoveModifier{ 1 };

    float IsTurnedTimer{ 2 };

    float MoveT{ 0 };

    float DefaultAttackTimer{ 2 }, AttackTimer{ DefaultAttackTimer };

    float DefaultDeathTimer{ 0.3f }, DeathTimer{ DefaultDeathTimer };
    bool HasDied{ false };

}; 