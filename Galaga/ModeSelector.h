#pragma once
#include "Component.h"
#include "Scene.h"
#include <functional>

namespace dae
{
	class ModeSelector final : public Component
	{

	public:
		ModeSelector(dae::Scene* scene, std::function<void(dae::Scene*, bool)> createMainGalaga, std::function<void(dae::Scene*)> createSecondaryGalaga, std::function<void(dae::Scene*)> createStage, std::function<void(dae::Scene*)> createVersusGalaga, std::function<void(dae::Scene*)> createScore) : m_pScene{ scene } {
			CreateMainGalaga = createMainGalaga;
			CreateSecondaryGalaga = createSecondaryGalaga;
			CreateVersusGalaga = createVersusGalaga;
			CreateStage = createStage;
			CreateScore = createScore;

		};
		~ModeSelector() = default;
		ModeSelector(const ModeSelector&) = delete;
		ModeSelector(ModeSelector&&) noexcept = delete;
		ModeSelector& operator=(const ModeSelector&) = delete;
		ModeSelector& operator=(ModeSelector&&) noexcept = delete;

		void CycleGameMode(bool isMoveUp);
		void StartGame(GameObject* pMenu);

	private:
		enum class GameMode {
			SOLO, COOP, VERSUS
		};

		GameMode m_SelectedGameMode{ GameMode::SOLO };
		bool m_IsMainMenu{ true };
		bool m_IsMenu{ false };

		dae::Scene* m_pScene;

		std::function<void(dae::Scene* scene, bool isVersus)> CreateMainGalaga;
		std::function<void(dae::Scene* scene)> CreateSecondaryGalaga;		
		std::function<void(dae::Scene* scene)> CreateStage;
		std::function<void(dae::Scene* scene)> CreateVersusGalaga;
		std::function<void(dae::Scene* scene)> CreateScore;

		//virtual void Update() override;
		//virtual void Render() const override;
		//virtual void FixedUpdate() override;

	};
}

