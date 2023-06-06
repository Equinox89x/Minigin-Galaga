#pragma once
#include "MoveKeyboardComponent.h"
#include "Minigin.h"
#include "MoveControllerComponent.h"
#include "ValuesComponent.h"
#include "../Galaga/ModeSelector.h"
#include "../Galaga/ShootComponent.h"


namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0; //you can add deltatime here
	};

	class QuitGame final : public Command
	{
	public:
		QuitGame(dae::Minigin* pEngine) : m_pObject(pEngine) {}

		void Execute() override 
		{
			m_pObject->StopRunning();
		}

	private:
		dae::Minigin* m_pObject;
	};

#pragma region Move Keyboard
	class MoveKeyboard final : public Command
	{
	public:
		MoveKeyboard(MoveKeyboardComponent* const object, const glm::vec3& moveSpeed) : m_pObject(object), m_MoveSpeed(moveSpeed) {}
		void Execute() override
		{
			m_pObject->SetMoveSpeed(m_MoveSpeed);
		}
	private:
		MoveKeyboardComponent* m_pObject;
		glm::vec3 m_MoveSpeed;
	};

	class StopMoveKeyboard final : public Command
	{
	public:
		StopMoveKeyboard(MoveKeyboardComponent* const object) : m_pObject(object) {};
		void Execute() override
		{
			m_pObject->SetMoveSpeed(glm::vec3(0, 0, 0));
		}
	private:
		MoveKeyboardComponent* m_pObject;
	};
#pragma endregion

#pragma region Move Controller
	class MoveController final : public Command
	{
	public:
		MoveController(MoveControllerComponent* const object, const glm::vec3& moveSpeed) : m_pObject(object), m_MoveSpeed(moveSpeed) {}
		void Execute() override
		{
			m_pObject->SetMoveSpeed(m_MoveSpeed);
		}
	private:
		MoveControllerComponent* m_pObject;
		glm::vec3 m_MoveSpeed;
	};

	class StopMoveController final : public Command
	{
	public:
		StopMoveController(MoveControllerComponent* const object) : m_pObject(object) {};
		void Execute() override
		{
			m_pObject->SetMoveSpeed(glm::vec3(0, 0, 0));
		}
	private:
		MoveControllerComponent* m_pObject;
	};
#pragma endregion

#pragma region values
	class IncreaseScore final : public Command
	{
	public:
		IncreaseScore(ValuesComponent* const object) : m_pObject(object){}
		void Execute() override
		{
			m_pObject->IncreaseScore(100);
		}
	private:
		ValuesComponent* m_pObject;
	};


	class DownLives final : public Command
	{
	public:
		DownLives(ValuesComponent* const object) : m_pObject(object) {}
		void Execute() override
		{
			m_pObject->Damage();
		}
	private:
		ValuesComponent* m_pObject;
	};
#pragma endregion
}

