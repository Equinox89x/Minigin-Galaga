#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObjectComponent.h"
#include "FPSCounterComponent.h"
#include "TextureComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "FileReader.h"
#include "RotatorComponent.h"
#include "MoveKeyboardComponent.h"
#include "MoveControllerComponent.h"
#include "imgui.h"
#include "imgui_plot.h"
#include <chrono>
#include "InputManager.h"
#include "ValuesComponent.h"
#include <sstream>
#include "Main.h"
#include "ShootComponent.h"
#include "MoveMenuComponent.h"
#include "EnemyComponent.h"
#include "CommandProject.h"

using namespace dae;

void MakeMainGalaga(dae::Scene& scene) {
	//Main Player
	std::shared_ptr<GameObject> mainPlayer = std::make_shared<dae::GameObject>();
	scene.Add(mainPlayer);

	//Texture
	mainPlayer->AddComponent(new TextureComponent());
	mainPlayer->GetComponent<TextureComponent>()->SetTexture("galaga.png");
	mainPlayer->GetComponent<TextureComponent>()->Scale(1, 1);
	mainPlayer->GetComponent<TextureComponent>()->SetPosition(WindowSizeX / 2 - Margin, WindowSizeY - SubMargin * 2);

	//UI
	mainPlayer->AddComponent(new ValuesComponent());
	mainPlayer->GetComponent<ValuesComponent>()->SetLives(3);

	//bullets
	mainPlayer->AddComponent(new ShootComponent(scene));

	//Callback* callback = new Callback;
	//callback->AddObserver(new HealthObserver(mainPlayer->GetComponent<HealthComponent>()));
	//mainPlayer->GetComponent<ValuesComponent>()->SetCallback(callback);

	//Keyboard
	mainPlayer->AddComponent(new MoveKeyboardComponent(mainPlayer->GetTransform()->GetPosition()));

	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_SPACE, 0 }, std::make_unique<Shoot>(mainPlayer->GetComponent<ShootComponent>()));

	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_a, 0 }, std::make_unique<MoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>(), glm::vec3(-300.f, 0.0f, 0.0f)));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_d, 0 }, std::make_unique<MoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>(), glm::vec3(300.f, 0.0f, 0.0f)));

	Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_a, 0 }, std::make_unique<StopMoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>()));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_d, 0 }, std::make_unique<StopMoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>()));
}

void MakeSecondGalaga(dae::Scene& scene) {
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

	//second galaga
	std::shared_ptr<GameObject> go = std::make_shared<dae::GameObject>();
	go->SetName("Second");
	go->AddComponent(new TextureComponent());
	go->GetComponent<TextureComponent>()->SetTexture("galaga.png");

	//UI
	go->AddComponent(new ValuesComponent());
	go->GetComponent<ValuesComponent>()->SetLives(3);

	//callback
	//Callback* callback = new Callback;
	//callback->AddObserver(new HealthObserver(go2->GetComponent<TextObjectComponent>()));
	//callback->AddObserver(new ScoreObserver(go3->GetComponent<TextObjectComponent>()));
	//comp->SetCallback(callback);

	//keyboard
	go->AddComponent(new MoveControllerComponent(go->GetTransform()->GetPosition()));
	scene.Add(go);

	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadLeft, 1 }, std::make_unique<MoveController>(go->GetComponent<MoveControllerComponent>(), glm::vec3(-300, 0.0f, 0.0f)));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadRight, 1 }, std::make_unique<MoveController>(go->GetComponent<MoveControllerComponent>(), glm::vec3(300, 0.0f, 0.0f)));

	Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadLeft, 1 }, std::make_unique<StopMoveController>(go->GetComponent<MoveControllerComponent>()));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadRight, 1 }, std::make_unique<StopMoveController>(go->GetComponent<MoveControllerComponent>()));
}

