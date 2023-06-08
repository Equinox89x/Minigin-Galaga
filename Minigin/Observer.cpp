#include "ValuesComponent.h"
#include <string>
#include "Minigin.h"
#include "Observer.h"

void dae::HealthObserver::Notify(GameObject* go, Event event)
{
	auto i{ go->GetComponent<ValuesComponent>()->GetLives() };
	switch (event)
	{
	case Event::Live:
		if (i >= 0) {
			GameObjects[i]->SetIsHidden(true);
		}
		break;
	case Event::Reset:
		//m_pTextComponent->SetText(std::to_string(comp->GetLives()));
		break;
	}
}

void dae::ScoreObserver::Notify(GameObject* go, Event event)
{
	ValuesComponent* comp{ go->GetComponent<ValuesComponent>() };
	auto score{ comp->GetScores() };
	switch (event)
	{
	case Event::Score:
		m_pTextComponent->SetText(std::to_string(score));
		if (score == 500) {
			/*if (g_SteamAchievements)
				g_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");*/
		}
		break;
	case Event::Reset:
		m_pTextComponent->SetText(std::to_string(comp->GetScores()));
		break;

	}
}

void dae::GameOverObserver::Notify(GameObject* /*go*/, Event event)
{
	auto player0{ m_Scene->GetGameObject("Player0") };
	auto player1{ m_Scene->GetGameObject("Player1") };
	auto scoreboard{ m_Scene->GetGameObject("ScoreBoard") };
	auto enemies{ m_Scene->GetGameObjects("Enemy") };
	switch (event)
	{
	case Event::GameOver:	
		MakeEndScreen(m_Scene);

		if (player0)
			player0->MarkForDestroy();
		if(player1)
			player1->MarkForDestroy();
		//if(m_Scene->GetGameObject("Player1")) m_Scene->Remove(m_Scene->GetGameObject("Player1"));
		scoreboard->MarkForDestroy();
		for (auto& enemy : enemies) {
			enemy->MarkForDestroy();
		}

		break;
	case Event::Reset:
		break;

	}
}

void dae::StageCleared::Notify(GameObject* /*go*/, Event event)
{
	auto player0{ m_pScene->GetGameObject("Player0") };
	auto player1{ m_pScene->GetGameObject("Player1") };
	auto scoreboard{ m_pScene->GetGameObject("ScoreBoard") };
	auto children{ scoreboard->GetChildren("Life") };

	switch (event)
	{
	case Event::StageCleared:
		
		for (size_t i = 0; i < 3; i++)
		{
			children[i]->SetIsHidden(false);
		}

		if (m_pScene->GetGameObject("Stage 1")) {
			CreateStage(m_pScene, Stages::Stage2, 3);
			m_pScene->GetGameObject("Stage 1")->SetName("Stage 2");
			m_pScene->GetGameObject("Player0")->GetComponent<ValuesComponent>()->Reset();
		}
		else if (m_pScene->GetGameObject("Stage 2")) {
			CreateStage(m_pScene, Stages::Stage3, 3);
			m_pScene->GetGameObject("Stage 2")->SetName("Stage 3");
			m_pScene->GetGameObject("Player0")->GetComponent<ValuesComponent>()->Reset();
		}
		else if (m_pScene->GetGameObject("Stage 3")) {

			MakeEndScreen(m_pScene);

			if (player0)
				player0->MarkForDestroy();
			if (player1)
				player1->MarkForDestroy();
			//if(m_Scene->GetGameObject("Player1")) m_Scene->Remove(m_Scene->GetGameObject("Player1"));
			scoreboard->MarkForDestroy();
			m_pScene->GetGameObject("Stage 3")->SetName("Stage 1");
		}
		break;
	case Event::Reset:
		break;

	}
}
