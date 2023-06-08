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
#include "BackgroundComponent.h"
#include "OverlapComponent.h"
#include "PlayerComponent.h"
#include "CommandProject.h"
#include "Callback.h"


using namespace dae;

void CreateEndScreen(dae::Scene* scene) {
	auto font = dae::ResourceManager::GetInstance().LoadFont("Emulogic-zrEw.ttf", 18);

	std::shared_ptr<GameObject> container = std::make_shared<dae::GameObject>();
	auto width{ WindowSizeX/2 - 120 };
	auto height{ WindowSizeY/2 - 100 };
	container->GetTransform()->Translate(width, height);
	container->SetName("EndScreen");

	GameObject* resultText{ new GameObject() };
	GameObject* shotsFiredText{ new GameObject() };
	GameObject* nrOfHitsText{ new GameObject() };
	GameObject* hitMissRatioText{ new GameObject() };
	container->AddChild(resultText);
	container->AddChild(shotsFiredText);
	container->AddChild(nrOfHitsText);
	container->AddChild(hitMissRatioText);

	auto comp{ scene->GetGameObject("Player0")->GetComponent<ValuesComponent>() };

	resultText->AddComponent(new TextObjectComponent("--RESULTS--", font));
	resultText->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 220,20,60 });

	shotsFiredText->AddComponent(new TextObjectComponent("SHOTS FIRED: " + std::to_string(comp->GetShotsFired()), font));
	shotsFiredText->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 249,254,14 });

	nrOfHitsText->AddComponent(new TextObjectComponent("NUMBER OF HITS: " + std::to_string(comp->GetNumberOfHits()), font));
	nrOfHitsText->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 249,254,14 });

	hitMissRatioText->AddComponent(new TextObjectComponent("HIT MISS RATIO: %" + std::to_string(comp->GetMissRatio()), font));

	resultText->GetTransform()->Translate(0, 30);
	shotsFiredText->GetTransform()->Translate(-40, 70);
	nrOfHitsText->GetTransform()->Translate(-40, 110);
	hitMissRatioText->GetTransform()->Translate(-40, 150);

	scene->Add(container);
}

void CreateScore(dae::Scene* scene) {
	auto font = dae::ResourceManager::GetInstance().LoadFont("Emulogic-zrEw.ttf", 18);
	std::shared_ptr<GameObject> scoreBoard = std::make_shared<dae::GameObject>();
	scoreBoard->SetName("ScoreBoard");
	scoreBoard->GetTransform()->Translate(WindowSizeX -140, 0);

	GameObject* goHighscoreText = new GameObject();
	GameObject* goHighscoreText2 = new GameObject();
	GameObject* goHighscoreValue = new GameObject();
	GameObject* goUpText = new GameObject();
	GameObject* goUpTextValue = new GameObject();

	scoreBoard->AddChild(goHighscoreText);
	scoreBoard->AddChild(goHighscoreText2);
	scoreBoard->AddChild(goHighscoreValue);
	scoreBoard->AddChild(goUpText);
	scoreBoard->AddChild(goUpTextValue);

	goHighscoreText->AddComponent(new TextObjectComponent("HIGH", font));
	goHighscoreText2->AddComponent(new TextObjectComponent("SCORE", font));
	goHighscoreText->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 220,20,60 });
	goHighscoreText2->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 220,20,60 });

	goHighscoreValue->AddComponent(new TextObjectComponent("30000", font));

	goUpText->AddComponent(new TextObjectComponent("1UP", font));
	goUpText->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 220,20,60 });

	goUpTextValue->AddComponent(new TextObjectComponent("00", font));
	goUpTextValue->SetName("Score");

	//auto width{ -140 };
	
	goHighscoreText->GetTransform()->Translate(0, 100);
	goHighscoreText2->GetTransform()->Translate(20, 120);
	goHighscoreValue->GetTransform()->Translate(20, 140);
	goUpText->GetTransform()->Translate(0, 200);
	goUpTextValue->GetTransform()->Translate(20, 220);

	auto width{ 0 };
	for (size_t i = 0; i < 3; i++)
	{
		GameObject* life{ new GameObject() };
		scoreBoard->AddChild(life);
		life->SetName("Life");

		life->AddComponent(new TextureComponent());
		life->GetComponent<TextureComponent>()->SetTexture("galaga.png");
		life->GetComponent<TextureComponent>()->Scale(0.6f, 0.6f);

		life->GetTransform()->Translate(width, WindowSizeY / 2);
		width += life->GetComponent<TextureComponent>()->GetRect().w + 10;
	}
	scene->Add(scoreBoard);

}


