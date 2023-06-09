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
#include "EndScreenComponent.h"
#include "EnemyManager.h"
#include "Callback.h"
#include "AudioComponent.h"
#include "../3rdParty/SDL2_mixer-2.0.4/include/SDL_mixer.h"
#include <random>


using namespace dae;

void CreateSelectorInput(dae::Scene* scene) {
	auto selector{ scene->GetGameObject(EnumStrings[Selector]) };
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_UP, 0 }, std::make_unique<CycleGameMode>(selector->GetComponent<ModeSelector>(), true));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_DOWN, 0 }, std::make_unique<CycleGameMode>(selector->GetComponent<ModeSelector>(), false));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_SPACE, 0 }, std::make_unique<StartGame>(selector->GetComponent<ModeSelector>(), selector->GetParent()));
}

void CreateEndScreen(dae::Scene* scene) {
	auto font = dae::ResourceManager::GetInstance().LoadFont("Emulogic-zrEw.ttf", 18);

	std::shared_ptr<GameObject> container = std::make_shared<dae::GameObject>();
	auto width{ WindowSizeX/2 - 120.f };
	auto height{ WindowSizeY/2 - 100.f };
	container->GetTransform()->Translate(width, height);
	container->SetName(EnumStrings[EndScreen]);
	container->AddComponent(new EndScreenComponent());

	//callback
	Callback* callback = new Callback();
	callback->AddObserver(new ToMenu(scene->GetGameObject(EnumStrings[MainMenu]).get(), &CreateSelectorInput, scene));
	container->GetComponent<EndScreenComponent>()->SetCallback(callback);

	GameObject* resultText{ new GameObject() };
	GameObject* shotsFiredText{ new GameObject() };
	GameObject* nrOfHitsText{ new GameObject() };
	GameObject* hitMissRatioText{ new GameObject() };
	container->AddChild(resultText);
	container->AddChild(shotsFiredText);
	container->AddChild(nrOfHitsText);
	container->AddChild(hitMissRatioText);

	//auto comp{ scene->GetGameObject("Player0")->GetComponent<ValuesComponent>() };
	auto comp{ scene->GetGameObject(EnumStrings[Values])->GetComponent<ValuesComponent>() };

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
	scoreBoard->SetName(EnumStrings[ScoreBoard]);
	scoreBoard->GetTransform()->Translate(GameWindowSizeX, 0);

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

	FileReader* file{ new FileReader("../Data/highscore.txt") };
	auto str{ file->ReadGameDataFile() };
	auto data{ file->ParseDataSimple(str, '+') };
	auto score{ data["Highscore"] };
	std::string scoreStr{ std::any_cast<std::string>(score) };
	goHighscoreValue->AddComponent(new TextObjectComponent(scoreStr, font));

	goUpText->AddComponent(new TextObjectComponent("1UP", font));
	goUpText->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 220,20,60 });

	goUpTextValue->AddComponent(new TextObjectComponent("00", font));
	goUpTextValue->SetName(EnumStrings[Score]);

	//auto width{ -140 };
	
	goHighscoreText->GetTransform()->Translate(0, 100);
	goHighscoreText2->GetTransform()->Translate(20, 120);
	goHighscoreValue->GetTransform()->Translate(20, 140);
	goUpText->GetTransform()->Translate(0, 200);
	goUpTextValue->GetTransform()->Translate(20, 220);

	float width{ GameWindowSizeX };
	for (size_t i = 0; i < 3; i++)
	{
		GameObject* life{ new GameObject() };
		scoreBoard->AddChild(life);
		life->SetName(EnumStrings[Life]);

		life->AddComponent(new TextureComponent());
		life->GetComponent<TextureComponent>()->SetTexture("galaga.png");
		life->GetComponent<TextureComponent>()->Scale(0.6f, 0.6f);

		life->GetTransform()->Translate(width, WindowSizeY / 2.f);
		width += life->GetComponent<TextureComponent>()->GetRect().w + 10;
	}
	scene->Add(scoreBoard);

}

