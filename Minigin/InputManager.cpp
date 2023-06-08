#include <SDL.h>
#include "InputManager.h"
#include "imgui_impl_sdl2.h"
#define WIN32_LEAN_AND_MEAN
#include "Timer.h"
#include <iostream>
#include <windows.h>
#include <Xinput.h>
#include <glm/glm.hpp>
#include <iostream>

using namespace dae;

class dae::InputManager::InputManagerImpl
{
	bool m_IsConnectedPreviousframe{ false };

	XINPUT_STATE previousState{};
	XINPUT_STATE currentState{};

	WORD buttonsPressedThisFrame{};
	WORD buttonsReleasedThisFrame{};

	int controllerIndex;

public:
	InputManagerImpl(int controllerIndex)
		:controllerIndex(controllerIndex)
	{
		ZeroMemory(&previousState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));
	}

	void Update()
	{
		CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));

		DWORD dwResult;
		dwResult = XInputGetState(controllerIndex, &currentState);

		bool connected{ dwResult == ERROR_SUCCESS };

		if (m_IsConnectedPreviousframe != connected)
		{
			if (connected) std::cout << "Controller " << controllerIndex << " connected" << std::endl;
			else std::cout << "Controller " << controllerIndex << " disconnected" << std::endl;
		}

		m_IsConnectedPreviousframe = connected;

		auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
		buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
		buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);
	}

	bool IsDownThisFrame(unsigned button) const
	{
		return buttonsPressedThisFrame & button;
	}

	bool IsUpThisFrame(unsigned button) const
	{
		return buttonsReleasedThisFrame & button;
	}

	bool IsPressed(unsigned button) const
	{
		return currentState.Gamepad.wButtons & button;
	}
};


dae::InputManager::InputManager(int controllerIndex)
{
	pImpl = new InputManagerImpl(controllerIndex);
}

dae::InputManager::~InputManager()
{
	delete pImpl;
	pImpl = nullptr;
}


void dae::Input::ClearKeys()
{
	m_consoleCommands.clear();
	m_KeyboardCommands.clear();
	m_Clear = true;
}

void dae::Input::BindKey(ControllerKey key, std::unique_ptr<Command> c) { m_consoleCommands[key] = std::move(c); }
void dae::Input::BindKey(KeyboardKey key, std::unique_ptr<Command> c) { m_KeyboardCommands[key] = std::move(c); }

bool dae::InputManager::IsPressed(ControllerButton button) const { return pImpl->IsPressed(static_cast<unsigned int>(button)); }
bool dae::InputManager::IsDownThisFrame(ControllerButton button) const{ return pImpl->IsDownThisFrame(static_cast<unsigned int>(button)); }
bool dae::InputManager::IsUpThisFrame(ControllerButton button) const { return pImpl->IsUpThisFrame(static_cast<unsigned int>(button)); }

void dae::InputManager::HandleInput()
{
	pImpl->Update();

#pragma region Controller
	for (const auto& [controllerKey, action] : Input::GetInstance().GetConsoleCommands())
	{

		auto [state, button, id] = controllerKey;

		switch (state)
		{
		case ButtonStates::BUTTON_DOWN:
			if (IsDownThisFrame(button))
				action->Execute();
			break;

		case ButtonStates::BUTTON_PRESSED:
			if (IsPressed(button))
				action->Execute();
			break;

		case ButtonStates::BUTTON_UP:
			if (IsUpThisFrame(button))
				action->Execute();
			break;
		}

		if (Input::GetInstance().GetClear() == true)
		{
			Input::GetInstance().SetClear(false);
			break;
		}

	}
#pragma endregion Controller

	#pragma region Keyboard
	SDL_Event e;
	while (SDL_PollEvent(&e)) {		
		for (const auto& [key, action] : Input::GetInstance().GetKeyboardCommands())
		{
			auto [state, button, id] = key;
			switch (state)
			{
			case ButtonStates::BUTTON_DOWN:
				if (e.key.type == SDL_KEYDOWN)
				{
					if (e.key.repeat == 0)
					{
						if (button == e.key.keysym.sym) action->Execute();
					}
				}
				break;
			case ButtonStates::BUTTON_PRESSED:
				if (e.key.type == SDL_KEYDOWN)
				{
					if (button == e.key.keysym.sym) action->Execute();
				}
				break;
			case ButtonStates::BUTTON_UP:
				if (e.key.type == SDL_KEYUP)
				{
					if (button == e.key.keysym.sym) action->Execute();
				}
				break;
			}

			if (Input::GetInstance().GetClear() == true)
			{
				Input::GetInstance().SetClear(false);
				break;
			}
		}
	}
	#pragma endregion Keyboard

}
