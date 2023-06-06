#include "ValuesComponent.h"

dae::ValuesComponent::~ValuesComponent()
{
	delete m_pCallback;
	m_pCallback = nullptr;
}

void dae::ValuesComponent::Update()
{
}

void dae::ValuesComponent::FixedUpdate()
{
}

void dae::ValuesComponent::Render() const
{
}

void dae::ValuesComponent::Damage()
{
	m_Lives--;
	m_pCallback->Notify(GetGameObject(), Event::Live);
}

void dae::ValuesComponent::IncreaseScore(int score)
{
	m_Score += score;
	auto go{ GetGameObject() };
	m_pCallback->Notify(go, Event::Score);
}

void dae::ValuesComponent::SetCallback(Callback* const subject)
{
	m_pCallback = subject;
}

int dae::ValuesComponent::GetLives() const
{
	return m_Lives;
}

void dae::ValuesComponent::SetLives(int lives)
{
	m_Lives = lives;
}

int dae::ValuesComponent::GetScores() const
{
	return m_Score;
}

void dae::ValuesComponent::ResetObserver()
{
	m_pCallback->Notify(GetGameObject(), Event::Reset);
}

void dae::ValuesComponent::Reset()
{

	m_Lives = 3;
	m_Score = 0;
}
