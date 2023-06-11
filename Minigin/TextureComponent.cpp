#include "TextureComponent.h"
#include "Font.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Timer.h"

//dae::TextureComponent::TextureComponent(const std::string& filename, glm::vec2 position, glm::vec2 scale, const float rotation, float animSpeed, int nrOfFrames) :
//    FileName{ filename },
//    Angle{ rotation },
//    NrOfFrames{ nrOfFrames },
//    DefaultAnimTime{ animSpeed },
//    m_Position{ position },
//    m_Scale{ scale }
//{}
//
//void dae::TextureComponent::Initialize() {
//    if (FileName != "") {
//        SetPosition(m_Position.x, m_Position.y);
//        Scale(m_Scale.x, m_Scale.y);
//        Rotate(Angle);
//        SetTexture(FileName, DefaultAnimTime, NrOfFrames);
//        HandleAnimation();
//    }
//}

void dae::TextureComponent::Update()
{
    auto dt{ Timer::GetInstance().GetDeltaTime() };
    if (!IsPaused) {
        AnimTimer -= dt;
        if (AnimTimer <= 0) {
            CurrentFrame++;
            AnimTimer = DefaultAnimTime;
        }
        if (CurrentFrame > NrOfFrames) {
            CurrentFrame = 1;
        }
    }

    HandleAnimation();
}
void dae::TextureComponent::HandleAnimation() {
    const auto& pos{ GetGameObject()->GetTransform()->GetPosition() };
    const auto& scale{ GetGameObject()->GetTransform()->GetScale() };

    SDL_QueryTexture(m_pTexture->GetSDLTexture(), nullptr, nullptr, &m_SrcRect.w, &m_SrcRect.h);
    if (NrOfFrames > 1) {
        m_SrcRect.w /= NrOfFrames;
        m_SrcRect.x = m_SrcRect.w * (CurrentFrame - 1);
    }
    m_DstRect = { 0, 0, m_SrcRect.w, m_SrcRect.h };

    if (NrOfFrames > 1) m_DstRect.x = m_SrcRect.w * CurrentFrame;
    m_DstRect.x += static_cast<int>(pos.x + Offset.x) - m_SrcRect.x;
    m_DstRect.y = static_cast<int>(pos.y + Offset.y);

    m_DstRect.w = static_cast<int>(m_SrcRect.w * scale.x);
    m_DstRect.h = static_cast<int>(m_SrcRect.h * scale.y);

    m_Rect = m_DstRect;
}

void dae::TextureComponent::Render() const
{
    if (!Isvisible) return;

    if (NrOfFrames > 1) {
        SDL_Point center = { m_SrcRect.w / 2, m_SrcRect.h / 2 };
        SDL_RenderCopyEx(Renderer::GetInstance().GetSDLRenderer(), m_pTexture->GetSDLTexture(), &m_SrcRect, &m_DstRect, Angle, nullptr, SDL_FLIP_NONE);
    }
    else {
        SDL_RenderCopyEx(Renderer::GetInstance().GetSDLRenderer(), m_pTexture->GetSDLTexture(), nullptr, &m_DstRect, Angle, nullptr, SDL_FLIP_NONE);
    }

    //SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
    //SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &m_Rect); // D
}

void dae::TextureComponent::SetTexture(const std::string& filename, float animSpeed, int nrOfFrames, bool resetAnim)
{
    FileName = filename;
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
    NrOfFrames = nrOfFrames;
    DefaultAnimTime = animSpeed;
    if (resetAnim) {
        CurrentFrame = 1;
        AnimTimer = DefaultAnimTime;
    }
}

void dae::TextureComponent::SetPosition(const float x, const float y)
{
    GetGameObject()->GetTransform()->Translate(x, y, 0.0f);
    m_needsUpdate = true;
}

void dae::TextureComponent::AddPosition(const float x, const float y)
{
    GetGameObject()->GetTransform()->AddTranslate(x, y, 0.0f);
    m_needsUpdate = true;
}

void dae::TextureComponent::SetWorldPosition(const float x, const float y)
{
    GetGameObject()->GetTransform()->TranslateWorld(x, y, 0.0f);
    m_needsUpdate = true;
}

void dae::TextureComponent::Scale(const float x, const float y)
{
    GetGameObject()->GetTransform()->SetScale(x, y, 1);
    m_needsUpdate = true;
}


void dae::TextureComponent::Rotate(const float angle)
{
    GetGameObject()->GetTransform()->Rotate(angle);
    Angle = angle;
    m_needsUpdate = true;
}

