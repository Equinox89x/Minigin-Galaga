#include <SDL.h>
#include "InputManager.h"
#include "imgui_impl_sdl2.h"
#define WIN32_LEAN_AND_MEAN
#include "Timer.h"
#include <iostream>

void dae::InputManager::ProcessInput()
{
	for (int playerId = 0; playerId < MAX_PLAYERS; playerId++)
	{
		pImpl->ProcessInput(playerId);
	}
}

void dae::Input::ClearKeys()
{
	m_consoleCommands.clear();
	m_KeyboardCommands.clear();
	m_Clear = true;
}

void dae::Input::BindKey(ControllerKey key, std::unique_ptr<Command> c) { m_consoleCommands[key] = std::move(c); }
void dae::Input::BindKey(KeyboardKey key, std::unique_ptr<Command> c) { m_KeyboardCommands[key] = std::move(c); }
bool dae::InputManager::IsPressed(ControllerButton button, int id) const { return pImpl->IsPressed(button, id); }
bool dae::InputManager::IsDownThisFrame(ControllerButton button, int id) const{ return pImpl->IsDownThisFrame(button, id); }
bool dae::InputManager::IsUpThisFrame(ControllerButton button, int id) const { return pImpl->IsUpThisFrame(button, id); }

void dae::InputManager::HandleInput()
{
	//float deltaTime{ Timer::GetInstance().GetDeltaTime() };

	#pragma region Controller
	for (const auto& [controllerKey, action] : Input::GetInstance().GetConsoleCommands())
	{

		auto [state, button, id] = controllerKey;

		switch (state)
		{
		case ButtonStates::BUTTON_DOWN:
			if (IsDownThisFrame(button, id)) 
				action->Execute();
			break;

		case ButtonStates::BUTTON_PRESSED:
			if (IsPressed(button, id))
				action->Execute();
			break;

		case ButtonStates::BUTTON_UP:
			if (IsUpThisFrame(button, id))
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
			case ButtonStates::SCORE_UP:
				if (e.key.type == SDL_KEYUP)
				{
					if (button == e.key.keysym.sym) action->Execute();
				}
				break;
			case ButtonStates::LIVE_DOWN:
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