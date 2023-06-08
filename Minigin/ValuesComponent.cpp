#include "ValuesComponent.h"
#include "Callback.h"

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
	if (m_Lives <= 0) {
		m_pCallback->Notify(GetGameObject(), Event::GameOver);
	}
}

void dae::ValuesComponent::IncreaseScore(int score)
{
	m_Score += score;
	auto go{ GetGameObject() };
	m_pCallback->Notify(go, Event::Score);	
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
	//NrOfHits = 0;
	//NrOfShotsFired = 0;
}

int dae::ValuesComponent::GetMissRatio()
{
	if (NrOfHits == 0 || NrOfShotsFired == 0) return 0;
	auto ratio{ (static_cast<float>(NrOfHits) / static_cast<float>(NrOfShotsFired)) * 100.f };
	return static_cast<int>(ratio);
}
