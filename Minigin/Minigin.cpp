#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#define _CRT_SECURE_NO_WARNINGS 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Timer.h"
#include <chrono>
//#include <steam_api.h>
#include "Main.h"
#include "InputManager.h"
#include "Audio.h"
#include "ConsoleAudio.h"
#include "Locator.h"

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		static_cast<int>(WindowSizeX),
		static_cast<int>(WindowSizeY),
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);

	Timer::GetInstance().Init();
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	Locator::Quit();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	Audio* audioService = new ConsoleAudio();
	Locator::provide(audioService);
	std::thread audioThread(&Audio::Update, audioService);

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	auto input = InputManager(0);
	auto input2 = InputManager(1);
	auto input3 = InputManager(2);

	m_Running = true;
	bool isFirstTime{ true };


	Timer::GetInstance().Start();
	while (m_Running)
	{
		Timer::GetInstance().Update();

		input.HandleInput();
		input2.HandleInput();
		input3.HandleInput();

		if (isFirstTime) {
			sceneManager.Initialize();
			sceneManager.PostInitialize();
			isFirstTime = false;
		}
		sceneManager.Update();
		renderer.Render();
	}

	audioThread.join();

	Timer::GetInstance().Stop();
}

void dae::Minigin::StopRunning()
{
	m_Running = false;
	Locator::getAudio()->StopAllSounds();
}