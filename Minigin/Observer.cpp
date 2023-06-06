#include "ValuesComponent.h"
#include <string>
#include "Minigin.h"

void dae::HealthObserver::Notify(GameObject* go, Event event)
{
	ValuesComponent* comp{ go->GetComponent<ValuesComponent>() };
	switch (event)
	{
	case Event::Live:
		m_pTextComponent->SetText(std::to_string(comp->GetLives()));
		break;
	case Event::Reset:
		m_pTextComponent->SetText(std::to_string(comp->GetLives()));
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