void MakeStage(dae::Scene& scene) {
	std::shared_ptr<GameObject> container = std::make_shared<dae::GameObject>();

	FileReader* file{ new FileReader("../Data/galagamap.txt") };
	auto str{ file->ReadJsonFile() };
	auto data{ file->ParseJson(str, '+') };

	auto gridData{ std::any_cast<std::vector<std::vector<std::string>>>(data["Test"]) };
	float currentX{ 0.f };
	float currentY{ 60.f };
	for (auto strLine : gridData) {
		for (auto tile : strLine) {

			if (tile == "goei") {
				std::shared_ptr<GameObject> enemy = std::make_shared<dae::GameObject>();
				enemy->SetName("enemy");
				enemy->AddComponent(new EnemyComponent(EnemyType::GOEI, 50));
				enemy->AddComponent(new TextureComponent());
				enemy->GetComponent<TextureComponent>()->SetTexture("goei.png");
				enemy->GetComponent<TextureComponent>()->SetName("enemy");
				enemy->GetComponent<TextureComponent>()->Scale(3, 3);
				enemy->GetTransform()->Translate(currentX, currentY);
				enemy->GetComponent<TextureComponent>()->SetNrOfFrames(2);
				enemy->GetComponent<TextureComponent>()->GetRect();
				scene.Add(enemy);
			}
			else if (tile == "zako") {
				std::shared_ptr<GameObject> enemy = std::make_shared<dae::GameObject>();
				enemy->SetName("enemy");
				enemy->AddComponent(new EnemyComponent(EnemyType::ZAKO, 80));
				enemy->AddComponent(new TextureComponent());
				enemy->GetComponent<TextureComponent>()->SetTexture("zako.png");
				enemy->GetComponent<TextureComponent>()->SetName("enemy");
				enemy->GetComponent<TextureComponent>()->Scale(3, 3);
				enemy->GetTransform()->Translate(currentX, currentY);
				enemy->GetComponent<TextureComponent>()->SetNrOfFrames(2);
				enemy->GetComponent<TextureComponent>()->GetRect();
				scene.Add(enemy);
			}
			else if (tile == "boss") {
				std::shared_ptr<GameObject> enemy = std::make_shared<dae::GameObject>();
				enemy->SetName("enemy");
				enemy->AddComponent(new EnemyComponent(EnemyType::BOSS, 150, 400));
				enemy->AddComponent(new TextureComponent());
				enemy->GetComponent<TextureComponent>()->SetTexture("boss.png");
				enemy->GetComponent<TextureComponent>()->SetName("enemy");
				enemy->GetComponent<TextureComponent>()->Scale(3, 3);
				enemy->GetTransform()->Translate(currentX, currentY);
				enemy->GetComponent<TextureComponent>()->SetNrOfFrames(2);
				enemy->GetComponent<TextureComponent>()->GetRect();

				auto comp{ new TextureComponent() };
				enemy->AddComponent(comp);
				comp->SetName("Weapon");
				comp->SetTexture("beam.png");
				comp->Scale(3, 3);
				comp->SetNrOfFrames(3);
				comp->GetRect();
				comp->SetIsVisible(false);
				comp->SetOffset({ -76,50 });
				comp->GetRect();
				scene.Add(enemy);
			}
			currentX += Cellsize;
		}
		currentX = 0;
		currentY += Cellsize;
	}

	scene.Add(container);
}

void MakeVersusStage(dae::Scene& scene) {
	std::shared_ptr<GameObject> container = std::make_shared<dae::GameObject>();
	scene.Add(container);
}

