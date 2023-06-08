#pragma once
#include <string>
#include <functional>
#include <vector>

class InputManager;
namespace dae
{
	/*#define WindowSizeX 720
	#define WindowSizeY 720
	#define Margin 40.f
	#define SubMargin Margin * 2
	#define Cellsize 48.3f*/

	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

		void StopRunning();

	private:
		bool m_Running{ false };
		//std::vector<InputManager> m_InputManager{ };

	};
}