void MakeValues(dae::Scene* scene) {
	if (scene->GetGameObject(EnumStrings[Values]) == nullptr) {
		//UI
		auto go = std::make_shared<GameObject>();
		go->SetName(EnumStrings[Values]);
		go->AddComponent(new ValuesComponent(scene));
		go->GetComponent<ValuesComponent>()->SetLives(3);

		//callback
		Callback* callback = new Callback();
		if (auto scoregocontainer{ scene->GetGameObject(EnumStrings[ScoreBoard]) }) {
			auto scorego{ scoregocontainer->GetChild(EnumStrings[Score]) };
			auto lifes{ scoregocontainer->GetChildren(EnumStrings[Life]) };
			callback->AddObserver(new ScoreObserver(scorego->GetComponent<TextObjectComponent>()));
			callback->AddObserver(new HealthObserver(lifes, scene));
		}
		callback->AddObserver(new GameOverObserver(&CreateEndScreen, scene));
		go->GetComponent<ValuesComponent>()->SetCallback(callback);
		scene->Add(go);
	}
}

void MakeStageOfNr(dae::Scene* scene, Stages stageName, float delayTimer) {
	std::shared_ptr<GameObject> enemyHolder = std::make_shared<dae::GameObject>();
	enemyHolder->SetName(EnumStrings[EnemyHolder]);

	FileReader* file{ new FileReader("../Data/galagamap.txt") };
	auto str{ file->ReadGameDataFile() };
	auto data{ file->ParseData(str, '+') };
	
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

	std::string name2{ name + " Enemies" };
	int nrOfEnemies{ std::stoi(std::any_cast<std::string>(data[name2])) };
	int enemiesPerGroup{ nrOfEnemies / 6 };

	auto gridData{ std::any_cast<std::vector<std::vector<std::string>>>(data[name]) };
	float currentX{ 0.f };
	float currentY{ 48.3f };

	float groupDelay{ 0.6f };
	auto delay{ ((0.1f * nrOfEnemies) * (groupDelay * 5)) + delayTimer };
	//auto delay{ (0.1f * nrOfEnemies) + 2.f };
	//delay += groupDelay * 6;

	enemyHolder->AddComponent(new EnemyManager(scene, delay));
	//callback
	Callback* callback2 = new Callback();
	callback2->AddObserver(new StageCleared(&MakeStageOfNr, &CreateEndScreen, scene));
	enemyHolder->GetComponent<EnemyManager>()->SetCallback(callback2);

	//enemy index per type
	int bossIndex{ 1 };
	int goeiIndex{ 1 };
	int zakoIndex{ 2 };  //2 so the int can never be 0 or 1 because 0 or 1 is the player or secondary player

	//general enemy index
	int index{ 1 };

	//index nr of enemies per group
	int groupIndex{ 1 };

	//index for chosen spawn path to take
	int path{ 0 };
	for (auto strLine : gridData) {
		for (auto tile : strLine) {

			if (tile == "goei") {
				//std::shared_ptr<GameObject> enemy = std::make_shared<dae::GameObject>();
				GameObject* enemy = new GameObject();
				enemy->SetName(EnumStrings[Enemy]);
				enemy->AddComponent(new EnemyComponent(scene, EnemyType::GOEI, index, delayTimer, path, { currentX, currentY }, 50));
				enemy->GetComponent<EnemyComponent>()->SetLives(1);
				enemy->AddComponent(new TextureComponent());
				enemy->GetComponent<TextureComponent>()->SetTexture("goei.png");
				enemy->GetComponent<TextureComponent>()->SetName(EnumStrings[Enemy]);
				enemy->GetComponent<TextureComponent>()->Scale(3, 3);
				enemy->GetTransform()->Translate(-100, -100);
				enemy->GetComponent<TextureComponent>()->SetNrOfFrames(2);
				enemy->GetComponent<TextureComponent>()->GetRect();
				//scene->Add(enemy);
				enemyHolder->AddChild(enemy);
				goeiIndex++;
				index++;
				groupIndex++;
			}
			else if (tile == "zako") {
				//std::shared_ptr<GameObject> enemy = std::make_shared<dae::GameObject>();
				GameObject* enemy = new GameObject();
				enemy->SetName(EnumStrings[Enemy]);
				enemy->AddComponent(new EnemyComponent(scene, EnemyType::ZAKO, index, delayTimer, path, { currentX, currentY }, 80));
				enemy->AddComponent(new ShootComponent(scene, zakoIndex, false, false, true));
				enemy->GetComponent<EnemyComponent>()->SetLives(1);
				enemy->AddComponent(new TextureComponent());
				enemy->GetComponent<TextureComponent>()->SetTexture("zako.png");
				enemy->GetComponent<TextureComponent>()->SetName(EnumStrings[Enemy]);
				enemy->GetComponent<TextureComponent>()->Scale(3, 3);
				enemy->GetTransform()->Translate(-100, -100);
				enemy->GetComponent<TextureComponent>()->SetNrOfFrames(2);
				enemy->GetComponent<TextureComponent>()->GetRect();
				enemyHolder->AddChild(enemy);
				//scene->Add(enemy);
				zakoIndex++;
				index++;
				groupIndex++;
			}
			else if (tile == "boss") {
				//std::shared_ptr<GameObject> enemy = std::make_shared<dae::GameObject>();
				GameObject* enemy = new GameObject();
				enemy->SetName(EnumStrings[Enemy]);
				enemy->AddComponent(new EnemyComponent(scene, EnemyType::BOSS, index, delayTimer, path, { currentX, currentY }, 150, 400));
				enemy->GetComponent<EnemyComponent>()->SetLives(2);
				enemy->AddComponent(new TextureComponent());

				enemy->GetComponent<TextureComponent>()->SetTexture("boss2.png");
				enemy->GetComponent<TextureComponent>()->SetName(EnumStrings[Enemy]);
				enemy->GetComponent<TextureComponent>()->Scale(3, 3);
				enemy->GetTransform()->Translate(-100, -100);
				enemy->GetComponent<TextureComponent>()->SetNrOfFrames(2);
				enemy->GetComponent<TextureComponent>()->GetRect();

				auto comp{ new TextureComponent() };
				enemy->AddComponent(comp);
				comp->SetName(EnumStrings[Weapon]);
				comp->SetTexture("beam.png");
				comp->Scale(3, 3);
				comp->SetNrOfFrames(3);
				comp->GetRect();
				comp->SetIsVisible(false);
				comp->SetOffset({ -76,50 });
				comp->GetRect();
				//scene->Add(enemy);
				enemyHolder->AddChild(enemy);
				bossIndex++;
				index++;
				groupIndex++;
			}
			currentX += Cellsize;
			if (groupIndex == enemiesPerGroup) {
				delayTimer += groupDelay;
				groupIndex = 0;
				path++;
				if (path > 3) {
					path = 0;
				}
			}
		}
		currentX = 0;
		currentY += Cellsize;
	}

	auto go{ std::make_shared<GameObject>() };
	go->SetName(name);
	scene->Add(go);
	scene->Add(enemyHolder);

	//MakeValues(scene);
}

