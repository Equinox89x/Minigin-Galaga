#include "OverlapComponent.h"
#include "TransformComponent.h"
#include "TextureComponent.h"
#include "GalagaMath.h"
#include "ValuesComponent.h"

std::vector<dae::GameObject*> dae::OverlapComponent::GetOverlappingObjects(bool hasDimensions)
{
    SDL_Rect rect{};
    if(hasDimensions) {
        rect = GetGameObject()->GetComponent<TextureComponent>()->GetRect();
    }
    else {
        auto pos{ GetGameObject()->GetTransform()->GetPosition() };
        rect = { static_cast<int>(pos.x), static_cast<int>(pos.y) ,1,1 };
    }

    std::vector<dae::GameObject*> children;
    for (auto obj : m_Scene->GetGameObjects()) {
        auto overlapPos{ obj->GetTransform()->GetPosition() };
        SDL_Rect overlapRect{ static_cast<int>(overlapPos.x), static_cast<int>(overlapPos.y) ,1,1 };
        if (IsOverlap(overlapRect, rect)) {
            children.push_back(obj.get());
        }
    }
    return children;
}

bool dae::OverlapComponent::IsOverlap(const SDL_Rect& square1, const SDL_Rect& square2) {
    // Check if the squares overlap on the x-axis
    bool xOverlap = (square1.x < square2.x + square2.w) && (square1.x + square1.w > square2.x);

    // Check if the squares overlap on the y-axis
    bool yOverlap = (square1.y < square2.y + square2.h) && (square1.y + square1.h > square2.y);

    // Return true if there is overlap on both axes
    return xOverlap && yOverlap;
}

void dae::OverlapComponent::Update()
{
    
}