void MakeStageOfNr(dae::Scene* scene, Stages stageName, float delayTimer) {
	FileReader* file{ new FileReader("../Data/galagamap.txt") };
	auto str{ file->ReadJsonFile() };
	auto data{ file->ParseJson(str, '+') };

	std::string name;
	switch (stageName)
	{
	case Stages::Stage1:
		name = "Stage 1";
		break;
	case Stages::Stage2:
		name = "Stage 2";
		break;
	case Stages::Stage3:
		name = "Stage 3";
		break;
	default:
		break;
	}

	auto gridData{ std::any_cast<std::vector<std::vector<std::string>>>(data[name]) };
	float currentX{ 0.f };
	float currentY{ 48.3f };

	int bossIndex{ 1 };
	int goeiIndex{ 1 };
	int zakoIndex{ 1 };
	int index{ 1 };
	for (auto strLine : gridData) {
		for (auto tile : strLine) {

			if (tile == "goei") {
				std::shared_ptr<GameObject> enemy = std::make_shared<dae::GameObject>();
				enemy->SetName("Enemy");
				enemy->AddComponent(new EnemyComponent(scene, EnemyType::GOEI, index, delayTimer, { currentX, currentY }, 50));
				enemy->AddComponent(new TextureComponent());
				enemy->GetComponent<TextureComponent>()->SetTexture("goei.png");
				enemy->GetComponent<TextureComponent>()->SetName("Enemy");
				enemy->GetComponent<TextureComponent>()->Scale(3, 3);
				enemy->GetTransform()->Translate(-100, -100);
				enemy->GetComponent<TextureComponent>()->SetNrOfFrames(2);
				enemy->GetComponent<TextureComponent>()->GetRect();
				scene->Add(enemy);
				goeiIndex++;
				index++;
			}
			else if (tile == "zako") {
				std::shared_ptr<GameObject> enemy = std::make_shared<dae::GameObject>();
				enemy->SetName("Enemy");
				enemy->AddComponent(new EnemyComponent(scene, EnemyType::ZAKO, index, delayTimer, { currentX, currentY }, 80));
				enemy->AddComponent(new TextureComponent());
				enemy->GetComponent<TextureComponent>()->SetTexture("zako.png");
				enemy->GetComponent<TextureComponent>()->SetName("Enemy");
				enemy->GetComponent<TextureComponent>()->Scale(3, 3);
				enemy->GetTransform()->Translate(-100, -100);
				enemy->GetComponent<TextureComponent>()->SetNrOfFrames(2);
				enemy->GetComponent<TextureComponent>()->GetRect();
				scene->Add(enemy);
				zakoIndex++;
				index++;
			}
			else if (tile == "boss") {
				std::shared_ptr<GameObject> enemy = std::make_shared<dae::GameObject>();
				enemy->SetName("Enemy");
				enemy->AddComponent(new EnemyComponent(scene, EnemyType::BOSS, index, delayTimer, { currentX, currentY }, 150, 400));
				enemy->AddComponent(new TextureComponent());
				enemy->GetComponent<TextureComponent>()->SetTexture("boss.png");
				enemy->GetComponent<TextureComponent>()->SetName("Enemy");
				enemy->GetComponent<TextureComponent>()->Scale(3, 3);
				enemy->GetTransform()->Translate(-100, -100);
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
				scene->Add(enemy);
				bossIndex++;
				index++;
			}
			currentX += Cellsize;
		}
		currentX = 0;
		currentY += Cellsize;
	}

	auto go{ std::make_shared<GameObject>() };
	go->SetName(name);
	scene->Add(go);
}