//TODO
//Player still needs to die on overlap from tractor beam on both versus and single/coop mode

void MakeVersusGalaga(dae::Scene* scene) {
	std::shared_ptr<GameObject> enemyHolder = std::make_shared<dae::GameObject>();
	enemyHolder->SetName(EnumStrings[EnemyHolder]);
	enemyHolder->AddComponent(new EnemyManager(scene, 0, true));
	scene->Add(enemyHolder);

	std::shared_ptr<GameObject> opposer = std::make_shared<GameObject>();
	opposer->SetName(EnumStrings[Opposer]);
	opposer->AddComponent(new TextureComponent());
	opposer->AddComponent(new OpposerComponent(scene, 2));
	opposer->AddComponent(new ShootComponent(scene, 1, true, true, false));

	Callback* callback = new Callback();
	callback->AddObserver(new GameOverObserver(&CreateEndScreen, scene));
	opposer->GetComponent<OpposerComponent>()->SetCallback(callback);

	opposer->GetComponent<TextureComponent>()->SetTexture("boss2.png");
	opposer->GetComponent<TextureComponent>()->SetName(EnumStrings[Enemy]);
	opposer->GetComponent<TextureComponent>(EnumStrings[Enemy])->Scale(3, 3);
	//opposer->GetTransform()->Translate(WindowSizeX/2, 60);
	opposer->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetNrOfFrames(2);
	opposer->GetComponent<TextureComponent>(EnumStrings[Enemy])->GetRect();
	opposer->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetPosition((GameWindowSizeX) / 2 - Margin, Margin*2);

	auto comp{ new TextureComponent() };
	opposer->AddComponent(comp);
	comp->SetName(EnumStrings[Weapon]);
	comp->SetTexture("beam.png");
	comp->Scale(3, 3);
	comp->SetNrOfFrames(3);
	comp->GetRect();
	comp->SetIsVisible(false);
	comp->SetOffset({ -76,50 });
	comp->GetRect();
	scene->Add(opposer);

	opposer->AddComponent(new MoveControllerComponent(opposer->GetTransform()->GetPosition()));

	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, dae::ControllerButton::ButtonA, 0 }, std::make_unique<Shoot>(opposer->GetComponent<ShootComponent>()));

	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, dae::ControllerButton::ButtonX, 0 }, std::make_unique<ExecuteBeam>(opposer->GetComponent<OpposerComponent>()));
}

