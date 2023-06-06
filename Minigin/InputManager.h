#pragma once
#include "Singleton.h"
#include <tuple>
#include <map>
#include <memory>
#include "Command.h"
#include <Windows.h>
#include <Xinput.h>

#define MAX_PLAYERS 2

namespace dae
{

	enum class ControllerButton
	{
		DpadUp = 0x0001,
		DpadDown = 0x0002,
		DpadLeft = 0x0004,
		DpadRight = 0x0008,
		Start = 0x0010,
		Back = 0x0020,
		LeftThumb = 0x0040,
		RightThumb = 0x0080,
		LeftShoulder = 0x0100,
		RightShoulder = 0x0200,
		ButtonA = 0x1000,
		ButtonB = 0x2000,
		ButtonX = 0x4000,
		ButtonY = 0x8000,
	};
	
	//enum class ControllerButton
	//{
	//	/*DpadUp = 203,
	//	DpadDown = 204,
	//	DpadLeft = 205,
	//	DpadRight = 206,
	//	Start = 0x0010,
	//	Back = 0x0020,
	//	LeftThumb = 0x0040,
	//	RightThumb = 0x0080,
	//	LeftShoulder = 0x0100,
	//	RightShoulder = 0x0200,
	//	ButtonA = 0x1000,
	//	ButtonB = 0x2000,
	//	ButtonX = 0x4000,
	//	ButtonY = 0x8000,*/
	//};

	enum class ButtonStates
	{
		BUTTON_DOWN,
		BUTTON_PRESSED,
		BUTTON_UP,
		SCORE_UP,
		LIVE_DOWN,
	};

	class Input final : public Singleton<Input>
	{
	private:
		using ControllerKey = std::tuple<ButtonStates, ControllerButton, int>;
		using KeyboardKey = std::tuple<ButtonStates, SDL_KeyCode, int>;
		using ControllerCommandsMap = std::map<ControllerKey, std::unique_ptr<Command>>;
		using KeyboardCommandsMap = std::map<KeyboardKey, std::unique_ptr<Command>>;

		ControllerCommandsMap m_consoleCommands{};
		KeyboardCommandsMap m_KeyboardCommands{};
		bool m_Clear{};

	public:
		void ClearKeys();
		bool GetClear() const { return m_Clear; };
		void SetClear(bool clear) { m_Clear = clear; };
		void BindKey(ControllerKey key, std::unique_ptr<Command> c);
		void BindKey(KeyboardKey key, std::unique_ptr<Command> c);

		const ControllerCommandsMap& GetConsoleCommands()
		{
			return m_consoleCommands;
		};

		const KeyboardCommandsMap& GetKeyboardCommands()
		{
			return m_KeyboardCommands;
		};
	};

	class InputManagerImpl
	{
		XINPUT_STATE m_CurrentState[MAX_PLAYERS];
		XINPUT_STATE m_PreviousState[MAX_PLAYERS];
		unsigned int m_ButtonsPressedThisFrame[MAX_PLAYERS];
		unsigned int m_ButtonsReleasedThisFrame[MAX_PLAYERS];

	public:
		InputManagerImpl() = default;

		void ProcessInput(int playerId)
		{
			int controllerIndex{ playerId };
			CopyMemory(&m_PreviousState[playerId], &m_CurrentState[playerId], sizeof(XINPUT_STATE));
			ZeroMemory(&m_CurrentState[playerId], sizeof(XINPUT_STATE));
			XInputGetState(controllerIndex, &m_CurrentState[playerId]);


			auto buttonChanges = m_CurrentState[playerId].Gamepad.wButtons ^ m_PreviousState[playerId].Gamepad.wButtons;
			m_ButtonsPressedThisFrame[playerId] = buttonChanges & m_CurrentState[playerId].Gamepad.wButtons;
			m_ButtonsReleasedThisFrame[playerId] = buttonChanges & (~m_CurrentState[playerId].Gamepad.wButtons);
		}

		bool IsPressed(ControllerButton button, int playerId) const
		{
			return m_CurrentState[playerId].Gamepad.wButtons & int(button);

		}
		bool IsDownThisFrame(ControllerButton button, int playerId) const
		{
			return m_ButtonsPressedThisFrame[playerId] & int(button);

		}
		bool IsUpThisFrame(ControllerButton button, int playerId) const
		{
			return m_ButtonsReleasedThisFrame[playerId] & int(button);

		}
	};

	class InputManager
	{
		std::unique_ptr<InputManagerImpl> pImpl{};

	public:
		InputManager() { pImpl = std::make_unique<InputManagerImpl>(); };
		~InputManager() { pImpl.release(); };
		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;

		void ProcessInput();
		void HandleInput();

	private:
		bool IsPressed(ControllerButton button, int id) const;
		bool IsDownThisFrame(ControllerButton button, int id) const;
		bool IsUpThisFrame(ControllerButton button, int id) const;
	};
}