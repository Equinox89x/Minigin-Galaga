#include "Timer.h"
#include "MoveControllerComponent.h"

void dae::MoveControllerComponent::UpdatePos(float dt)
{
	if (m_Movespeed.y > 0 || m_Movespeed.y < 0 || m_Movespeed.x > 0 || m_Movespeed.x < 0)
	{
		auto go{ GetGameObject() };

		glm::vec3 curPos = go->GetTransform()->GetPosition();
		glm::vec3 furPos = glm::vec3(curPos.x + (m_Movespeed.x * dt), curPos.y + (m_Movespeed.y * dt), 1);


		go->GetTransform()->Translate(furPos.x, furPos.y, 0);
	}
}

void dae::MoveControllerComponent::Update()
{

	auto dt{ Timer::GetInstance().GetDeltaTime() };

	UpdatePos(dt);
}

void dae::MoveControllerComponent::FixedUpdate()
{
}

void dae::MoveControllerComponent::Render() const
{
}

void dae::MoveControllerComponent::SetMoveSpeed(const glm::vec3& movespeed)
{
	m_Movespeed = movespeed;
}