void MakeGalaga(dae::Scene* scene, std::string textureName, int id, bool isVersus) {
	auto playerName{ EnumStrings[PlayerGeneral] + std::to_string(id) };

	//Main Player
	std::shared_ptr<GameObject> mainPlayer = std::make_shared<dae::GameObject>();
	scene->Add(mainPlayer);
	mainPlayer->SetName(playerName);

	//Texture
	mainPlayer->AddComponent(new TextureComponent());
	mainPlayer->GetComponent<TextureComponent>()->SetName(playerName);
	mainPlayer->GetComponent<TextureComponent>()->SetTexture(textureName);
	mainPlayer->GetComponent<TextureComponent>()->Scale(0.7f, 0.7f);
	mainPlayer->GetComponent<TextureComponent>()->SetPosition((GameWindowSizeX) / 2 - Margin, WindowSizeY - Margin*3);

	//bullets
	mainPlayer->AddComponent(new ShootComponent(scene, id, isVersus));
	mainPlayer->AddComponent(new PlayerComponent(scene));

	if (id == 0) {
		//Keyboard
		mainPlayer->AddComponent(new MoveKeyboardComponent(mainPlayer->GetTransform()->GetPosition()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_SPACE, id }, std::make_unique<Shoot>(mainPlayer->GetComponent<ShootComponent>()));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_a, id }, std::make_unique<MoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>(), glm::vec3(-600.f, 0.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_d, id }, std::make_unique<MoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>(), glm::vec3(600.f, 0.0f, 0.0f)));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_a, id }, std::make_unique<StopMoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_d, id }, std::make_unique<StopMoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>()));

		////Controller
		//mainPlayer->AddComponent(new MoveControllerComponent(mainPlayer->GetTransform()->GetPosition()));

		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, dae::ControllerButton::ButtonA, 0 }, std::make_unique<Shoot>(mainPlayer->GetComponent<ShootComponent>()));

		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadLeft, 0 }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(-600.f, 0.0f, 0.0f)));
		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadRight, 0 }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(600.f, 0.0f, 0.0f)));

		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadLeft, 0 }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadRight, 0 }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
	}
	else {
		//Controller
		mainPlayer->AddComponent(new MoveControllerComponent(mainPlayer->GetTransform()->GetPosition()));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, dae::ControllerButton::ButtonA, id }, std::make_unique<Shoot>(mainPlayer->GetComponent<ShootComponent>()));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadLeft, id }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(-600.f, 0.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadRight, id }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(600.f, 0.0f, 0.0f)));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadLeft, id }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadRight, id }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
	}
	
	MakeValues(scene);
}

void MakeMainGalaga(dae::Scene* scene, bool isVersus) {
	MakeGalaga(scene, "galaga.png", 0, isVersus);
}