void MakeGalaga(dae::Scene* scene, std::string textureName, int id) {
	auto playerName{ "Player" + std::to_string(id) };

	//Main Player
	std::shared_ptr<GameObject> mainPlayer = std::make_shared<dae::GameObject>();
	scene->Add(mainPlayer);
	mainPlayer->SetName(playerName);

	//Texture
	mainPlayer->AddComponent(new TextureComponent());
	mainPlayer->GetComponent<TextureComponent>()->SetTexture(textureName);
	mainPlayer->GetComponent<TextureComponent>()->Scale(1, 1);
	mainPlayer->GetComponent<TextureComponent>()->SetPosition(WindowSizeX / 2 - Margin, WindowSizeY - SubMargin * 2);

	//UI
	mainPlayer->AddComponent(new ValuesComponent(scene));
	mainPlayer->GetComponent<ValuesComponent>()->SetLives(3);

	//bullets
	mainPlayer->AddComponent(new ShootComponent(scene, id));
	mainPlayer->AddComponent(new PlayerComponent(scene));

	//callback
	Callback* callback = new Callback();
	auto scoregocontainer{ scene->GetGameObject("ScoreBoard") };
	auto scorego{ scoregocontainer->GetChild("Score") };
	auto lifes{ scoregocontainer->GetChildren("Life") };
	callback->AddObserver(new ScoreObserver(scorego->GetComponent<TextObjectComponent>()));
	callback->AddObserver(new HealthObserver(lifes, scene));
	callback->AddObserver(new GameOverObserver(&CreateEndScreen, scene));
	mainPlayer->GetComponent<ValuesComponent>()->SetCallback(callback);

	//callback
	Callback* callback2 = new Callback();
	callback2->AddObserver(new StageCleared(&MakeStageOfNr, &CreateEndScreen, scene));
	mainPlayer->GetComponent<ShootComponent>()->SetCallback(callback2);



	if (id == 0) {
		//Keyboard
		mainPlayer->AddComponent(new MoveKeyboardComponent(mainPlayer->GetTransform()->GetPosition()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_SPACE, id }, std::make_unique<Shoot>(mainPlayer->GetComponent<ShootComponent>()));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_a, id }, std::make_unique<MoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>(), glm::vec3(-300.f, 0.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_d, id }, std::make_unique<MoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>(), glm::vec3(300.f, 0.0f, 0.0f)));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_a, id }, std::make_unique<StopMoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_d, id }, std::make_unique<StopMoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>()));
	}
	
	//Controller
	mainPlayer->AddComponent(new MoveControllerComponent(mainPlayer->GetTransform()->GetPosition()));

	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, dae::ControllerButton::ButtonA, id }, std::make_unique<Shoot>(mainPlayer->GetComponent<ShootComponent>()));

	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadLeft, id }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(-300.f, 0.0f, 0.0f)));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadRight, id }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(300.f, 0.0f, 0.0f)));

	Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadLeft, id }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadRight, id }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
	
}

void MakeMainGalaga(dae::Scene* scene) {
	MakeGalaga(scene, "galaga.png", 0);
}

void MakeSecondGalaga(dae::Scene* scene) {
	MakeGalaga(scene, "galagaRed.png", 1);
}

void MakeStage(dae::Scene* scene) {
	MakeStageOfNr(scene, Stages::Stage1, 3);
}

void MakeVersusStage(dae::Scene* scene) {
	std::shared_ptr<GameObject> container = std::make_shared<dae::GameObject>();
	scene->Add(container);
}

