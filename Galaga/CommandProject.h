#pragma once
#include "Command.h"
#include "ModeSelector.h"
#include "ShootComponent.h"
#include "OpposerComponent.h"

namespace dae{
#pragma region Menu
	class CycleGameMode final : public Command
	{
	public:
		CycleGameMode(ModeSelector* const object, bool isMoveUp) : m_pObject(object), m_IsMoveUp{ isMoveUp } {}
		void Execute() override
		{
			m_pObject->CycleGameMode(m_IsMoveUp);
		}
	private:
		ModeSelector* m_pObject;
		bool m_IsMoveUp{};
	};

	class StartGame final : public Command
	{
	public:
		StartGame(ModeSelector* const object, GameObject* menu) : m_pObject(object), m_pMenu{ menu } {}
		void Execute() override
		{
			m_pObject->StartGame(m_pMenu);
		}
	private:
		ModeSelector* m_pObject;
		GameObject* m_pMenu;
	};
#pragma endregion

#pragma region player
	class Shoot final : public Command
	{
	public:
		Shoot(ShootComponent* const object) : m_pObject(object) {}
		void Execute() override
		{
			m_pObject->Shoot();
		}
	private:
		ShootComponent* m_pObject;
	};
#pragma endregion

#pragma region player
	class ExecuteBeam final : public Command
	{
	public:
		ExecuteBeam(OpposerComponent* const object) : m_pObject(object) {}
		void Execute() override
		{
			m_pObject->ExecuteBeam();
		}
	private:
		OpposerComponent* m_pObject;
	};
#pragma endregion
	
#pragma region Skipping
	class Skip final : public Command
	{
	public:
		Skip(std::function<void(Scene*, Stages, float)> createStage, Scene* scene) : CreateStage(createStage), m_pScene{scene} {}
		void Execute() override
		{
			auto enemies{ m_pScene->GetGameObject(EnumStrings[EnemyHolder]) };
			auto capturedFighters{ m_pScene->GetGameObjects(EnumStrings[CapturedFighter], false) };
			auto bullets{ m_pScene->GetGameObjects(EnumStrings[BulletGeneral], false) };

			
			if (enemies)
				enemies->MarkForDestroy();

			for (auto fighter : capturedFighters)
			{
				fighter->MarkForDestroy();
			}

			for (auto bullet : bullets)
			{
				bullet->MarkForDestroy();
			}

			auto children{ m_pScene->GetGameObject(EnumStrings[ScoreBoard])->GetChildren(EnumStrings[Life]) };
			for (size_t i = 0; i < 3; i++)
			{
				children[i]->SetIsHidden(false);
			}

			if(m_pScene->GetGameObject("Stage 1")){
				CreateStage(m_pScene, Stages::Stage2, 0.f);
				m_pScene->GetGameObject("Stage 1")->SetName("Stage 2");
			}
			else if (m_pScene->GetGameObject("Stage 2")) {
				CreateStage(m_pScene, Stages::Stage3, 0.f);
				m_pScene->GetGameObject("Stage 2")->SetName("Stage 3");
			}
			else if (m_pScene->GetGameObject("Stage 3")) {
				CreateStage(m_pScene, Stages::Stage1, 0.f);
				m_pScene->GetGameObject("Stage 3")->SetName("Stage 1");
			}
		}
	private:
		std::function<void(Scene*, Stages, float)> CreateStage;
		Scene* m_pScene;
	};
#pragma endregion


};

