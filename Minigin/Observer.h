#pragma once
#include "GameObject.h"
#include "TextObjectComponent.h"
#include "Scene.h"
//#include "Steam.h"


namespace dae
{
	enum class Event
	{
		Live,
		Score,
		GameOver,
		Reset
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
		HealthObserver(const std::vector<std::shared_ptr<GameObject>> gameObjects, Scene* scene) : GameObjects{ gameObjects }, m_Scene{ scene } {};
		void Notify(GameObject* go, Event event) override;

	private:
		const std::vector<std::shared_ptr<GameObject>> GameObjects;
		Scene* m_Scene;
	};

}