#pragma once
#include "GameObject.h"
#include "TextObjectComponent.h"
#include "Scene.h"
#include <functional>

namespace dae
{
	enum class Event
	{
		Live,
		Score,
		GameOver,
		StageCleared,
		Reset,
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(GameObject* go, Event event) = 0;
		//CSteamAchievements* g_SteamAchievements = nullptr;

	};

	class ScoreObserver final : public Observer
	{
	public:

		ScoreObserver(TextObjectComponent* const textComponent) : m_pTextComponent(textComponent) {
			//g_SteamAchievements = new CSteamAchievements(g_Achievements, 4);
		};
		void Notify(GameObject* go, Event event) override;

	private:

		TextObjectComponent* m_pTextComponent;

	};

	class HealthObserver final : public Observer
	{
	public:
		HealthObserver(const std::vector<GameObject*> gameObjects, Scene* scene) : GameObjects{ gameObjects }, m_Scene{ scene } {};
		void Notify(GameObject* go, Event event) override;

	private:
		const std::vector<GameObject*> GameObjects;
		Scene* m_Scene;
	};

	class GameOverObserver final : public Observer
	{
	public:
		GameOverObserver(std::function<void(Scene*)> createEndScreen, Scene* scene) : MakeEndScreen{ createEndScreen }, m_Scene{ scene } {};
		void Notify(GameObject* go, Event event) override;

	private:
		std::function<void(Scene* scene)> MakeEndScreen;
		Scene* m_Scene;
	};


	class StageCleared final : public Observer
	{
	public:
		StageCleared(std::function<void(Scene*, Stages, float)> createStage, std::function<void(Scene*)> makeEndScreen, Scene* scene) : CreateStage(createStage), MakeEndScreen{ makeEndScreen }, m_pScene { scene } {}
		void Notify(GameObject* go, Event event) override;

	private:
		std::function<void(Scene*, Stages, float)> CreateStage;
		std::function<void(Scene*)> MakeEndScreen;
		Scene* m_pScene;
	};

}