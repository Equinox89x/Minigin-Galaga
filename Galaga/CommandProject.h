#pragma once
#include "Command.h"
#include "ModeSelector.h"
#include "ShootComponent.h"

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
};

