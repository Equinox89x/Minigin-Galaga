#include "ModeSelector.h"
#include "TextObjectComponent.h"
#include "Transformcomponent.h"
#include "Minigin.h"

void dae::ModeSelector::CycleGameMode(bool isMoveUp)
{
	if (!isMoveUp) {
		if (m_SelectedGameMode != GameMode::VERSUS) {
			m_SelectedGameMode = GameMode(static_cast<int>(m_SelectedGameMode) + 1);
		}
	}
	else {
		if (m_SelectedGameMode != GameMode::SOLO) {
			m_SelectedGameMode = GameMode(static_cast<int>(m_SelectedGameMode) - 1);
		}
	}

	auto selector{ GetGameObject() };
	auto pos{ selector->GetTransform()->GetPosition() };
	selector->GetTransform()->Translate(pos.x, WindowSizeY/2 + Margin * static_cast<int>(m_SelectedGameMode), pos.z);
}

void dae::ModeSelector::StartGame(GameObject* pMenu)
{
	// 1. hide menu
	pMenu->SetIsHidden(true);

	switch (m_SelectedGameMode)
	{
	case dae::ModeSelector::GameMode::SOLO:
		CreateScore(m_pScene);
		CreateMainGalaga(m_pScene);
		//2. show first stage
		CreateStage(m_pScene);
		break;
	case dae::ModeSelector::GameMode::COOP:
		CreateScore(m_pScene);
		CreateMainGalaga(m_pScene);
		CreateSecondaryGalaga(m_pScene);
		//2. show first stage
		CreateStage(m_pScene);
		break;
	case dae::ModeSelector::GameMode::VERSUS:
		CreateMainGalaga(m_pScene);
		CreateSecondaryGalaga(m_pScene);
		//2. show versus stage
		CreateVersusGalaga(m_pScene);
		break;
	default:
		break;
	}

	//TODO 3. stage start sequence

}