#include "Callback.h"

dae::Callback::~Callback()
{
	for (auto observer : m_pObservers)
	{
		delete observer;
		observer = nullptr;
	}
}

void dae::Callback::AddObserver(Observer* const observer)
{
	m_pObservers.push_back(observer);
}

void dae::Callback::RemoveObserver(Observer* const observer)
{
	auto it = std::find(m_pObservers.begin(), m_pObservers.end(), observer);
	m_pObservers.erase(it);
}

void dae::Callback::Notify(GameObject* go, Event event)
{
	for (unsigned int i{}; i < m_pObservers.size(); i++)
	{
		m_pObservers.at(i)->Notify(go, event);
	}
}