void MakeSecondGalaga(dae::Scene* scene) {
	MakeGalaga(scene, "galagaRed.png", 1, false);
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
	comp1->SetName(EnumStrings[Bg_back1]);
	container2->SetName(EnumStrings[Bg_back1]);
	container2->AddComponent(comp1);
	container2->GetComponent<TextureComponent>(EnumStrings[Bg_back1])->SetTexture("bg_back.png");
	auto rect{ container2->GetComponent<TextureComponent>(EnumStrings[Bg_back1])->GetRect() };
	auto startPos{ glm::vec2{ 0, 0 - (rect.h + WindowSizeY) } };

	container2->AddComponent(new BackgroundComponent(150.f, startPos, { 0, 0 - (720 * 2) }));
	container2->GetComponent<BackgroundComponent>();

	comp1 = new TextureComponent();
	comp1->SetName(EnumStrings[Bg_back2]);
	container3->SetName(EnumStrings[Bg_back2]);
	container3->AddComponent(comp1);
	container3->GetComponent<TextureComponent>(EnumStrings[Bg_back2])->SetTexture("bg_back.png");
	container3->AddComponent(new BackgroundComponent(150.f, startPos, { 0, 0 - (720 * 5) }));
	container3->GetComponent<BackgroundComponent>();

	comp1 = new TextureComponent();
	comp1->SetName(EnumStrings[Bg_front1]);
	container4->SetName(EnumStrings[Bg_front1]);
	container4->AddComponent(comp1);
	container4->GetComponent<TextureComponent>(EnumStrings[Bg_front1])->SetTexture("bg_front.png");
	container4->AddComponent(new BackgroundComponent(300.f, startPos, { 0, 0 - (720 * 2) }));
	container4->GetComponent<BackgroundComponent>();

	comp1 = new TextureComponent();
	comp1->SetName(EnumStrings[Bg_front2]);
	container5->SetName(EnumStrings[Bg_front2]);
	container5->AddComponent(comp1);
	container5->GetComponent<TextureComponent>(EnumStrings[Bg_front2])->SetTexture("bg_front.png");
	container5->AddComponent(new BackgroundComponent(300.f, startPos, { 0, 0 - (720 * 5) }));
	container5->GetComponent<BackgroundComponent>();

	std::shared_ptr<GameObject> container = std::make_shared<dae::GameObject>();
	container->SetName(EnumStrings[MainMenu]);
	scene->Add(container);
	container->AddComponent(new MoveMenuComponent(400.f, scene));
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

	FileReader* file{ new FileReader("../Data/highscore.txt") };
	auto str{ file->ReadGameDataFile() };
	auto data{ file->ParseDataSimple(str, '+') };
	auto score{ data["Highscore"] };
	std::string scoreStr{ std::any_cast<std::string>(score) };
	highScoreScore->AddComponent(new TextObjectComponent(scoreStr, font));
	highScore->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - Margin * 3, Margin);
	highScoreScore->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - SubMargin, SubMargin);

	//logo
	std::shared_ptr<GameObject> logo = std::make_shared<dae::GameObject>();
	logo->SetName(EnumStrings[Logo]);
	scene->Add(logo);
	container->AddChild(logo.get());
	logo->AddComponent(new TextureComponent());
	logo->GetComponent<TextureComponent>()->SetTexture("galagaLogo.png");
	logo->GetComponent<TextureComponent>()->SetName(EnumStrings[Logo]);
	logo->GetComponent<TextureComponent>()->SetPosition(WindowSizeX / 2 - SubMargin * 1.75f, WindowSizeY / 2 - SubMargin * 2);
	logo->GetComponent<TextureComponent>()->Scale(0.6f, 0.6f);
	logo->GetTransform()->AddTranslate(0.f, WindowSizeY);

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
	selector->SetName(EnumStrings[Selector]);
	scene->Add(selector);
	container->AddChild(selector.get());
	selector->AddComponent(new ModeSelector(scene, &MakeMainGalaga, &MakeSecondGalaga, &MakeStage, &MakeVersusGalaga, &CreateScore));
	selector->AddComponent(new TextObjectComponent(">", font));
	selector->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - SubMargin * 2, WindowSizeY / 2);
	CreateSelectorInput(scene);

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

	auto go{ std::make_shared<GameObject>() };
	go->SetName(EnumStrings[Global]);
	go->AddComponent(new AudioComponent());
	//go->AddComponent(new MoveKeyboardComponent(go->GetTransform()->GetPosition()));
	go->AddComponent(new MoveKeyboardComponent(go->GetTransform()->GetPosition()));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_F1, 0 }, std::make_unique<Skip>(&MakeStageOfNr, scene));
	scene->Add(go);
}

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Galaga");

	MakeMainMenu(&scene);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}