void MakeMainMenu(dae::Scene* scene) {
	std::shared_ptr<GameObject> container2 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> container3 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> container4 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> container5 = std::make_shared<dae::GameObject>();
	scene->Add(container2);
	scene->Add(container3);
	scene->Add(container4);
	scene->Add(container5);

	auto comp1{ new TextureComponent() };
	comp1->SetName("bg_back1");
	container2->SetName("bg_back1");
	container2->AddComponent(comp1);
	container2->GetComponent<TextureComponent>("bg_back1")->SetTexture("bg_back.png");
	auto rect{ container2->GetComponent<TextureComponent>("bg_back1")->GetRect() };
	auto startPos{ glm::vec2{ 0, 0 - (rect.h + WindowSizeY) } };

	container2->AddComponent(new BackgroundComponent(150.f, startPos, { 0, 0 - (720 * 2) }));
	container2->GetComponent<BackgroundComponent>();

	comp1 = new TextureComponent();
	comp1->SetName("bg_back2");
	container3->SetName("bg_back2");
	container3->AddComponent(comp1);
	container3->GetComponent<TextureComponent>("bg_back2")->SetTexture("bg_back.png");
	container3->AddComponent(new BackgroundComponent(150.f, startPos, { 0, 0 - (720 * 5) }));
	container3->GetComponent<BackgroundComponent>();

	comp1 = new TextureComponent();
	comp1->SetName("bg_front1");
	container4->SetName("bg_front1");
	container4->AddComponent(comp1);
	container4->GetComponent<TextureComponent>("bg_front1")->SetTexture("bg_front.png");
	container4->AddComponent(new BackgroundComponent(300.f, startPos, { 0, 0 - (720 * 2) }));
	container4->GetComponent<BackgroundComponent>();

	comp1 = new TextureComponent();
	comp1->SetName("bg_front2");
	container5->SetName("bg_front2");
	container5->AddComponent(comp1);
	container5->GetComponent<TextureComponent>("bg_front2")->SetTexture("bg_front.png");
	container5->AddComponent(new BackgroundComponent(300.f, startPos, { 0, 0 - (720 * 5) }));
	container5->GetComponent<BackgroundComponent>();

	std::shared_ptr<GameObject> container = std::make_shared<dae::GameObject>();
	container->SetName("MainMenu");
	scene->Add(container);
	//container->AddComponent(new RotatorComponent());
	container->AddComponent(new MoveMenuComponent(400.f));
	container->GetTransform()->Translate(0, 720);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Emulogic-zrEw.ttf", 24);

	//score 1
	std::shared_ptr<GameObject> upText1 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> upText1Score = std::make_shared<dae::GameObject>();
	scene->Add(upText1);
	scene->Add(upText1Score);
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
	scene->Add(upText2);
	scene->Add(upText2Score);
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
	scene->Add(highScore);
	scene->Add(highScoreScore);
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
	scene->Add(logo);
	container->AddChild(logo.get());
	logo->AddComponent(new TextureComponent());
	logo->GetComponent<TextureComponent>()->SetTexture("galagaLogo.png");
	logo->GetComponent<TextureComponent>()->SetPosition(WindowSizeX / 2 - SubMargin * 1.75f, WindowSizeY / 2 - SubMargin * 2);
	logo->GetComponent<TextureComponent>()->Scale(0.6f, 0.6f);

	//game mode selection
	std::shared_ptr<GameObject> player1 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> player2 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> player3 = std::make_shared<dae::GameObject>();
	scene->Add(player1);
	scene->Add(player2);
	scene->Add(player3);
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
	scene->Add(selector);
	container->AddChild(selector.get());
	selector->AddComponent(new ModeSelector(scene, &MakeMainGalaga, &MakeSecondGalaga, &MakeStage, &MakeVersusStage, &CreateScore));
	selector->AddComponent(new TextObjectComponent(">", font));
	selector->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - SubMargin * 2, WindowSizeY / 2);
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_UP, 0 }, std::make_unique<CycleGameMode>(selector->GetComponent<ModeSelector>(), true));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_DOWN, 0 }, std::make_unique<CycleGameMode>(selector->GetComponent<ModeSelector>(), false));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_SPACE, 0 }, std::make_unique<StartGame>(selector->GetComponent<ModeSelector>(), container.get()));

	//credits
	std::shared_ptr<GameObject> credit = std::make_shared<dae::GameObject>();
	scene->Add(credit);
	container->AddChild(credit.get());
	credit->AddComponent(new TextObjectComponent("TM AND C 1981 1988 NAMCO LTD.", font));
	credit->GetComponent<TextObjectComponent>()->SetPosition(Margin / 2, WindowSizeY - SubMargin * 2);

	//company credits
	std::shared_ptr<GameObject> company1 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> company2 = std::make_shared<dae::GameObject>();
	scene->Add(company1);
	scene->Add(company2);
	container->AddChild(company1.get());
	container->AddChild(company2.get());
	company1->AddComponent(new TextObjectComponent("BANDAI CO. LTD. IS LICENSED", font));
	company2->AddComponent(new TextObjectComponent("BY NINTENDO OF AMERICA INC.", font));
	company1->GetComponent<TextObjectComponent>()->SetPosition(Margin, WindowSizeY - SubMargin);
	company2->GetComponent<TextObjectComponent>()->SetPosition(Margin, WindowSizeY - Margin);

	auto go{ new GameObject() };
	//go->AddComponent(new MoveKeyboardComponent(go->GetTransform()->GetPosition()));
	go->AddComponent(new MoveKeyboardComponent(go->GetTransform()->GetPosition()));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_F1, 0 }, std::make_unique<Skip>(&MakeStageOfNr, scene));
}

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Galaga");

	MakeMainMenu(&scene);
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