void MakeMainMenu(dae::Scene& scene) {

	std::shared_ptr<GameObject> container = std::make_shared<dae::GameObject>();
	scene.Add(container);
	//container->AddComponent(new RotatorComponent());
	container->AddComponent(new MoveMenuComponent(1.f));
	container->GetTransform()->TranslateWorld(0, 400);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Emulogic-zrEw.ttf", 24);

	//score 1
	std::shared_ptr<GameObject> upText1 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> upText1Score = std::make_shared<dae::GameObject>();
	scene.Add(upText1);
	scene.Add(upText1Score);
	container->AddChild(upText1.get());
	container->AddChild(upText1Score.get());
	upText1->AddComponent(new TextObjectComponent("1UP", font));
	upText1Score->AddComponent(new TextObjectComponent("00", font));
	upText1->GetComponent<TextObjectComponent>()->SetPosition(Margin, Margin);
	upText1->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 220,20,60 });
	upText1Score->GetComponent<TextObjectComponent>()->SetPosition(Margin * 3, SubMargin);

	//score 2
	std::shared_ptr<GameObject> upText2 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> upText2Score = std::make_shared<dae::GameObject>();
	scene.Add(upText2);
	scene.Add(upText2Score);
	container->AddChild(upText2.get());
	container->AddChild(upText2Score.get());
	upText2->AddComponent(new TextObjectComponent("2UP", font));
	upText2->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 220,20,60 });
	upText2Score->AddComponent(new TextObjectComponent("00", font));
	upText2->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX - (SubMargin * 3), Margin);
	upText2Score->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX - SubMargin * 2, SubMargin);


	//highscore
	std::shared_ptr<GameObject> highScore = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> highScoreScore = std::make_shared<dae::GameObject>();
	scene.Add(highScore);
	scene.Add(highScoreScore);
	container->AddChild(highScore.get());
	container->AddChild(highScoreScore.get());
	highScore->AddComponent(new TextObjectComponent("HI-SCORE", font));
	highScore->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 220,20,60 });
	highScore->GetComponent<TextObjectComponent>()->SetName("Debug");
	highScoreScore->AddComponent(new TextObjectComponent("30000", font));
	highScore->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - Margin * 3, Margin);
	highScoreScore->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - SubMargin, SubMargin);


	//logo
	std::shared_ptr<GameObject> logo = std::make_shared<dae::GameObject>();
	scene.Add(logo);
	container->AddChild(logo.get());
	logo->AddComponent(new TextureComponent());
	logo->GetComponent<TextureComponent>()->SetTexture("galagaLogo.png");
	logo->GetComponent<TextureComponent>()->SetPosition(WindowSizeX / 2 - SubMargin * 1.75f, WindowSizeY / 2 - SubMargin * 2);
	logo->GetComponent<TextureComponent>()->Scale(0.6f, 0.6f);

	//game mode selection
	std::shared_ptr<GameObject> player1 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> player2 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> player3 = std::make_shared<dae::GameObject>();
	scene.Add(player1);
	scene.Add(player2);
	scene.Add(player3);
	container->AddChild(player1.get());
	container->AddChild(player2.get());
	container->AddChild(player3.get());
	player1->AddComponent(new TextObjectComponent("1 PLAYER", font));
	player2->AddComponent(new TextObjectComponent("2 PLAYERS", font));
	player3->AddComponent(new TextObjectComponent("VERSUS", font));
	player1->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - Margin * 3, WindowSizeY / 2);
	player2->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - Margin * 3, WindowSizeY / 2 + Margin);
	player3->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - Margin * 3, WindowSizeY / 2 + SubMargin);

	//game mode selector
	std::shared_ptr<GameObject> selector = std::make_shared<dae::GameObject>();
	scene.Add(selector);
	container->AddChild(selector.get());
	selector->AddComponent(new ModeSelector(scene, &MakeMainGalaga, &MakeSecondGalaga, &MakeStage, &MakeVersusStage));
	selector->AddComponent(new TextObjectComponent(">", font));
	selector->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - SubMargin * 2, WindowSizeY / 2);
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_UP, 0 }, std::make_unique<CycleGameMode>(selector->GetComponent<ModeSelector>(), true));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_DOWN, 0 }, std::make_unique<CycleGameMode>(selector->GetComponent<ModeSelector>(), false));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_SPACE, 0 }, std::make_unique<StartGame>(selector->GetComponent<ModeSelector>(), container.get()));

	//credits
	std::shared_ptr<GameObject> credit = std::make_shared<dae::GameObject>();
	scene.Add(credit);
	container->AddChild(credit.get());
	credit->AddComponent(new TextObjectComponent("TM AND C 1981 1988 NAMCO LTD.", font));
	credit->GetComponent<TextObjectComponent>()->SetPosition(Margin / 2, WindowSizeY - SubMargin * 2);

	//company credits
	std::shared_ptr<GameObject> company1 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> company2 = std::make_shared<dae::GameObject>();
	scene.Add(company1);
	scene.Add(company2);
	container->AddChild(company1.get());
	container->AddChild(company2.get());
	company1->AddComponent(new TextObjectComponent("BANDAI CO. LTD. IS LICENSED", font));
	company2->AddComponent(new TextObjectComponent("BY NINTENDO OF AMERICA INC.", font));
	company1->GetComponent<TextObjectComponent>()->SetPosition(Margin, WindowSizeY - SubMargin);
	company2->GetComponent<TextObjectComponent>()->SetPosition(Margin, WindowSizeY - Margin);
}

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	MakeMainMenu(scene);
}

int main(int, char* []) {
	//if (!SteamAPI_Init())
	//{
	//	std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
	//	return 1;
	//}
	//else {
	//	std::cout << "Successfully initialized steam." << std::endl;
	//}

	dae::Minigin engine("../Data/");
	engine.Run(load);

	//SteamAPI_Shutdown();
	return 0;
}