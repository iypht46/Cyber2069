//System Headers
#include <stdlib.h>
#include <time.h>

#include "World.hpp"

#include "Graphic/GraphicCore.hpp"
#include "Graphic/Camera.hpp"
#include "Core/GameInfo.h"
#include "Core/Logger.hpp"
#include "Input/Input.hpp"
#include "SceneManagement/SceneManager.h"

#include "Physic/PhysicScene.hpp"
//#include "Physic/Collision.hpp"

//#include "Core/Factory.h"
#include "Core/FactoryCollection.h"
#include "Core/EC/GameObject.hpp"

#include "Utility/ObjectPool.h"

//component headers might not necessary anymore
#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Animator.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"
#include "Core/EC/Components/SoundPlayer.hpp"
#include "Core/EC/UIComponents/TextRenderer.hpp"
#include "Core/EC/UIComponents/Button.hpp"
#include "Core/Particle/ParticleSystem.h"

#include "Enemy.hpp"
#include "EnemyBehaviours.h"
#include "PlayerController.hpp"
#include "EnemySpawner.hpp"
#include "GameController.hpp"
#include "Weapon.hpp"
#include "EquipmentManager.hpp"
#include "LoadoutUI.hpp"
#include "HighScoreUI.hpp"
#include "ItemDrop.hpp"
#include "Scripts/GameControl/UIController.h"

using SceneManagement::Instantiate;

namespace World
{
	//Constants
	constexpr float c_maxFrameRate = 60.0f;
	constexpr float c_targetDT = 1.0f / c_maxFrameRate;
	bool g_isDebug;

	//Game Info Var
	static GameInfo* g_gameInfo;
	static Graphic::CameraObject* cam = Graphic::getCamera();

	std::unique_ptr<GameObject> mouseCursor;

	//======================================
	//TESTING ONLY, DON'T FORGET TO REMOVE v
	//======================================
	ObjectPool* BulletPool;

	GameObject* title;
	std::shared_ptr<GameObject> Rabbit;
	GameObject* Bg1;
	GameObject* Bg2;
	GameObject* platform;
	GameObject* queen;

	GameObject* gamecontroller;

	//gameplay 
	std::shared_ptr<GameObject> ui_ScoreText;
	std::shared_ptr<GameObject> ui_ComboText;
	std::shared_ptr<GameObject> ui_HPbar;
	std::shared_ptr<GameObject> ui_HPtext;
	std::shared_ptr<GameObject> ui_StaminaBar;
	std::shared_ptr<GameObject> ui_BossHP;
	std::shared_ptr<GameObject> ui_BossHPtext;
	std::shared_ptr<GameObject> ui_Weapongp;
	std::shared_ptr<GameObject> ui_Artifactgp1;
	std::shared_ptr<GameObject> ui_Artifactgp2;

	GameObject* Spawner;

	std::shared_ptr<AnimationController> EnemCon;
	std::shared_ptr<AnimationController> RabbitController;
	std::shared_ptr<AnimationController> queenAnimControl;


	//GameObject** test;

	Physic::PhysicScene* g_physicScene;

	bool running;
	int Delay = 0;
	int Delay2 = 0;
	int platformNum = 5;
	int rand_AD;
	int rand_WS;

	float parlx1 = 0.3f;
	float parlx2 = 0.5f;

#define MOVE_SPEED 300.0f
#define MOVE_SPEED_ENEM 100.0f
#define CHAR_SIZE 120.0f
#define ENEMY_SIZE 40.0f

	//====================================

	//Physic Scene
	//static PhysicScene* g_physicScene;

	void DebugInput(float dt)
	{

		if (Input::GetKeyHold(Input::KeyCode::KEY_I))
		{
			cam->Zoom(1.0f * dt);
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_O))
		{
			cam->Zoom(-1.0f * dt);
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_Q))
		{
			Rabbit->m_transform->Rotate(1.0f);
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_E))
		{
			Rabbit->m_transform->Rotate(-1.0f);
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_Y))
		{
			Bg1->m_transform->SetPosition(Bg1->m_transform->GetPosition() + glm::vec3(0, 0, 5));
			ENGINE_INFO("Increased 1Zpos to {}", Bg1->m_transform->GetPosition().z);
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_U))
		{
			Bg1->m_transform->SetPosition(Bg1->m_transform->GetPosition() + glm::vec3(0, 0, -5));
			ENGINE_INFO("Decreased 1Zpos to {}", Bg1->m_transform->GetPosition().z);
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_H))
		{
			Bg2->m_transform->SetPosition(Bg2->m_transform->GetPosition() + glm::vec3(0, 0, 5));
			ENGINE_INFO("Increased 2Zpos to {}", Bg2->m_transform->GetPosition().z);
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_J))
		{
			Bg2->m_transform->SetPosition(Bg2->m_transform->GetPosition() + glm::vec3(0, 0, -5));
			ENGINE_INFO("Decreased 2Zpos to {}", Bg2->m_transform->GetPosition().z);
		}

		//if (Input::GetKeyHold(Input::KeyCode::KEY_N))
		//{
		//	Bg2->GetComponent<SoundPlayer>()->IncreaseVolume();
		//}

		//if (Input::GetKeyHold(Input::KeyCode::KEY_M))
		//{
		//	Bg2->GetComponent<SoundPlayer>()->DecreaseVolume();
		//}
	}

	void Init(void)
	{
		srand(time(NULL));
		g_gameInfo = &(GameInfo::GetInstance());
		g_isDebug = false;
		//Initialize All System
		//Core

		//Runtime
		Core::Logger::Init();
		Graphic::Init();

		//Input
		//Bool for debugging
		Input::Init(false);

		mouseCursor = std::make_unique<GameObject>();
		mouseCursor->m_transform->SetScale(glm::vec3(50, 50, 1));
		mouseCursor->AddComponent<MeshRenderer>();
		mouseCursor->GetComponent<MeshRenderer>()->SetTexture(TexturePath("UIs/Cursor"));
		mouseCursor->GetComponent<MeshRenderer>()->SetLayer(9999);
		mouseCursor->GetComponent<MeshRenderer>()->SetUI(true);
		mouseCursor->GetComponent<MeshRenderer>()->Init();

		//Physics
		g_physicScene = new Physic::PhysicScene();

		ENGINE_WARN("Engine Initialized");

		SceneManagement::ActiveScene = std::make_unique<SceneManagement::Scene>();
		ENGINE_WARN("Made Temp Scene");

		//Add Physic
		//Set name to layer
		Physic::PhysicScene::GetInstance()->SetLayerName("Player", Physic::Layer::PHYSIC_LAYER_1);
		Physic::PhysicScene::GetInstance()->SetLayerName("Enemy", Physic::Layer::PHYSIC_LAYER_2);
		Physic::PhysicScene::GetInstance()->SetLayerName("Platform", Physic::Layer::PHYSIC_LAYER_3);
		Physic::PhysicScene::GetInstance()->SetLayerName("Bullet", Physic::Layer::PHYSIC_LAYER_4);
		Physic::PhysicScene::GetInstance()->SetLayerName("GroundEnemy", Physic::Layer::PHYSIC_LAYER_5);
		Physic::PhysicScene::GetInstance()->SetLayerName("EnemyBullet", Physic::Layer::PHYSIC_LAYER_6);
		Physic::PhysicScene::GetInstance()->SetLayerName("Default", Physic::Layer::PHYSIC_LAYER_7);
		Physic::PhysicScene::GetInstance()->SetLayerName("Item", Physic::Layer::PHYSIC_LAYER_8);
		//Set collision between layer
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("Player", "Platform", Physic::RESOLVE_TYPE::COLLISION);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("GroundEnemy", "Platform", Physic::RESOLVE_TYPE::COLLISION);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("Bullet", "Platform", Physic::RESOLVE_TYPE::COLLISION);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("EnemyBullet", "Platform", Physic::RESOLVE_TYPE::COLLISION);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("Item", "Platform", Physic::RESOLVE_TYPE::COLLISION);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("Player", "Enemy", Physic::RESOLVE_TYPE::TRIGGER);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("Bullet", "Enemy", Physic::RESOLVE_TYPE::TRIGGER);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("Bullet", "GroundEnemy", Physic::RESOLVE_TYPE::TRIGGER);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("EnemyBullet", "Player", Physic::RESOLVE_TYPE::TRIGGER);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("Item", "Player", Physic::RESOLVE_TYPE::TRIGGER);

		
		title = new GameObject();
		title->AddComponent<MeshRenderer>();
		title->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
		title->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/mockup_title.jpg");
		title->GetComponent<MeshRenderer>()->SetLayer(-1);
		
		title->m_transform->SetScale(glm::vec3(Graphic::Window::GetWidth(), Graphic::Window::GetHeight(), 1.0f));
		
		while (!Input::GetKeyDown(Input::KeyCode::KEY_SPACE))
		{
			Input::Update();
		
			Graphic::Render();
		
			if (Input::GetKeyDown(Input::KeyCode::KEY_SPACE))
			{
				title->SetActive(false);
			}
		}

		//Serialization::LoadObject(*SceneManagement::ActiveScene, ScenePath("SerializationTest"));

		//700 lines is in here
		{
			//this point is where it all start =========================================

			//GameObject
			gamecontroller = Instantiate().get();
			ui_HPbar = Instantiate();
			ui_StaminaBar = Instantiate();
			
			gamecontroller->AddComponent<GameController>();
			gamecontroller->GetComponent<GameController>()->SetGameState(GAME_STATE::MAINMENU);
			gamecontroller->GetComponent<GameController>()->SetGameplayState(GAMEPLAY_STATE::NORMAL);


			gamecontroller->AddComponent<UIController>();

			//MenuUI
			{
				std::shared_ptr<GameObject> uiBG = Instantiate();
				uiBG->AddComponent<MeshRenderer>();
				uiBG->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				uiBG->GetComponent<MeshRenderer>()->SetTexture(TexturePath("UIs/Panel02"));
				uiBG->GetComponent<MeshRenderer>()->SetUI(true);
				uiBG->GetComponent<MeshRenderer>()->SetLayer(5);

				uiBG->m_transform->SetScale(glm::vec3(800.0f, 600.0f, 1.0f));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Highscore].push_back(uiBG);
				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Credit].push_back(uiBG);
				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(uiBG);
				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::GameOver].push_back(uiBG);


				std::shared_ptr<GameObject> mainmenuBG = Instantiate();
				mainmenuBG->AddComponent<MeshRenderer>();
				mainmenuBG->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				mainmenuBG->GetComponent<MeshRenderer>()->SetTexture(TexturePath("UIs/MenuBrickWall"));
				mainmenuBG->GetComponent<MeshRenderer>()->SetUI(true);
				mainmenuBG->GetComponent<MeshRenderer>()->SetLayer(5);

				mainmenuBG->m_transform->SetScale(glm::vec3(Graphic::Window::GetWidth() * (7.0f / 16.0f), Graphic::Window::GetHeight(), 1.0f));
				mainmenuBG->m_transform->SetPosition(glm::vec3(-400, 0, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::MainMenu].push_back(mainmenuBG);
				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Loadout].push_back(mainmenuBG);

				std::shared_ptr<GameObject> buttonpanelBG = Instantiate();
				buttonpanelBG->AddComponent<MeshRenderer>();
				buttonpanelBG->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				buttonpanelBG->GetComponent<MeshRenderer>()->SetTexture(TexturePath("UIs/ButtonsMenuNEW"));
				buttonpanelBG->GetComponent<MeshRenderer>()->SetUI(true);
				buttonpanelBG->GetComponent<MeshRenderer>()->SetLayer(5);
			
				buttonpanelBG->m_transform->SetScale(glm::vec3(200, 500, 1.0f));
				buttonpanelBG->m_transform->SetPosition(glm::vec3(-500, 0, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::MainMenu].push_back(buttonpanelBG);

				std::shared_ptr<GameObject> startbuttonpanelBG = Instantiate();
				startbuttonpanelBG->AddComponent<MeshRenderer>();
				startbuttonpanelBG->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				startbuttonpanelBG->GetComponent<MeshRenderer>()->SetTexture(TexturePath("UIs/ButtonStartNEW"));
				startbuttonpanelBG->GetComponent<MeshRenderer>()->SetUI(true);
				startbuttonpanelBG->GetComponent<MeshRenderer>()->SetLayer(5);
				
				startbuttonpanelBG->m_transform->SetScale(glm::vec3(200, 500, 1.0f));
				startbuttonpanelBG->m_transform->SetPosition(glm::vec3(-400, 0, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::MainMenu].push_back(startbuttonpanelBG);

				std::shared_ptr<GameObject> BackButton = Instantiate();
				BackButton->AddComponent<MeshRenderer>();
				BackButton->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				BackButton->GetComponent<MeshRenderer>()->SetUI(true);
				BackButton->GetComponent<MeshRenderer>()->SetLayer(10);
				BackButton->m_transform->SetScale(glm::vec3(100, 50, 1));
				BackButton->m_transform->SetPosition(glm::vec3(-300, 200, 1));
				BackButton->AddComponent<Button>();
				BackButton->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::UICONTROL, UI_GROUP::MainMenu);
				BackButton->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Highscore].push_back(BackButton);
				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Credit].push_back(BackButton);
				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(BackButton);

				std::shared_ptr<GameObject> logo = Instantiate();
				logo->AddComponent<MeshRenderer>();
				logo->GetComponent<MeshRenderer>()->SetTexture(TexturePath("UIs/WhiteLogo"));
				logo->GetComponent<MeshRenderer>()->SetUI(true);
				logo->GetComponent<MeshRenderer>()->SetLayer(10);
				logo->m_transform->SetScale(glm::vec3(68 * 3, 55 * 3, 1));
				logo->m_transform->SetPosition(glm::vec3(-500, 200, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::MainMenu].push_back(logo);

				OnHoverModifier menuHoverModifier;
				menuHoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);
				menuHoverModifier.ReTexturePath = TexturePath("UIs/ButtonSelection");

				std::shared_ptr<GameObject> playButton = Instantiate();
				playButton->AddComponent<MeshRenderer>();
				playButton->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				playButton->GetComponent<MeshRenderer>()->SetUI(true);
				playButton->GetComponent<MeshRenderer>()->SetLayer(10);
				playButton->m_transform->SetScale(glm::vec3(100, 50, 1));
				playButton->m_transform->SetPosition(glm::vec3(-500, 50, 1));
				playButton->AddComponent<Button>();
				playButton->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::STATECONTROL, GAME_STATE::LOADOUT);
				playButton->GetComponent<Button>()->SetModifier(menuHoverModifier);

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::MainMenu].push_back(playButton);

				std::shared_ptr<GameObject> playButtonText = Instantiate();
				playButtonText->AddComponent<TextRenderer>();
				playButtonText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				playButtonText->GetComponent<TextRenderer>()->SetText("Start");
				playButtonText->m_transform->SetPosition(glm::vec3(-550, 50, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::MainMenu].push_back(playButtonText);

				std::shared_ptr<GameObject> highscoreButton = Instantiate();
				highscoreButton->AddComponent<MeshRenderer>();
				highscoreButton->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				highscoreButton->GetComponent<MeshRenderer>()->SetUI(true);
				highscoreButton->GetComponent<MeshRenderer>()->SetLayer(10);
				highscoreButton->m_transform->SetScale(glm::vec3(100, 50, 1));
				highscoreButton->m_transform->SetPosition(glm::vec3(-500, -10, 1));
				highscoreButton->AddComponent<Button>();
				highscoreButton->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::UICONTROL, UI_GROUP::Highscore);
				highscoreButton->GetComponent<Button>()->SetModifier(menuHoverModifier);

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::MainMenu].push_back(highscoreButton);

				std::shared_ptr<GameObject> highScoreButtonText = Instantiate();
				highScoreButtonText->AddComponent<TextRenderer>();
				highScoreButtonText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				highScoreButtonText->GetComponent<TextRenderer>()->SetText("HighScore");
				highScoreButtonText->m_transform->SetPosition(glm::vec3(-550, -10, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::MainMenu].push_back(highScoreButtonText);

				std::shared_ptr<GameObject> optionButton = Instantiate();
				optionButton->AddComponent<MeshRenderer>();
				optionButton->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				optionButton->GetComponent<MeshRenderer>()->SetUI(true);
				optionButton->GetComponent<MeshRenderer>()->SetLayer(10);
				optionButton->m_transform->SetScale(glm::vec3(100, 50, 1));
				optionButton->m_transform->SetPosition(glm::vec3(-500, -70, 1));
				optionButton->AddComponent<Button>();
				optionButton->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::UICONTROL, UI_GROUP::Option);
				optionButton->GetComponent<Button>()->SetModifier(menuHoverModifier);

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::MainMenu].push_back(optionButton);

				std::shared_ptr<GameObject> optionButtonText = Instantiate();
				optionButtonText->AddComponent<TextRenderer>();
				optionButtonText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				optionButtonText->GetComponent<TextRenderer>()->SetText("Option");
				optionButtonText->m_transform->SetPosition(glm::vec3(-550, -70, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::MainMenu].push_back(optionButtonText);


				std::shared_ptr<GameObject> creditButton = Instantiate();
				creditButton->AddComponent<MeshRenderer>();
				creditButton->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				creditButton->GetComponent<MeshRenderer>()->SetUI(true);
				creditButton->GetComponent<MeshRenderer>()->SetLayer(10);
				creditButton->m_transform->SetScale(glm::vec3(100, 50, 1));
				creditButton->m_transform->SetPosition(glm::vec3(-500, -130, 1));
				creditButton->AddComponent<Button>();
				creditButton->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::UICONTROL, UI_GROUP::Credit);
				creditButton->GetComponent<Button>()->SetModifier(menuHoverModifier);

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::MainMenu].push_back(creditButton);

				std::shared_ptr<GameObject> creditButtonText = Instantiate();
				creditButtonText->AddComponent<TextRenderer>();
				creditButtonText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				creditButtonText->GetComponent<TextRenderer>()->SetText("Credit");
				creditButtonText->m_transform->SetPosition(glm::vec3(-550, -130, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::MainMenu].push_back(creditButtonText);

				std::shared_ptr<GameObject> quitButton = Instantiate();
				quitButton->AddComponent<MeshRenderer>();
				quitButton->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				quitButton->GetComponent<MeshRenderer>()->SetUI(true);
				quitButton->GetComponent<MeshRenderer>()->SetLayer(10);
				quitButton->m_transform->SetScale(glm::vec3(100, 50, 1));
				quitButton->m_transform->SetPosition(glm::vec3(-500, -190, 1));
				quitButton->AddComponent<Button>();
				quitButton->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::UICONTROL, UI_GROUP::QuitGameWarn);
				quitButton->GetComponent<Button>()->SetModifier(menuHoverModifier);

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::MainMenu].push_back(quitButton);

				std::shared_ptr<GameObject> quitButtonText = Instantiate();
				quitButtonText->AddComponent<TextRenderer>();
				quitButtonText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				quitButtonText->GetComponent<TextRenderer>()->SetText("Quit");
				quitButtonText->m_transform->SetPosition(glm::vec3(-550, -190, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::MainMenu].push_back(quitButtonText);
			}

			//GameplayUI
			{
				ui_ScoreText = Instantiate();
				ui_ScoreText->AddComponent<TextRenderer>();
				ui_ScoreText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 50);
				ui_ScoreText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
				ui_ScoreText->m_transform->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
				ui_ScoreText->m_transform->SetPosition(glm::vec3((Graphic::Window::GetWidth() / -2) + 50.0f, (Graphic::Window::GetHeight() / -2) + 50.0f, 1.0f));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Gameplay].push_back(ui_ScoreText);

				ui_ComboText = Instantiate();
				ui_ComboText->AddComponent<TextRenderer>();
				ui_ComboText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 50);
				ui_ComboText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
				ui_ComboText->m_transform->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
				ui_ComboText->m_transform->SetPosition(glm::vec3((Graphic::Window::GetWidth() / -2) + 50.0f, (Graphic::Window::GetHeight() / -2) + 150.0f, 1.0f));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Gameplay].push_back(ui_ComboText);

				ui_HPbar->AddComponent<MeshRenderer>();
				ui_HPbar->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_HPbar->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Red.jpg");
				ui_HPbar->GetComponent<MeshRenderer>()->SetUI(true);
				ui_HPbar->GetComponent<MeshRenderer>()->SetLayer(10);
				ui_HPbar->m_transform->SetScale(glm::vec3(500.0f, 40.0f, 1.0f));
				ui_HPbar->m_transform->SetPosition(glm::vec3((Graphic::Window::GetWidth() / -2) + 280.0f, (Graphic::Window::GetHeight() / 2) - 40.0f, 1.0f));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Gameplay].push_back(ui_HPbar);

				ui_StaminaBar->AddComponent<MeshRenderer>();
				ui_StaminaBar->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_StaminaBar->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Blue.jpg");
				ui_StaminaBar->GetComponent<MeshRenderer>()->SetUI(true);
				ui_StaminaBar->GetComponent<MeshRenderer>()->SetLayer(10);
				ui_StaminaBar->m_transform->SetScale(glm::vec3(500.0f, 20.0f, 1.0f));
				ui_StaminaBar->m_transform->SetPosition(glm::vec3((Graphic::Window::GetWidth() / -2) + 280.0f, (Graphic::Window::GetHeight() / 2) - 80.0f, 1.0f));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Gameplay].push_back(ui_StaminaBar);

				ui_BossHP = Instantiate();
				ui_BossHP->AddComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_BossHP->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Red.jpg");
				ui_BossHP->GetComponent<MeshRenderer>()->SetUI(true);
				ui_BossHP->m_transform->SetScale(glm::vec3(100.0f, 20.0f, 1.0f));
				ui_BossHP->m_transform->SetPosition(glm::vec3(0, (Graphic::Window::GetHeight() / 2) + 200, 1.0f));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Gameplay].push_back(ui_BossHP);

				ui_Weapongp = Instantiate();
				ui_Weapongp->AddComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_Weapongp->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				ui_Weapongp->GetComponent<MeshRenderer>()->SetUI(true);
				ui_Weapongp->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));
				ui_Weapongp->m_transform->SetPosition(glm::vec3((Graphic::Window::GetWidth() / 2) - 50.0f, (Graphic::Window::GetHeight() / -2) + 40.0f, 1.0f));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Gameplay].push_back(ui_Weapongp);

				ui_Artifactgp1 = Instantiate();
				ui_Artifactgp1->AddComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_Artifactgp1->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				ui_Artifactgp1->GetComponent<MeshRenderer>()->SetUI(true);
				ui_Artifactgp1->m_transform->SetScale(glm::vec3(50.0f, 50.0f, 1.0f));
				ui_Artifactgp1->m_transform->SetPosition(glm::vec3((Graphic::Window::GetWidth() / 2) - 50.0f, (Graphic::Window::GetHeight() / -2) + 120.0f, 1.0f));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Gameplay].push_back(ui_Artifactgp1);

				ui_Artifactgp2 = Instantiate();
				ui_Artifactgp2->AddComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_Artifactgp2->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				ui_Artifactgp2->GetComponent<MeshRenderer>()->SetUI(true);
				ui_Artifactgp2->m_transform->SetScale(glm::vec3(50.0f, 50.0f, 1.0f));
				ui_Artifactgp2->m_transform->SetPosition(glm::vec3((Graphic::Window::GetWidth() / 2) - 50.0f, (Graphic::Window::GetHeight() / -2) + 190.0f, 1.0f));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Gameplay].push_back(ui_Artifactgp2);

				ui_HPtext = Instantiate();
				ui_HPtext->AddComponent<TextRenderer>();
				ui_HPtext->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 10);
				ui_HPtext->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
				ui_HPtext->m_transform->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
				ui_HPtext->m_transform->SetPosition(glm::vec3((Graphic::Window::GetWidth() / -2) + 280.0f, (Graphic::Window::GetHeight() / 2) - 40.0f, 1.0f));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Gameplay].push_back(ui_HPtext);

				ui_BossHPtext = Instantiate();
				ui_BossHPtext->AddComponent<TextRenderer>();
				ui_BossHPtext->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 10);
				ui_BossHPtext->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
				ui_BossHPtext->m_transform->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
				ui_BossHPtext->m_transform->SetPosition(glm::vec3((Graphic::Window::GetWidth() / 2), (Graphic::Window::GetHeight() / 2) + 200, 1.0f));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Gameplay].push_back(ui_BossHPtext);
			}

			//LoadoutUI
			{
				std::shared_ptr<GameObject> startButton = Instantiate();
				startButton->AddComponent<MeshRenderer>();
				startButton->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				startButton->GetComponent<MeshRenderer>()->SetUI(true);
				startButton->GetComponent<MeshRenderer>()->SetLayer(10);
				startButton->m_transform->SetScale(glm::vec3(68, 55, 1));
				startButton->m_transform->SetPosition(glm::vec3(-300.0f, -250.0f, 1.0f));
				startButton->AddComponent<Button>();
				startButton->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::STATECONTROL, GAME_STATE::GAMEPLAY);
				startButton->GetComponent<Button>()->hoverModifier.ReTexturePath = TexturePath("UIs/ButtonSelection");
				startButton->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);
				startButton->SetActive(false);

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Loadout].push_back(startButton);

				std::shared_ptr<GameObject> startButtonText = Instantiate();
				startButtonText->AddComponent<TextRenderer>();
				startButtonText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				startButtonText->GetComponent<TextRenderer>()->SetText("Start");
				startButtonText->m_transform->SetPosition(glm::vec3(-300.0f, -250.0f, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Loadout].push_back(startButtonText);

				std::shared_ptr<GameObject> ui_LoadOut = Instantiate();
				std::shared_ptr<GameObject> ui_button = Instantiate();

				ui_LoadOut->AddComponent<LoadoutUI>();
				ui_LoadOut->AddComponent<MeshRenderer>();
				ui_LoadOut->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_LoadOut->GetComponent<MeshRenderer>()->SetTexture(TexturePath("UIs/Panel02"));
				ui_LoadOut->GetComponent<MeshRenderer>()->SetUI(true);
				ui_LoadOut->GetComponent<MeshRenderer>()->SetLayer(5);

				ui_LoadOut->m_transform->SetScale(glm::vec3(800.0f, 600.0f, 1.0f));

				ui_button->AddComponent<MeshRenderer>();
				ui_button->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_button->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/White.jpg");
				ui_button->GetComponent<MeshRenderer>()->SetUI(true);
				ui_button->GetComponent<MeshRenderer>()->SetLayer(6);
				ui_button->AddComponent<LoadoutSelectButton>();
				ui_button->AddComponent<Button>();
				ui_button->GetComponent<Button>()->buttonType = BUTTON_TYPE::LOADOUTSELECT;
				ui_button->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				ui_button->m_transform->SetScale(glm::vec3(100.0f, 100.0f, 1.0f));
				ui_button->m_transform->SetPosition(glm::vec3(-300.0f, -100.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->AssignArtifactDisplaySlot(ui_button);

				ui_button = Instantiate();

				ui_button->AddComponent<MeshRenderer>();
				ui_button->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_button->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/White.jpg");
				ui_button->GetComponent<MeshRenderer>()->SetUI(true);
				ui_button->GetComponent<MeshRenderer>()->SetLayer(6);
				ui_button->AddComponent<LoadoutSelectButton>();
				ui_button->AddComponent<Button>();
				ui_button->GetComponent<Button>()->buttonType = BUTTON_TYPE::LOADOUTSELECT;
				ui_button->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				ui_button->m_transform->SetScale(glm::vec3(100.0f, 100.0f, 1.0f));
				ui_button->m_transform->SetPosition(glm::vec3(-100.0f, -100.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->AssignArtifactDisplaySlot(ui_button);

				ui_button = Instantiate();

				ui_button->AddComponent<MeshRenderer>();
				ui_button->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_button->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/White.jpg");
				ui_button->GetComponent<MeshRenderer>()->SetUI(true);
				ui_button->GetComponent<MeshRenderer>()->SetLayer(6);
				ui_button->AddComponent<LoadoutSelectButton>();
				ui_button->AddComponent<Button>();
				ui_button->GetComponent<Button>()->buttonType = BUTTON_TYPE::LOADOUTSELECT;
				ui_button->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				ui_button->m_transform->SetScale(glm::vec3(200.0f, 100.0f, 1.0f));
				ui_button->m_transform->SetPosition(glm::vec3(-200.0f, 100.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->AssignWeaponDisplaySlot(ui_button);

				ui_button = Instantiate();

				ui_button->AddComponent<MeshRenderer>();
				ui_button->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_button->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/08_Amplifier"));
				ui_button->GetComponent<MeshRenderer>()->SetUI(true);
				ui_button->GetComponent<MeshRenderer>()->SetLayer(6);
				ui_button->AddComponent<LoadoutSelectButton>();
				ui_button->GetComponent<LoadoutSelectButton>()->SetType(ITEM_TYPE::ARTIFACT);
				ui_button->GetComponent<LoadoutSelectButton>()->SetEquipmentType(ARTIFACT_TYPE::ARTF_BULLETAMP);
				ui_button->GetComponent<LoadoutSelectButton>()->eq_name = "Bullet Amplifier";
				ui_button->GetComponent<LoadoutSelectButton>()->eq_description = "Make bullet more powerful";
				ui_button->AddComponent<Button>();
				ui_button->GetComponent<Button>()->buttonType = BUTTON_TYPE::LOADOUTSELECT;
				ui_button->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				ui_button->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));
				ui_button->m_transform->SetPosition(glm::vec3(80.0f, 20.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->AssignSelectButton(ui_button);

				ui_button = Instantiate();

				ui_button->AddComponent<MeshRenderer>();
				ui_button->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_button->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/01_Amplifiler"));
				ui_button->GetComponent<MeshRenderer>()->SetUI(true);
				ui_button->GetComponent<MeshRenderer>()->SetLayer(6);
				ui_button->AddComponent<LoadoutSelectButton>();
				ui_button->GetComponent<LoadoutSelectButton>()->SetType(ITEM_TYPE::ARTIFACT);
				ui_button->GetComponent<LoadoutSelectButton>()->SetEquipmentType(ARTIFACT_TYPE::ARTF_FIRERATEUP);
				ui_button->GetComponent<LoadoutSelectButton>()->eq_name = "FireRate UP";
				ui_button->GetComponent<LoadoutSelectButton>()->eq_description = "Make weapon shoot faster";
				ui_button->AddComponent<Button>();
				ui_button->GetComponent<Button>()->buttonType = BUTTON_TYPE::LOADOUTSELECT;
				ui_button->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				ui_button->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));
				ui_button->m_transform->SetPosition(glm::vec3(180.0f, 20.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->AssignSelectButton(ui_button);

				ui_button = Instantiate();

				ui_button->AddComponent<MeshRenderer>();
				ui_button->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_button->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/04_Speed"));
				ui_button->GetComponent<MeshRenderer>()->SetUI(true);
				ui_button->GetComponent<MeshRenderer>()->SetLayer(6);
				ui_button->AddComponent<LoadoutSelectButton>();
				ui_button->GetComponent<LoadoutSelectButton>()->SetType(ITEM_TYPE::ARTIFACT);
				ui_button->GetComponent<LoadoutSelectButton>()->SetEquipmentType(ARTIFACT_TYPE::ARTF_SPEEDRUNNER);
				ui_button->GetComponent<LoadoutSelectButton>()->eq_name = "SpeedRunner";
				ui_button->GetComponent<LoadoutSelectButton>()->eq_description = "Make player move faster";
				ui_button->AddComponent<Button>();
				ui_button->GetComponent<Button>()->buttonType = BUTTON_TYPE::LOADOUTSELECT;
				ui_button->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				ui_button->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));
				ui_button->m_transform->SetPosition(glm::vec3(280.0f, 20.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->AssignSelectButton(ui_button);

				ui_button = Instantiate();

				ui_button->AddComponent<MeshRenderer>();
				ui_button->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_button->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/02_Buffer"));
				ui_button->GetComponent<MeshRenderer>()->SetUI(true);
				ui_button->GetComponent<MeshRenderer>()->SetLayer(6);
				ui_button->AddComponent<LoadoutSelectButton>();
				ui_button->GetComponent<LoadoutSelectButton>()->SetType(ITEM_TYPE::ARTIFACT);
				ui_button->GetComponent<LoadoutSelectButton>()->SetEquipmentType(ARTIFACT_TYPE::ARTF_ATKUP);
				ui_button->GetComponent<LoadoutSelectButton>()->eq_name = "Attack UP";
				ui_button->GetComponent<LoadoutSelectButton>()->eq_description = "Make weapon to deal more damage";
				ui_button->AddComponent<Button>();
				ui_button->GetComponent<Button>()->buttonType = BUTTON_TYPE::LOADOUTSELECT;
				ui_button->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				ui_button->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));
				ui_button->m_transform->SetPosition(glm::vec3(380.0f, 20.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->AssignSelectButton(ui_button);

				ui_button = Instantiate();

				ui_button->AddComponent<MeshRenderer>();
				ui_button->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_button->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/03_Gravity"));
				ui_button->GetComponent<MeshRenderer>()->SetUI(true);
				ui_button->GetComponent<MeshRenderer>()->SetLayer(6);
				ui_button->AddComponent<LoadoutSelectButton>();
				ui_button->GetComponent<LoadoutSelectButton>()->SetType(ITEM_TYPE::ARTIFACT);
				ui_button->GetComponent<LoadoutSelectButton>()->SetEquipmentType(ARTIFACT_TYPE::ARTF_LOWGRAV);
				ui_button->GetComponent<LoadoutSelectButton>()->eq_name = "LowGravity";
				ui_button->GetComponent<LoadoutSelectButton>()->eq_description = "Make player fall slower";
				ui_button->AddComponent<Button>();
				ui_button->GetComponent<Button>()->buttonType = BUTTON_TYPE::LOADOUTSELECT;
				ui_button->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				ui_button->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));
				ui_button->m_transform->SetPosition(glm::vec3(80.0f, -50.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->AssignSelectButton(ui_button);

				ui_button = Instantiate();

				ui_button->AddComponent<MeshRenderer>();
				ui_button->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_button->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/05_Amplifier"));
				ui_button->GetComponent<MeshRenderer>()->SetUI(true);
				ui_button->GetComponent<MeshRenderer>()->SetLayer(6);
				ui_button->AddComponent<LoadoutSelectButton>();
				ui_button->GetComponent<LoadoutSelectButton>()->SetType(ITEM_TYPE::ARTIFACT);
				ui_button->GetComponent<LoadoutSelectButton>()->SetEquipmentType(ARTIFACT_TYPE::ARTF_ARTIFACTAMP);
				ui_button->GetComponent<LoadoutSelectButton>()->eq_name = "Artifact Amplifier";
				ui_button->GetComponent<LoadoutSelectButton>()->eq_description = "Double the artifact power";
				ui_button->AddComponent<Button>();
				ui_button->GetComponent<Button>()->buttonType = BUTTON_TYPE::LOADOUTSELECT;
				ui_button->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				ui_button->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));
				ui_button->m_transform->SetPosition(glm::vec3(180.0f, -50.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->AssignSelectButton(ui_button);

				ui_button = Instantiate();

				ui_button->AddComponent<MeshRenderer>();
				ui_button->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_button->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/07_Curse"));
				ui_button->GetComponent<MeshRenderer>()->SetUI(true);
				ui_button->GetComponent<MeshRenderer>()->SetLayer(6);
				ui_button->AddComponent<LoadoutSelectButton>();
				ui_button->GetComponent<LoadoutSelectButton>()->SetType(ITEM_TYPE::ARTIFACT);
				ui_button->GetComponent<LoadoutSelectButton>()->SetEquipmentType(ARTIFACT_TYPE::ARTF_CURSEDPENDANT);
				ui_button->GetComponent<LoadoutSelectButton>()->eq_name = "Curse Pendant";
				ui_button->GetComponent<LoadoutSelectButton>()->eq_description = "One shot kill everything, including player";
				ui_button->AddComponent<Button>();
				ui_button->GetComponent<Button>()->buttonType = BUTTON_TYPE::LOADOUTSELECT;
				ui_button->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				ui_button->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));
				ui_button->m_transform->SetPosition(glm::vec3(280.0f, -50.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->AssignSelectButton(ui_button);

				ui_button = Instantiate();

				ui_button->AddComponent<MeshRenderer>();
				ui_button->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_button->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/MachineGun.png");
				ui_button->GetComponent<MeshRenderer>()->SetUI(true);
				ui_button->GetComponent<MeshRenderer>()->SetLayer(6);
				ui_button->AddComponent<LoadoutSelectButton>();
				ui_button->GetComponent<LoadoutSelectButton>()->SetType(ITEM_TYPE::WEAPON);
				ui_button->GetComponent<LoadoutSelectButton>()->SetEquipmentType(WEAPON_TYPE::WEAPON_MACHINEGUN);
				ui_button->GetComponent<LoadoutSelectButton>()->eq_name = "Machine Gun";
				ui_button->GetComponent<LoadoutSelectButton>()->eq_description = "Shoot bullet";
				ui_button->AddComponent<Button>();
				ui_button->GetComponent<Button>()->buttonType = BUTTON_TYPE::LOADOUTSELECT;
				ui_button->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				ui_button->m_transform->SetScale(glm::vec3(70.0f, 35.0f, 1.0f));
				ui_button->m_transform->SetPosition(glm::vec3(100.0f, 100.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->AssignSelectButton(ui_button);

				ui_button = Instantiate();

				ui_button->AddComponent<MeshRenderer>();
				ui_button->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_button->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/laser_idle-01"));
				ui_button->GetComponent<MeshRenderer>()->SetUI(true);
				ui_button->GetComponent<MeshRenderer>()->SetLayer(6);
				ui_button->AddComponent<LoadoutSelectButton>();
				ui_button->GetComponent<LoadoutSelectButton>()->SetType(ITEM_TYPE::WEAPON);
				ui_button->GetComponent<LoadoutSelectButton>()->SetEquipmentType(WEAPON_TYPE::WEAPON_LASER);
				ui_button->GetComponent<LoadoutSelectButton>()->eq_name = "Laser Gun";
				ui_button->GetComponent<LoadoutSelectButton>()->eq_description = "Shoot laser";
				ui_button->AddComponent<Button>();
				ui_button->GetComponent<Button>()->buttonType = BUTTON_TYPE::LOADOUTSELECT;
				ui_button->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				ui_button->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));
				ui_button->m_transform->SetPosition(glm::vec3(200.0f, 100.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->AssignSelectButton(ui_button);

				ui_button = Instantiate();

				ui_button->AddComponent<MeshRenderer>();
				ui_button->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_button->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/grenadeLaucher_idle-01"));
				ui_button->GetComponent<MeshRenderer>()->SetUI(true);
				ui_button->GetComponent<MeshRenderer>()->SetLayer(6);
				ui_button->AddComponent<LoadoutSelectButton>();
				ui_button->GetComponent<LoadoutSelectButton>()->SetType(ITEM_TYPE::WEAPON);
				ui_button->GetComponent<LoadoutSelectButton>()->SetEquipmentType(WEAPON_TYPE::WEAPON_GRENADELAUNCHER);
				ui_button->GetComponent<LoadoutSelectButton>()->eq_name = "GrenadeLauncher";
				ui_button->GetComponent<LoadoutSelectButton>()->eq_description = "Shoot Grenade";
				ui_button->AddComponent<Button>();
				ui_button->GetComponent<Button>()->buttonType = BUTTON_TYPE::LOADOUTSELECT;
				ui_button->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				ui_button->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));
				ui_button->m_transform->SetPosition(glm::vec3(300.0f, 100.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->AssignSelectButton(ui_button);

				ui_button = Instantiate();

				ui_button->AddComponent<MeshRenderer>();
				ui_button->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_button->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/zapper_idle-01"));
				ui_button->GetComponent<MeshRenderer>()->SetUI(true);
				ui_button->GetComponent<MeshRenderer>()->SetLayer(6);
				ui_button->AddComponent<LoadoutSelectButton>();
				ui_button->GetComponent<LoadoutSelectButton>()->SetType(ITEM_TYPE::WEAPON);
				ui_button->GetComponent<LoadoutSelectButton>()->SetEquipmentType(WEAPON_TYPE::WEAPON_ZAPPER);
				ui_button->GetComponent<LoadoutSelectButton>()->eq_name = "ZapperGun";
				ui_button->GetComponent<LoadoutSelectButton>()->eq_description = "Shoot ZapBullet";
				ui_button->AddComponent<Button>();
				ui_button->GetComponent<Button>()->buttonType = BUTTON_TYPE::LOADOUTSELECT;
				ui_button->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				ui_button->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));
				ui_button->m_transform->SetPosition(glm::vec3(400.0f, 100.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->AssignSelectButton(ui_button);

				ui_button = Instantiate();

				ui_button->AddComponent<MeshRenderer>();
				ui_button->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				ui_button->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/blackhole_idle-04"));
				ui_button->GetComponent<MeshRenderer>()->SetUI(true);
				ui_button->GetComponent<MeshRenderer>()->SetLayer(6);
				ui_button->AddComponent<LoadoutSelectButton>();
				ui_button->GetComponent<LoadoutSelectButton>()->SetType(ITEM_TYPE::WEAPON);
				ui_button->GetComponent<LoadoutSelectButton>()->SetEquipmentType(WEAPON_TYPE::WEAPON_BLACKHOLE);
				ui_button->GetComponent<LoadoutSelectButton>()->eq_name = "Blackhole Gun";
				ui_button->GetComponent<LoadoutSelectButton>()->eq_description = "Shoot blackhole";
				ui_button->AddComponent<Button>();
				ui_button->GetComponent<Button>()->buttonType = BUTTON_TYPE::LOADOUTSELECT;
				ui_button->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				ui_button->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));
				ui_button->m_transform->SetPosition(glm::vec3(500.0f, 100.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->AssignSelectButton(ui_button);

				std::shared_ptr<GameObject> LoadoutText;
				LoadoutText = Instantiate();

				LoadoutText->AddComponent<TextRenderer>();
				LoadoutText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 50);
				LoadoutText->GetComponent<TextRenderer>()->SetText("Test/n Test");
				LoadoutText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
				LoadoutText->m_transform->SetScale(glm::vec3(0.7f, 0.7f, 0.7f));
				LoadoutText->m_transform->SetPosition(glm::vec3(100.0f, -100.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->eqNameObj = LoadoutText;

				LoadoutText = Instantiate();

				LoadoutText->AddComponent<TextRenderer>();
				LoadoutText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 30);
				LoadoutText->GetComponent<TextRenderer>()->SetText("Test/n Test");
				LoadoutText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
				LoadoutText->m_transform->SetScale(glm::vec3(0.7f, 0.7f, 0.7f));
				LoadoutText->m_transform->SetPosition(glm::vec3(100.0f, -150.0f, 1.0f));

				ui_LoadOut->GetComponent<LoadoutUI>()->eqDescriptionObj = LoadoutText;

				gamecontroller->GetComponent<GameController>()->loadoutUI = ui_LoadOut;

				std::shared_ptr<GameObject> LoadOutBackButton = Instantiate();
				LoadOutBackButton->AddComponent<MeshRenderer>();
				LoadOutBackButton->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				LoadOutBackButton->GetComponent<MeshRenderer>()->SetUI(true);
				LoadOutBackButton->GetComponent<MeshRenderer>()->SetLayer(10);
				LoadOutBackButton->m_transform->SetScale(glm::vec3(100, 50, 1));
				LoadOutBackButton->m_transform->SetPosition(glm::vec3(-300, 200, 1));
				LoadOutBackButton->AddComponent<Button>();
				LoadOutBackButton->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::STATECONTROL,GAME_STATE::MAINMENU);
				LoadOutBackButton->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Loadout].push_back(LoadOutBackButton);

				std::shared_ptr<GameObject> LoadoutTextBox = Instantiate();
				LoadoutTextBox->AddComponent<MeshRenderer>();
				LoadoutTextBox->GetComponent<MeshRenderer>()->SetTexture(TexturePath("UIs/Panel03"));
				LoadoutTextBox->GetComponent<MeshRenderer>()->SetUI(true);
				LoadoutTextBox->GetComponent<MeshRenderer>()->SetLayer(10);
				LoadoutTextBox->m_transform->SetScale(glm::vec3(200, 100, 1));
				LoadoutTextBox->m_transform->SetPosition(glm::vec3(200, -200, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Loadout].push_back(LoadoutTextBox);

				std::shared_ptr<GameObject> LoadoutWeaponSign = Instantiate();
				LoadoutWeaponSign->AddComponent<MeshRenderer>();
				LoadoutWeaponSign->GetComponent<MeshRenderer>()->SetTexture(TexturePath("UIs/Weapon"));
				LoadoutWeaponSign->GetComponent<MeshRenderer>()->SetUI(true);
				LoadoutWeaponSign->GetComponent<MeshRenderer>()->SetLayer(10);
				LoadoutWeaponSign->m_transform->SetScale(glm::vec3(200, 100, 1));
				LoadoutWeaponSign->m_transform->SetPosition(glm::vec3(-200, 100, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Loadout].push_back(LoadoutWeaponSign);

				std::shared_ptr<GameObject> LoadoutArtifactSign = Instantiate();
				LoadoutArtifactSign->AddComponent<MeshRenderer>();
				LoadoutArtifactSign->GetComponent<MeshRenderer>()->SetTexture(TexturePath("UIs/Artifacte"));
				LoadoutArtifactSign->GetComponent<MeshRenderer>()->SetUI(true);
				LoadoutArtifactSign->GetComponent<MeshRenderer>()->SetLayer(10);
				LoadoutArtifactSign->m_transform->SetScale(glm::vec3(200, 100, 1));
				LoadoutArtifactSign->m_transform->SetPosition(glm::vec3(-200, 0, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Loadout].push_back(LoadoutArtifactSign);

				std::shared_ptr<GameObject> LoadoutWeaponShelf = Instantiate();
				LoadoutWeaponShelf->AddComponent<MeshRenderer>();
				LoadoutWeaponShelf->GetComponent<MeshRenderer>()->SetTexture(TexturePath("UIs/Shelf01"));
				LoadoutWeaponShelf->GetComponent<MeshRenderer>()->SetUI(true);
				LoadoutWeaponShelf->GetComponent<MeshRenderer>()->SetLayer(10);
				LoadoutWeaponShelf->m_transform->SetScale(glm::vec3(200, 100, 1));
				LoadoutWeaponShelf->m_transform->SetPosition(glm::vec3(-200, 100, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Loadout].push_back(LoadoutWeaponShelf);

				std::shared_ptr<GameObject> LoadoutArtifactShelf = Instantiate();
				LoadoutArtifactShelf->AddComponent<MeshRenderer>();
				LoadoutArtifactShelf->GetComponent<MeshRenderer>()->SetTexture(TexturePath("UIs/Shelf01"));
				LoadoutArtifactShelf->GetComponent<MeshRenderer>()->SetUI(true);
				LoadoutArtifactShelf->GetComponent<MeshRenderer>()->SetLayer(10);
				LoadoutArtifactShelf->m_transform->SetScale(glm::vec3(200, 100, 1));
				LoadoutArtifactShelf->m_transform->SetPosition(glm::vec3(-200, 0, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Loadout].push_back(LoadoutArtifactShelf);

			}

			//GameOver UI
			{
				std::shared_ptr<GameObject> GameOverText = Instantiate();
				
				GameOverText->AddComponent<TextRenderer>();
				GameOverText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 50);
				GameOverText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
				GameOverText->GetComponent<TextRenderer>()->SetText("GameOver!");
				GameOverText->m_transform->SetPosition(glm::vec3(-100, 200, 1));

				std::shared_ptr<GameObject> GameOverScoreText = Instantiate();

				GameOverScoreText->AddComponent<TextRenderer>();
				GameOverScoreText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 50);
				GameOverScoreText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
				GameOverScoreText->GetComponent<TextRenderer>()->SetText("GameOver!");
				GameOverScoreText->m_transform->SetPosition(glm::vec3(-100, 100, 1));

				gamecontroller->GetComponent<UIController>()->GameOverScoreText = GameOverScoreText;

				std::shared_ptr<GameObject> GameOverButton = Instantiate();
				GameOverButton->AddComponent<MeshRenderer>();
				GameOverButton->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				GameOverButton->GetComponent<MeshRenderer>()->SetUI(true);
				GameOverButton->GetComponent<MeshRenderer>()->SetLayer(10);
				GameOverButton->m_transform->SetScale(glm::vec3(100, 50, 1));
				GameOverButton->m_transform->SetPosition(glm::vec3(0, -200, 1));
				GameOverButton->AddComponent<Button>();
				GameOverButton->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::UICONTROL, UI_GROUP::MainMenu);
				GameOverButton->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::GameOver].push_back(GameOverText);
				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::GameOver].push_back(GameOverButton);
				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::GameOver].push_back(GameOverScoreText);
			}

			//HighScore UI
			{
				std::shared_ptr<GameObject> HighScoreText = Instantiate();
				HighScoreText->AddComponent<TextRenderer>();
				HighScoreText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				HighScoreText->GetComponent<TextRenderer>()->SetText("Highscore");
				HighScoreText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f));
				HighScoreText->m_transform->SetPosition(glm::vec3(0, 0, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Highscore].push_back(HighScoreText);
			}

			//Option UI
			{
				std::shared_ptr<GameObject> OptionText = Instantiate();
				OptionText->AddComponent<TextRenderer>();
				OptionText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				OptionText->GetComponent<TextRenderer>()->SetText("Option");
				OptionText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f));
				OptionText->m_transform->SetPosition(glm::vec3(0, 200, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(OptionText);

				std::shared_ptr<GameObject> MasterVolumeText = Instantiate();
				MasterVolumeText->AddComponent<TextRenderer>();
				MasterVolumeText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				MasterVolumeText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f));
				MasterVolumeText->m_transform->SetPosition(glm::vec3(100, 50, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(MasterVolumeText);

				gamecontroller->GetComponent<UIController>()->MasterVolumeText = MasterVolumeText;

				std::shared_ptr<GameObject> MasterText = Instantiate();
				MasterText->AddComponent<TextRenderer>();
				MasterText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				MasterText->GetComponent<TextRenderer>()->SetText("Master Volume");
				MasterText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f));
				MasterText->m_transform->SetPosition(glm::vec3(-300, 50, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(MasterText);

				std::shared_ptr<GameObject> MasterUP = Instantiate();
				MasterUP->AddComponent<MeshRenderer>();
				MasterUP->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				MasterUP->GetComponent<MeshRenderer>()->SetUI(true);
				MasterUP->GetComponent<MeshRenderer>()->SetLayer(10);
				MasterUP->m_transform->SetScale(glm::vec3(20, 20, 1));
				MasterUP->m_transform->SetPosition(glm::vec3(150, 50, 1));
				MasterUP->AddComponent<Button>();
				MasterUP->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::OPTION, OPTION::Master_Increase);
				MasterUP->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(MasterUP);

				std::shared_ptr<GameObject> MasterDown = Instantiate();
				MasterDown->AddComponent<MeshRenderer>();
				MasterDown->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				MasterDown->GetComponent<MeshRenderer>()->SetUI(true);
				MasterDown->GetComponent<MeshRenderer>()->SetLayer(10);
				MasterDown->m_transform->SetScale(glm::vec3(20, 20, 1));
				MasterDown->m_transform->SetPosition(glm::vec3(50, 50, 1));
				MasterDown->AddComponent<Button>();
				MasterDown->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::OPTION, OPTION::Master_Decrease);
				MasterDown->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(MasterDown);

				std::shared_ptr<GameObject> MusicVolumeText = Instantiate();
				MusicVolumeText->AddComponent<TextRenderer>();
				MusicVolumeText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				MusicVolumeText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f));
				MusicVolumeText->m_transform->SetPosition(glm::vec3(100, 30, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(MusicVolumeText);

				gamecontroller->GetComponent<UIController>()->MusicVolumeText = MusicVolumeText;

				std::shared_ptr<GameObject> MusicText = Instantiate();
				MusicText->AddComponent<TextRenderer>();
				MusicText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				MusicText->GetComponent<TextRenderer>()->SetText("Music Volume");
				MusicText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f));
				MusicText->m_transform->SetPosition(glm::vec3(-300, 30, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(MusicText);

				std::shared_ptr<GameObject> MusicUP = Instantiate();
				MusicUP->AddComponent<MeshRenderer>();
				MusicUP->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				MusicUP->GetComponent<MeshRenderer>()->SetUI(true);
				MusicUP->GetComponent<MeshRenderer>()->SetLayer(10);
				MusicUP->m_transform->SetScale(glm::vec3(20, 20, 1));
				MusicUP->m_transform->SetPosition(glm::vec3(150, 30, 1));
				MusicUP->AddComponent<Button>();
				MusicUP->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::OPTION, OPTION::Music_Increase);
				MusicUP->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(MusicUP);

				std::shared_ptr<GameObject> MusicDown = Instantiate();
				MusicDown->AddComponent<MeshRenderer>();
				MusicDown->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				MusicDown->GetComponent<MeshRenderer>()->SetUI(true);
				MusicDown->GetComponent<MeshRenderer>()->SetLayer(10);
				MusicDown->m_transform->SetScale(glm::vec3(20, 20, 1));
				MusicDown->m_transform->SetPosition(glm::vec3(50, 30, 1));
				MusicDown->AddComponent<Button>();
				MusicDown->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::OPTION, OPTION::Music_Decrease);
				MusicDown->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(MusicDown);


				std::shared_ptr<GameObject> SFXVolumeText = Instantiate();
				SFXVolumeText->AddComponent<TextRenderer>();
				SFXVolumeText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				SFXVolumeText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f));
				SFXVolumeText->m_transform->SetPosition(glm::vec3(100, 10, 1));


				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(SFXVolumeText);

				gamecontroller->GetComponent<UIController>()->SFXVolumeText = SFXVolumeText;

				std::shared_ptr<GameObject> SFXText = Instantiate();
				SFXText->AddComponent<TextRenderer>();
				SFXText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				SFXText->GetComponent<TextRenderer>()->SetText("SFX Volume");
				SFXText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f));
				SFXText->m_transform->SetPosition(glm::vec3(-300, 10, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(SFXText);

				std::shared_ptr<GameObject> SFXUP = Instantiate();
				SFXUP->AddComponent<MeshRenderer>();
				SFXUP->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				SFXUP->GetComponent<MeshRenderer>()->SetUI(true);
				SFXUP->GetComponent<MeshRenderer>()->SetLayer(10);
				SFXUP->m_transform->SetScale(glm::vec3(20, 20, 1));
				SFXUP->m_transform->SetPosition(glm::vec3(150, 10, 1));
				SFXUP->AddComponent<Button>();
				SFXUP->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::OPTION, OPTION::SFX_Increase);
				SFXUP->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(SFXUP);

				std::shared_ptr<GameObject> SFXDown = Instantiate();
				SFXDown->AddComponent<MeshRenderer>();
				SFXDown->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				SFXDown->GetComponent<MeshRenderer>()->SetUI(true);
				SFXDown->GetComponent<MeshRenderer>()->SetLayer(10);
				SFXDown->m_transform->SetScale(glm::vec3(20, 20, 1));
				SFXDown->m_transform->SetPosition(glm::vec3(50, 10, 1));
				SFXDown->AddComponent<Button>();
				SFXDown->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::OPTION, OPTION::SFX_Decrease);
				SFXDown->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(SFXDown);

				std::shared_ptr<GameObject> ResetProgressButton = Instantiate();
				ResetProgressButton->AddComponent<MeshRenderer>();
				ResetProgressButton->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				ResetProgressButton->GetComponent<MeshRenderer>()->SetUI(true);
				ResetProgressButton->GetComponent<MeshRenderer>()->SetLayer(10);
				ResetProgressButton->m_transform->SetScale(glm::vec3(100, 50, 1));
				ResetProgressButton->m_transform->SetPosition(glm::vec3(-200, -100, 1));
				ResetProgressButton->AddComponent<Button>();
				ResetProgressButton->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::UICONTROL, UI_GROUP::ResetProgressWarn);
				ResetProgressButton->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);
				ResetProgressButton->GetComponent<Button>()->hoverModifier.ReTexturePath = TexturePath("UIs/ButtonSelection");

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(ResetProgressButton);

				std::shared_ptr<GameObject> ResetProgressText = Instantiate();
				ResetProgressText->AddComponent<TextRenderer>();
				ResetProgressText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				ResetProgressText->GetComponent<TextRenderer>()->SetText("Reset Progress");
				ResetProgressText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f));
				ResetProgressText->m_transform->SetPosition(glm::vec3(-200, -100, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Option].push_back(ResetProgressText);
			}

			//WarningUI
			{
				std::shared_ptr<GameObject> WarningUiBG = Instantiate();
				WarningUiBG->AddComponent<MeshRenderer>();
				WarningUiBG->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				WarningUiBG->GetComponent<MeshRenderer>()->SetTexture(TexturePath("UIs/Panel02"));
				WarningUiBG->GetComponent<MeshRenderer>()->SetUI(true);
				WarningUiBG->GetComponent<MeshRenderer>()->SetLayer(5);
				
				WarningUiBG->m_transform->SetScale(glm::vec3(400.0f, 300.0f, 1.0f));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::ResetProgressWarn].push_back(WarningUiBG);
				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::QuitGameWarn].push_back(WarningUiBG);

				std::shared_ptr<GameObject> YesText = Instantiate();
				YesText->AddComponent<TextRenderer>();
				YesText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				YesText->GetComponent<TextRenderer>()->SetText("Yes");
				YesText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f));
				YesText->m_transform->SetPosition(glm::vec3(-50, -100, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::ResetProgressWarn].push_back(YesText);
				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::QuitGameWarn].push_back(YesText);

				std::shared_ptr<GameObject> NoText = Instantiate();
				NoText->AddComponent<TextRenderer>();
				NoText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				NoText->GetComponent<TextRenderer>()->SetText("No");
				NoText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f));
				NoText->m_transform->SetPosition(glm::vec3(50, -100, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::ResetProgressWarn].push_back(NoText);
				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::QuitGameWarn].push_back(NoText);

				std::shared_ptr<GameObject> WarnResetProgressText = Instantiate();
				WarnResetProgressText->AddComponent<TextRenderer>();
				WarnResetProgressText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				WarnResetProgressText->GetComponent<TextRenderer>()->SetText("Warning: Reset Progress");
				WarnResetProgressText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f));
				WarnResetProgressText->m_transform->SetPosition(glm::vec3(-100, 100, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::ResetProgressWarn].push_back(WarnResetProgressText);

				std::shared_ptr<GameObject> WarnResetYesButton = Instantiate();
				WarnResetYesButton->AddComponent<MeshRenderer>();
				WarnResetYesButton->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				WarnResetYesButton->GetComponent<MeshRenderer>()->SetUI(true);
				WarnResetYesButton->GetComponent<MeshRenderer>()->SetLayer(10);
				WarnResetYesButton->m_transform->SetScale(glm::vec3(100, 50, 1));
				WarnResetYesButton->m_transform->SetPosition(glm::vec3(-50, -100, 1));
				WarnResetYesButton->AddComponent<Button>();
				WarnResetYesButton->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::OPTION, OPTION::RESET_Progress);
				WarnResetYesButton->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);
				WarnResetYesButton->GetComponent<Button>()->hoverModifier.ReTexturePath = TexturePath("UIs/ButtonSelection");

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::ResetProgressWarn].push_back(WarnResetYesButton);

				std::shared_ptr<GameObject> WarnResetNoButton = Instantiate();
				WarnResetNoButton->AddComponent<MeshRenderer>();
				WarnResetNoButton->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				WarnResetNoButton->GetComponent<MeshRenderer>()->SetUI(true);
				WarnResetNoButton->GetComponent<MeshRenderer>()->SetLayer(10);
				WarnResetNoButton->m_transform->SetScale(glm::vec3(100, 50, 1));
				WarnResetNoButton->m_transform->SetPosition(glm::vec3(50, -100, 1));
				WarnResetNoButton->AddComponent<Button>();
				WarnResetNoButton->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::UICONTROL, UI_GROUP::Option);
				WarnResetNoButton->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);
				WarnResetNoButton->GetComponent<Button>()->hoverModifier.ReTexturePath = TexturePath("UIs/ButtonSelection");

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::ResetProgressWarn].push_back(WarnResetNoButton);

				std::shared_ptr<GameObject> WarnQuitText = Instantiate();
				WarnQuitText->AddComponent<TextRenderer>();
				WarnQuitText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				WarnQuitText->GetComponent<TextRenderer>()->SetText("Warning: Quit game");
				WarnQuitText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f));
				WarnQuitText->m_transform->SetPosition(glm::vec3(-100, 100, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::QuitGameWarn].push_back(WarnQuitText);

				std::shared_ptr<GameObject> WarnQuitYesButton = Instantiate();
				WarnQuitYesButton->AddComponent<MeshRenderer>();
				WarnQuitYesButton->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				WarnQuitYesButton->GetComponent<MeshRenderer>()->SetUI(true);
				WarnQuitYesButton->GetComponent<MeshRenderer>()->SetLayer(10);
				WarnQuitYesButton->m_transform->SetScale(glm::vec3(100, 50, 1));
				WarnQuitYesButton->m_transform->SetPosition(glm::vec3(-50, -100, 1));
				WarnQuitYesButton->AddComponent<Button>();
				WarnQuitYesButton->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::STATECONTROL, GAME_STATE::QUIT);
				WarnQuitYesButton->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);
				WarnQuitYesButton->GetComponent<Button>()->hoverModifier.ReTexturePath = TexturePath("UIs/ButtonSelection");

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::QuitGameWarn].push_back(WarnQuitYesButton);

				std::shared_ptr<GameObject> WarnQuitNoButton = Instantiate();
				WarnQuitNoButton->AddComponent<MeshRenderer>();
				WarnQuitNoButton->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/white.jpg");
				WarnQuitNoButton->GetComponent<MeshRenderer>()->SetUI(true);
				WarnQuitNoButton->GetComponent<MeshRenderer>()->SetLayer(10);
				WarnQuitNoButton->m_transform->SetScale(glm::vec3(100, 50, 1));
				WarnQuitNoButton->m_transform->SetPosition(glm::vec3(50, -100, 1));
				WarnQuitNoButton->AddComponent<Button>();
				WarnQuitNoButton->GetComponent<Button>()->SetButtonType(BUTTON_TYPE::UICONTROL, UI_GROUP::MainMenu);
				WarnQuitNoButton->GetComponent<Button>()->hoverModifier.ReColor = glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f);
				WarnQuitNoButton->GetComponent<Button>()->hoverModifier.ReTexturePath = TexturePath("UIs/ButtonSelection");

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::QuitGameWarn].push_back(WarnQuitNoButton);


			}

			//Credit UI
			{
				std::shared_ptr<GameObject> CreditText = Instantiate();
				CreditText->AddComponent<TextRenderer>();
				CreditText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				CreditText->GetComponent<TextRenderer>()->SetText("Credit");
				CreditText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f));
				CreditText->m_transform->SetPosition(glm::vec3(0, 0, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Credit].push_back(CreditText);

				std::shared_ptr<GameObject> CreditContentText = Instantiate();
				CreditContentText->AddComponent<TextRenderer>();
				CreditContentText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 20);
				CreditContentText->GetComponent<TextRenderer>()->SetText("Credit");
				CreditContentText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f));
				CreditContentText->m_transform->SetPosition(glm::vec3(0, -100, 1));

				gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Credit].push_back(CreditContentText);
			}

			std::shared_ptr<GameObject> ui_highscore = Instantiate();
			ui_highscore->AddComponent<HighScoreUI>();

			gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::GameOver].push_back(ui_highscore);
			gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Highscore].push_back(ui_highscore);

			std::shared_ptr<GameObject> ScoreText;

			ScoreText = Instantiate();

			ScoreText->AddComponent<TextRenderer>();
			ScoreText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 30);
			ScoreText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));

			ScoreText->m_transform->SetScale(glm::vec3(0.7f, 0.7f, 0.7f));
			ScoreText->m_transform->SetPosition(glm::vec3(100.0f, 50.0f, 1.0f));

			gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::GameOver].push_back(ScoreText);
			gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Highscore].push_back(ScoreText);

			ui_highscore->GetComponent<HighScoreUI>()->NameText = ScoreText;

			ScoreText = Instantiate();

			ScoreText->AddComponent<TextRenderer>();
			ScoreText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 30);
			ScoreText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));

			ScoreText->m_transform->SetScale(glm::vec3(0.7f, 0.7f, 0.7f));
			ScoreText->m_transform->SetPosition(glm::vec3(-100.0f, 50.0f, 1.0f));

			gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::GameOver].push_back(ScoreText);
			gamecontroller->GetComponent<UIController>()->UIGroups[UI_GROUP::Highscore].push_back(ScoreText);

			ui_highscore->GetComponent<HighScoreUI>()->ScoreText = ScoreText;
			


			
			gamecontroller->GetComponent<UIController>()->ScoreText = ui_ScoreText;
			gamecontroller->GetComponent<UIController>()->ComboText = ui_ComboText;
			gamecontroller->GetComponent<UIController>()->HPbar = ui_HPbar;
			gamecontroller->GetComponent<UIController>()->Staminabar = ui_StaminaBar;
			gamecontroller->GetComponent<UIController>()->HPText = ui_HPtext;
			gamecontroller->GetComponent<UIController>()->QueenHPText = ui_BossHPtext;
			gamecontroller->GetComponent<UIController>()->EquippedWeaponDisplay.push_back(ui_Weapongp);
			gamecontroller->GetComponent<UIController>()->EquippedArtifactDisplay.push_back(ui_Artifactgp1);
			gamecontroller->GetComponent<UIController>()->EquippedArtifactDisplay.push_back(ui_Artifactgp2);


			Bg2 = Instantiate().get();
			Bg2->AddComponent<MeshRenderer>();
			Bg2->GetComponent<MeshRenderer>()->SetLayer(-5);
			Bg2->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
			Bg2->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_Background_Layer2.png");
			Bg2->m_transform->SetScale(glm::vec3(Graphic::Window::GetWidth() * 2.0f, Graphic::Window::GetHeight() * 2.0f, 1));
			Bg2->m_transform->SetPosition(glm::vec3(0, -300, 3000));


			Bg1 = Instantiate().get();
			Bg1->AddComponent<MeshRenderer>();
			Bg1->GetComponent<MeshRenderer>()->SetLayer(-4);
			Bg1->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
			Bg1->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_Background_Layer1.png");
			Bg1->m_transform->SetScale(glm::vec3(Graphic::Window::GetWidth() * 2.0f, Graphic::Window::GetHeight() * 2.0f, 1));
			Bg1->m_transform->SetPosition(glm::vec3(0, -300, 2000));

			std::shared_ptr<GameObject> Bg0 = Instantiate();
			Bg0->AddComponent<MeshRenderer>();
			Bg0->GetComponent<MeshRenderer>()->SetLayer(-3);
			Bg0->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
			Bg0->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_Background_Layer1.png");
			Bg0->GetComponent<MeshRenderer>()->SetReplaceColor(glm::vec3(0.2, 0.2, 0.8));
			Bg0->m_transform->SetScale(glm::vec3(Graphic::Window::GetWidth() * -2.0f, Graphic::Window::GetHeight() * 1.5f, 1));
			Bg0->m_transform->SetPosition(glm::vec3(0, -500, 1000));

			std::shared_ptr<GameObject> Bg00 = Instantiate();
			Bg00->AddComponent<MeshRenderer>();
			Bg00->GetComponent<MeshRenderer>()->SetLayer(-2);
			Bg00->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
			Bg00->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_Background_Layer1.png");
			Bg00->GetComponent<MeshRenderer>()->SetReplaceColor(glm::vec3(0.8, 0.8, 0.8));
			Bg00->m_transform->SetScale(glm::vec3(Graphic::Window::GetWidth() * 2.0f, Graphic::Window::GetHeight() * 1.0f, 1));
			Bg00->m_transform->SetPosition(glm::vec3(0, -700, 50));


			//Player animation controller
			{
				//Add Animator
				std::shared_ptr<Animation> Idle = std::make_shared<Animation>();

				Idle->setStartPosition(0, 0);
				Idle->setEndPosition(6, 0);
				Idle->setSpeedMultiplier(1);

				std::shared_ptr<Animation> Running = std::make_shared<Animation>();

				Running->setStartPosition(0, 1);
				Running->setEndPosition(4, 1);
				Running->setSpeedMultiplier(2);

				std::shared_ptr<Animation> Dashing = std::make_shared<Animation>();

				Dashing->setStartPosition(0, 4);
				Dashing->setEndPosition(2, 4);
				Dashing->setSpeedMultiplier(1);

				std::shared_ptr<Animation> Jumping = std::make_shared<Animation>();

				Jumping->setStartPosition(0, 3);
				Jumping->setEndPosition(3, 3);
				Jumping->setSpeedMultiplier(1);

				std::shared_ptr<Animation> falling = std::make_shared<Animation>();

				falling->setStartPosition(3, 3);
				falling->setEndPosition(3, 3);
				falling->setSpeedMultiplier(1);

				RabbitController = std::make_shared<AnimationController>();
				RabbitController->setSheetSize(glm::vec2(7, 5));

				ENGINE_INFO("making rabbbit animcon");
				RabbitController->AddState(Idle, true);
				RabbitController->AddState(Running, true);
				RabbitController->AddState(Dashing, false);
				RabbitController->AddState(Jumping, false);
				RabbitController->AddState(falling, false);

				Serialization::SaveObject(*RabbitController, AnimationControllerPath("Player"));
			}
			//player
			{
				//Rabbit = Instantiate(PrefabPath("Player"));

				Rabbit = Instantiate();
				Rabbit->Layer = "Player";

				//Set Transform
				Rabbit->m_transform->SetScale(glm::vec3(CHAR_SIZE, CHAR_SIZE, 1));
				Rabbit->m_transform->SetPosition(glm::vec3(0.0f, 100.0f, 0.0f));

				Rabbit->AddComponent<MeshRenderer>();
				Rabbit->GetComponent<MeshRenderer>()->CreateMesh(7, 5);
				Rabbit->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_PlayerBody_Vversion03.png");
				Rabbit->GetComponent<MeshRenderer>()->SetLayer(0);

				Rabbit->AddComponent<Animator>();
				Rabbit->GetComponent<Animator>()->sr_controllerPath = AnimationControllerPath("Player");
				Rabbit->GetComponent<Animator>()->setFramePerSec(12);

				Rabbit->AddComponent<BoxCollider>();

				//Add Rigidbody
				Rabbit->AddComponent<Rigidbody>();
				Rabbit->GetComponent<Rigidbody>()->SetDrag(0.01f);

				Rabbit->GetComponent<BoxCollider>()->m_rigidbody = Rabbit->GetComponent<Rigidbody>();
				Rabbit->GetComponent<BoxCollider>()->ReSize(25, 25);

				//Behavior Script
				Rabbit->AddComponent<HPsystem>();
				Rabbit->GetComponent<HPsystem>()->SetMaxHP(100);
				Rabbit->AddComponent<PlayerController>();

				//platform->m_transform->SetParent(Rabbit->m_transform);

				Serialization::SaveObject(*Rabbit, PrefabPath("Player"));

				gamecontroller->GetComponent<GameController>()->player = Rabbit;
			}

			std::shared_ptr<GameObject> wp_MachineGun = Instantiate();
			wp_MachineGun->AddComponent<MeshRenderer>();
			wp_MachineGun->GetComponent<MeshRenderer>()->CreateMesh(4, 1);
			wp_MachineGun->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/machinegun_shoot.png");
			wp_MachineGun->GetComponent<MeshRenderer>()->SetLayer(3);

			wp_MachineGun->AddComponent<MachineGun>()->SetWeaponFireRate(7.0f);
			wp_MachineGun->m_transform->SetParent(Rabbit->m_transform);
			wp_MachineGun->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));
			
			wp_MachineGun->AddComponent<SoundPlayer>();

			Serialization::SaveObject(*wp_MachineGun, PrefabPath("Weapons/MachineGun"));

			std::shared_ptr<GameObject> wp_LaserGun = Instantiate();

			wp_LaserGun->AddComponent<MeshRenderer>();
			wp_LaserGun->GetComponent<MeshRenderer>()->CreateMesh(4, 1);
			wp_LaserGun->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/laser_sheet"));
			wp_LaserGun->GetComponent<MeshRenderer>()->SetLayer(3);

			wp_LaserGun->AddComponent<LaserGun>()->SetWeaponFireRate(1.0f);
			wp_LaserGun->m_transform->SetParent(Rabbit->m_transform);
			wp_LaserGun->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));

			std::shared_ptr<GameObject> laser = Instantiate();
			laser->AddComponent<MeshRenderer>();
			laser->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
			laser->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/laser_bullet_sheet"));

			wp_LaserGun->GetComponent<LaserGun>()->AssignLaserObj(laser);

			wp_LaserGun->AddComponent<SoundPlayer>();

			Serialization::SaveObject(*wp_LaserGun, PrefabPath("Weapons/LaserGun"));

			std::shared_ptr<GameObject> wp_GrenadeLauncher = Instantiate();

			wp_GrenadeLauncher->AddComponent<MeshRenderer>();
			wp_GrenadeLauncher->GetComponent<MeshRenderer>()->CreateMesh(4, 1);
			wp_GrenadeLauncher->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/grenadeLaucher_sheet"));
			wp_GrenadeLauncher->GetComponent<MeshRenderer>()->SetLayer(3);
			
			wp_GrenadeLauncher->AddComponent<GrenadeLauncher>()->SetWeaponFireRate(1.0f);
			wp_GrenadeLauncher->m_transform->SetParent(Rabbit->m_transform);
			wp_GrenadeLauncher->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));
			
			wp_GrenadeLauncher->AddComponent<SoundPlayer>();

			Serialization::SaveObject(*wp_GrenadeLauncher, PrefabPath("Weapons/GrenadeLauncher"));

			std::shared_ptr<GameObject> wp_ZapperGun = Instantiate();

			wp_ZapperGun->AddComponent<MeshRenderer>();
			wp_ZapperGun->GetComponent<MeshRenderer>()->CreateMesh(5, 1);
			wp_ZapperGun->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/zapper_sheet"));
			wp_ZapperGun->GetComponent<MeshRenderer>()->SetLayer(3);

			wp_ZapperGun->AddComponent<ZapperGun>()->SetWeaponFireRate(1.0f);
			wp_ZapperGun->m_transform->SetParent(Rabbit->m_transform);
			wp_ZapperGun->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));

			wp_ZapperGun->AddComponent<SoundPlayer>();

			Serialization::SaveObject(*wp_ZapperGun, PrefabPath("Weapons/ZapperGun"));

			std::shared_ptr<GameObject> wp_Blackhole = Instantiate();

			wp_Blackhole->AddComponent<MeshRenderer>();
			wp_Blackhole->GetComponent<MeshRenderer>()->CreateMesh(7, 1);
			wp_Blackhole->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Equipments/blackhole_sheet"));
			wp_Blackhole->GetComponent<MeshRenderer>()->SetLayer(3);
			
			wp_Blackhole->AddComponent<BlackholeGun>()->SetWeaponFireRate(1.0f);
			wp_Blackhole->m_transform->SetParent(Rabbit->m_transform);
			wp_Blackhole->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));

			Serialization::SaveObject(*wp_Blackhole, PrefabPath("Weapons/Blackhole"));
			
			wp_Blackhole->AddComponent<SoundPlayer>();

			gamecontroller->AddComponent<EquipmentManager>();
			gamecontroller->GetComponent<EquipmentManager>()->AssignWeaponToManager(wp_MachineGun);
			gamecontroller->GetComponent<EquipmentManager>()->AssignWeaponToManager(wp_LaserGun);
			gamecontroller->GetComponent<EquipmentManager>()->AssignWeaponToManager(wp_GrenadeLauncher);
			gamecontroller->GetComponent<EquipmentManager>()->AssignWeaponToManager(wp_ZapperGun);
			gamecontroller->GetComponent<EquipmentManager>()->AssignWeaponToManager(wp_Blackhole);


			gamecontroller->GetComponent<EquipmentManager>()->AssignArtifactToManager(std::make_shared<BulletAmplifier>());
			gamecontroller->GetComponent<EquipmentManager>()->AssignArtifactToManager(std::make_shared<FireRateUP>());
			gamecontroller->GetComponent<EquipmentManager>()->AssignArtifactToManager(std::make_shared<SpeedRunner>());
			gamecontroller->GetComponent<EquipmentManager>()->AssignArtifactToManager(std::make_shared<AttackUP>());
			gamecontroller->GetComponent<EquipmentManager>()->AssignArtifactToManager(std::make_shared<LowGravity>());
			gamecontroller->GetComponent<EquipmentManager>()->AssignArtifactToManager(std::make_shared<ArtifactAmplifier>());
			gamecontroller->GetComponent<EquipmentManager>()->AssignArtifactToManager(std::make_shared<CursedPendant>());

			gamecontroller->GetComponent<EquipmentManager>()->AssignPlayer(Rabbit);
			gamecontroller->GetComponent<EquipmentManager>()->InitData();

			//gamecontroller->GetComponent<EquipmentManager>()->AddPlayerWeapon(WEAPON_TYPE::WEAPON_MACHINEGUN);
			

			//test parent/child------------------------------------------------
			//Bg2->m_transform->SetParent(Rabbit->m_transform);

			//platform
			platform = Instantiate().get();
			platform->Layer = "Platform";
			platform->AddComponent<MeshRenderer>();
			platform->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
			platform->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/platform01.png");
			platform->GetComponent<MeshRenderer>()->SetLayer(3);
			platform->m_transform->SetScale(glm::vec3(800, 20, 1));
			//platform->m_transform->SetParent(Rabbit->m_transform);
			platform->AddComponent<BoxCollider>()->ReScale(1, 1);

			platform = Instantiate().get();
			platform->Layer = "Platform";
			platform->AddComponent<MeshRenderer>();
			platform->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
			platform->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/platform01.png");
			platform->GetComponent<MeshRenderer>()->SetLayer(3);
			platform->m_transform->SetScale(glm::vec3(800, 20, 1));
			platform->m_transform->SetPosition(glm::vec3(500, 500, 1));
			platform->AddComponent<BoxCollider>()->ReScale(1, 1);

			ENGINE_INFO("Enemy Creation ==========================================================");
			//flyer animation
			{
				EnemCon = std::make_shared<AnimationController>();

				std::shared_ptr<Animation> Fly = std::make_shared<Animation>();
				Fly->setStartPosition(0, 2);
				Fly->setEndPosition(5, 2);
				Fly->setSpeedMultiplier(3);

				std::shared_ptr<Animation> Dash = std::make_shared<Animation>();
				Dash->setStartPosition(0, 0);
				Dash->setEndPosition(3, 0);

				std::shared_ptr<Animation> Die = std::make_shared<Animation>();
				Die->setStartPosition(0, 1);
				Die->setEndPosition(4, 1);

				EnemCon->setSheetSize(glm::vec2(6, 3));

				EnemCon->AddState(Fly, true);
				EnemCon->AddState(Dash, false);
				EnemCon->AddState(Die, false);

				Serialization::SaveObject(*EnemCon, AnimationControllerPath("Flyer"));

			}
			//flyer
			{
				GameObject* flyer = Instantiate().get();
				flyer->Layer = "Enemy";
				flyer->m_transform->SetScale(glm::vec3(50, 50, 1));

				flyer->AddComponent<MeshRenderer>();
				flyer->GetComponent<MeshRenderer>()->SetLayer(1);
				flyer->GetComponent<MeshRenderer>()->CreateMesh(6, 3);
				flyer->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Characters/Enemy_Flyer"));

				flyer->AddComponent<Rigidbody>();
				flyer->GetComponent<Rigidbody>()->Init(15, 15);

				flyer->AddComponent<HPsystem>();
				flyer->AddComponent<SoundPlayer>();

				//og flyer
				flyer->AddComponent<AirFollowing>();
				flyer->AddComponent<Flyer>();
				flyer->GetComponent<Rigidbody>()->SetGravityScale(0.00000001f);

				flyer->AddComponent<Animator>();
				flyer->GetComponent<Animator>()->setFramePerSec(12);

				flyer->GetComponent<Animator>()->sr_controllerPath = AnimationControllerPath("Flyer");

				flyer->SetActive(false);

				Serialization::SaveObject(*flyer, PrefabPath("Flyer"));

			}

			//bomber anim
			{

				std::shared_ptr<Animation> BomberIdle = std::make_shared<Animation>();

				BomberIdle->setStartPosition(0, 0);
				BomberIdle->setEndPosition(2, 0);

				std::shared_ptr<Animation> BomberCharging = std::make_shared<Animation>();
				BomberCharging->setStartPosition(0, 1);
				BomberCharging->setEndPosition(1, 1);

				std::shared_ptr<Animation> BomberDashing = std::make_shared<Animation>();
				BomberDashing->setStartPosition(2, 1);
				BomberDashing->setEndPosition(3, 1);

				std::shared_ptr<Animation> BomberExplode = std::make_shared<Animation>();
				BomberExplode->setStartPosition(0, 3);
				BomberExplode->setEndPosition(11, 3);

				std::shared_ptr<Animation> BomberDie = std::make_shared<Animation>();
				BomberDie->setStartPosition(0, 2);
				BomberDie->setEndPosition(4, 2);

				std::shared_ptr<AnimationController> BomberAnimController = std::make_shared<AnimationController>();

				BomberAnimController->setSheetSize(glm::vec2(12, 4));
				BomberAnimController->AddState(BomberIdle, true);
				BomberAnimController->AddState(BomberCharging, false);
				BomberAnimController->AddState(BomberDashing, true);
				BomberAnimController->AddState(BomberExplode, false);
				BomberAnimController->AddState(BomberDie, false);

				Serialization::SaveObject(*BomberAnimController, AnimationControllerPath("Bomber"));

			}
			//bomber
			{

				GameObject* bomber = Instantiate().get();
				bomber->Layer = "Enemy";
				bomber->m_transform->SetScale(glm::vec3(50, 50, 1));

				bomber->AddComponent<MeshRenderer>();
				bomber->GetComponent<MeshRenderer>()->CreateMesh(12, 4);
				bomber->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_Enemy_Bomber_V[version01].png");

				bomber->AddComponent<Animator>();
				bomber->GetComponent<Animator>()->sr_controllerPath = AnimationControllerPath("Bomber");
				bomber->GetComponent<Animator>()->setFramePerSec(12);

				bomber->AddComponent<Rigidbody>();
				bomber->GetComponent<Rigidbody>()->Init(15, 15);
				bomber->GetComponent<Rigidbody>()->SetGravityScale(0.000001);

				bomber->AddComponent<HPsystem>();
				bomber->AddComponent<SoundPlayer>();
				bomber->AddComponent<AirFollowing>();
				bomber->AddComponent<AirDash>()->dashDamage = 0;
				bomber->AddComponent<Explosion>();
				bomber->AddComponent<Bomber>();

				bomber->SetActive(false);
				Serialization::SaveObject(*bomber, PrefabPath("Bomber"));

			}

			//Tank anim
			{
				std::shared_ptr<AnimationController> EnemCon = std::make_shared<AnimationController>();

				std::shared_ptr<Animation> Patrol = std::make_shared<Animation>();
				Patrol->setStartPosition(0, 0);
				Patrol->setEndPosition(6, 0);
				Patrol->setSpeedMultiplier(3);

				std::shared_ptr<Animation> Die = std::make_shared<Animation>();
				Die->setStartPosition(0, 1);
				Die->setEndPosition(3, 1);
				Die->setSpeedMultiplier(1);

				EnemCon->setSheetSize(glm::vec2(7, 2));

				EnemCon->AddState(Patrol, true);
				EnemCon->AddState(Die, false);

				Serialization::SaveObject(*EnemCon, AnimationControllerPath("Tank"));
			}
			//Tank
			{

				GameObject* flyer = Instantiate().get();
				flyer->Layer = "GroundEnemy";
				flyer->m_transform->SetScale(glm::vec3(200, 200, 1));

				flyer->AddComponent<MeshRenderer>();
				flyer->GetComponent<MeshRenderer>()->SetLayer(1);
				flyer->GetComponent<MeshRenderer>()->CreateMesh(7, 2);
				flyer->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Characters/Enemy_Tank"));

				flyer->AddComponent<Rigidbody>();
				flyer->AddComponent<BoxCollider>()->ReScale(0.5, 0.5);

				flyer->AddComponent<HPsystem>();
				flyer->AddComponent<SoundPlayer>();

				//tank
				flyer->AddComponent<GroundPatrol>()->SetSpeed(50.0f);
				flyer->AddComponent<Tank>();
				flyer->GetComponent<Rigidbody>()->SetGravityScale(1.0f);
				flyer->GetComponent<HPsystem>()->SetMaxHP(50.0);

				flyer->AddComponent<Animator>();
				flyer->GetComponent<Animator>()->setFramePerSec(12);

				flyer->GetComponent<Animator>()->sr_controllerPath = AnimationControllerPath("Tank");

				flyer->SetActive(false);

				Serialization::SaveObject(*flyer, PrefabPath("Tank"));

			}

			//charger anim
			{
				std::shared_ptr<AnimationController> EnemCon = std::make_shared<AnimationController>();

				std::shared_ptr<Animation> Patrol = std::make_shared<Animation>();
				Patrol->setStartPosition(0, 0);
				Patrol->setEndPosition(3, 0);
				Patrol->setSpeedMultiplier(1);

				std::shared_ptr<Animation> Charge = std::make_shared<Animation>();
				Charge->setStartPosition(0, 1);
				Charge->setEndPosition(1, 1);
				Charge->setSpeedMultiplier(1);

				std::shared_ptr<Animation> Dash = std::make_shared<Animation>();
				Dash->setStartPosition(0, 1);
				Dash->setEndPosition(3, 1);
				Dash->setSpeedMultiplier(1);

				std::shared_ptr<Animation> Die = std::make_shared<Animation>();
				Die->setStartPosition(0, 2);
				Die->setEndPosition(4, 2);
				Die->setSpeedMultiplier(1);

				EnemCon->setSheetSize(glm::vec2(5, 3));

				EnemCon->AddState(Patrol, true);
				EnemCon->AddState(Charge, false);
				EnemCon->AddState(Dash, true);
				EnemCon->AddState(Die, false);

				Serialization::SaveObject(*EnemCon, AnimationControllerPath("Charger"));
			}
			//Charger
			{

				GameObject* flyer = Instantiate().get();
				flyer->Layer = "GroundEnemy";
				flyer->m_transform->SetScale(glm::vec3(50, 50, 1));

				flyer->AddComponent<MeshRenderer>();
				flyer->GetComponent<MeshRenderer>()->SetLayer(1);
				flyer->GetComponent<MeshRenderer>()->CreateMesh(5, 3);
				flyer->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Characters/Enemy_Charger"));

				flyer->AddComponent<Rigidbody>();
				flyer->AddComponent<BoxCollider>()->ReScale(1, 1);

				flyer->AddComponent<HPsystem>();
				flyer->AddComponent<SoundPlayer>();

				//charger
				flyer->AddComponent<GroundPatrol>();
				flyer->AddComponent<GroundDash>();
				flyer->AddComponent<Charger>()->targetDetectionRange = 300.0f;
				flyer->GetComponent<Rigidbody>()->SetGravityScale(1.0f);

				flyer->AddComponent<Animator>();
				flyer->GetComponent<Animator>()->setFramePerSec(12);

				flyer->GetComponent<Animator>()->sr_controllerPath = AnimationControllerPath("Charger");

				flyer->SetActive(false);

				Serialization::SaveObject(*flyer, PrefabPath("Charger"));

			}

			//Spitter anim
			{
				std::shared_ptr<AnimationController> EnemCon = std::make_shared<AnimationController>();

				std::shared_ptr<Animation> Idle = std::make_shared<Animation>();
				Idle->setStartPosition(0, 1);
				Idle->setEndPosition(5, 1);
				Idle->setSpeedMultiplier(1);

				std::shared_ptr<Animation> Shoot = std::make_shared<Animation>();
				Shoot->setStartPosition(0, 2);
				Shoot->setEndPosition(4, 2);
				Shoot->setSpeedMultiplier(1);

				std::shared_ptr<Animation> Die = std::make_shared<Animation>();
				Die->setStartPosition(0, 0);
				Die->setEndPosition(3, 0);
				Die->setSpeedMultiplier(1);

				EnemCon->setSheetSize(glm::vec2(5, 8));

				EnemCon->AddState(Idle, true);
				EnemCon->AddState(Shoot, false);
				EnemCon->AddState(Die, false);

				Serialization::SaveObject(*EnemCon, AnimationControllerPath("Spitter"));
			}
			//Spitter
			{

				GameObject* flyer = Instantiate().get();
				flyer->Layer = "GroundEnemy";
				flyer->m_transform->SetScale(glm::vec3(100, 100, 1));

				flyer->AddComponent<MeshRenderer>();
				flyer->GetComponent<MeshRenderer>()->SetLayer(1);
				flyer->GetComponent<MeshRenderer>()->CreateMesh(5, 8);
				flyer->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Characters/Enemy_Spitter"));

				flyer->AddComponent<Rigidbody>();
				flyer->AddComponent<BoxCollider>()->ReScale(0.5, 0.5);

				flyer->AddComponent<HPsystem>();
				flyer->AddComponent<SoundPlayer>();

				//spitter
				flyer->AddComponent<GroundPatrol>();
				flyer->AddComponent<ObjectShooter>();
				flyer->GetComponent<ObjectShooter>()->BulletObjectTypeAsInt = POOL_TYPE::BULLET_FUME;
				flyer->GetComponent<ObjectShooter>()->bullet_speed = 500;
				flyer->AddComponent<Spitter>()->targetDetectionRange = 1000.0f;
				flyer->GetComponent<Rigidbody>()->SetGravityScale(1.0f);

				flyer->AddComponent<Animator>();
				flyer->GetComponent<Animator>()->setFramePerSec(12);

				flyer->GetComponent<Animator>()->sr_controllerPath = AnimationControllerPath("Spitter");

				flyer->SetActive(false);

				Serialization::SaveObject(*flyer, PrefabPath("Spitter"));

			}

			//Queen anim
			{
				std::shared_ptr<Animation> queenIdle = std::make_shared<Animation>();

				queenIdle->setStartPosition(0, 0);
				queenIdle->setEndPosition(7, 0);
				queenIdle->setSpeedMultiplier(1);

				std::shared_ptr<Animation> queenSpawning = std::make_shared<Animation>();

				queenSpawning->setStartPosition(0, 1);
				queenSpawning->setEndPosition(3, 1);
				queenSpawning->setSpeedMultiplier(1);

				queenAnimControl = std::make_shared<AnimationController>();
				queenAnimControl->setSheetSize(glm::vec2(7, 2));
				queenAnimControl->AddState(queenIdle, true);
				queenAnimControl->AddState(queenSpawning, false);

				Serialization::SaveObject(*queenAnimControl, AnimationControllerPath("Queen"));
			}
			//Queen
			{
				queen = Instantiate().get();
				queen->Layer = "Enemy";

				queen->m_transform->SetScale(glm::vec3(CHAR_SIZE * 10, CHAR_SIZE * 10, 1.0f));
				queen->m_transform->SetPosition(glm::vec3(-(Graphic::Window::GetWidth()), (Graphic::Window::GetHeight() * 2 / 3) + 700.0f, 1.0f));

				queen->AddComponent<MeshRenderer>();
				queen->GetComponent<MeshRenderer>()->CreateMesh(7, 2);
				queen->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Characters/Enemy_Queen"));

				queen->AddComponent<Animator>();
				queen->GetComponent<Animator>()->sr_controllerPath = AnimationControllerPath("Queen");
				queen->GetComponent<Animator>()->setFramePerSec(3);

				queen->AddComponent<Rigidbody>();
				queen->GetComponent<Rigidbody>()->Init(400, 300);
				queen->GetComponent<Rigidbody>()->SetGravityScale(0.00001);

				queen->AddComponent<HPsystem>();
				queen->AddComponent<SoundPlayer>();
				queen->AddComponent<AirPatrol>();
				queen->GetComponent<AirPatrol>()->SetPoint(-(Graphic::Window::GetWidth() * 2), Graphic::Window::GetWidth() * 2);
				queen->AddComponent<DeQueen>();

				queen->SetActive(false);

				Serialization::SaveObject(*queen, PrefabPath("Queen"));
			}

			//Cocoon
			{
				GameObject* cocoon = Instantiate().get();

				cocoon->Layer = "Enemy";
				cocoon->m_transform->SetScale(glm::vec3(100.0f, 100.0f, 1.0f));

				cocoon->AddComponent<MeshRenderer>();
				cocoon->GetComponent<MeshRenderer>()->SetLayer(1);
				cocoon->GetComponent<MeshRenderer>()->CreateMesh(5, 4);
				cocoon->GetComponent<MeshRenderer>()->SetTexture(TexturePath("Characters/Enemy_Cocoon"));

				cocoon->AddComponent<Rigidbody>();
				cocoon->AddComponent<BoxCollider>()->ReScale(1, 1);
				cocoon->GetComponent<Rigidbody>()->SetGravityScale(0.00001);
				cocoon->AddComponent<Enemy>();

				cocoon->AddComponent<HPsystem>();
				cocoon->GetComponent<HPsystem>()->SetMaxHP(10.0f);

				cocoon->SetActive(false);

				Serialization::SaveObject(*cocoon, PrefabPath("Cocoon"));
			}


			ENGINE_INFO("Example Particle Creation ===============================================");
			//Bullet Hit Example
			{
				ParticleSystem* particle = new ParticleSystem();
				particle->texturePath = "Sources/Assets/yellow_square.png";
				particle->emitter->isEnabled = true;
				particle->emitter->constantParticle = false;
				particle->emitter->burstParticleNumber = 3;
				particle->emitter->particleSamples = 3;
				particle->emitter->minEmissionAngle = 135;
				particle->emitter->maxEmissionAngle = 225;
				particle->emitter->spawnRadius = 0.1f;
				particle->velocity->minSpeed = 300.0f;
				particle->velocity->maxSpeed = 300.0f;
				particle->velocity->sr_directionTypeAsInt = 0;
				particle->velocity->gravityScale = 2;
				particle->lifetime->minLifeTime = 0.2f;
				particle->lifetime->maxLifeTime = 0.2f;
				particle->shape->minXSize = 4.5f;
				particle->shape->maxXSize = 4.5f;
				particle->shape->minYSize = 4.5f;
				particle->shape->maxYSize = 4.5f;
				particle->color->usingLifeTimeModifier = true;
				particle->color->InterpolationEnd = 0.5f;
				particle->color->Color_Start = glm::vec3(1, 0, 0);
				particle->color->Color_End = glm::vec3(1, 1, 1);

				Serialization::SaveObject(*particle, ParticlePath("Bullet_Hit_Example"));
			}

			//Bullet Fume
			{
				ParticleSystem* particle = new ParticleSystem();
				particle->texturePath = "Sources/Assets/white_square.png";
				particle->emitter->isEnabled = true;
				particle->emitter->spawnRadius = 50;
				particle->emitter->particleSamples = 10;
				particle->emitter->particleRate = 0.0f;
				particle->color->Color = glm::vec3(0.1f, 1.0f, 0.0f);
				particle->velocity->SetDirectiontype(ParticleSystem::DirectionType::AwayFromSpawn);
				particle->velocity->minSpeed = 10;
				particle->velocity->maxSpeed = 30;

				Serialization::SaveObject(*particle, ParticlePath("Bullet_Fume_Traverse"));
			}

			ENGINE_INFO("Bullet Creation =========================================================");
			//machinegun bullet
			{
				GameObject* Bullet = Instantiate().get();
				Bullet->Layer = "Bullet";
				Bullet->AddComponent<MeshRenderer>();
				Bullet->GetComponent<MeshRenderer>()->SetLayer(2);
				Bullet->GetComponent<MeshRenderer>()->CreateMesh(4, 1);
				Bullet->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/machinegun_bullet.png");

				Bullet->AddComponent<Rigidbody>();
				Bullet->GetComponent<Rigidbody>()->Init(7, 7);
				Bullet->GetComponent<Rigidbody>()->SetGravityScale(0.0000001f);

				Bullet->AddComponent<SoundPlayer>();

				Bullet->AddComponent<ParticleSystem>();
				Serialization::LoadObject(*Bullet->GetComponent<ParticleSystem>(), ParticlePath("Bullet_Hit_Example"));

				Bullet->AddComponent<MachineGunBullet>();

				Bullet->m_transform->SetScale(glm::vec3(10, 10, 1));

				Bullet->SetActive(false);

				Serialization::SaveObject(*Bullet, PrefabPath("Bullet_MG"));
			}

			//grenade bullet
			{
				GameObject* Bullet = Instantiate().get();
				Bullet->Layer = "Bullet";
				Bullet->AddComponent<MeshRenderer>();
				Bullet->GetComponent<MeshRenderer>()->CreateMesh(2, 1);
				Bullet->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/grenadeL_bullet.png");

				Bullet->AddComponent<Rigidbody>();
				Bullet->GetComponent<Rigidbody>()->Init(7, 7);
				Bullet->GetComponent<Rigidbody>()->SetGravityScale(1.0f);

				Bullet->AddComponent<SoundPlayer>();

				Bullet->m_transform->SetScale(glm::vec3(30, 30, 1));

				Bullet->AddComponent<GrenadeLauncherBullet>();

				Bullet->SetActive(false);

				Serialization::SaveObject(*Bullet, PrefabPath("Bullet_GL"));
			}

			//zapper bullet
			{
				GameObject* Bullet = Instantiate().get();
				Bullet->Layer = "Bullet";
				Bullet->AddComponent<MeshRenderer>();
				Bullet->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				Bullet->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/blue.jpg");

				Bullet->AddComponent<Rigidbody>();
				Bullet->GetComponent<Rigidbody>()->Init(7, 7);
				Bullet->GetComponent<Rigidbody>()->SetGravityScale(0.0f);

				Bullet->AddComponent<SoundPlayer>();

				Bullet->m_transform->SetScale(glm::vec3(10, 10, 1));

				Bullet->AddComponent<ZapperGunBullet>();

				Bullet->SetActive(false);

				Serialization::SaveObject(*Bullet, PrefabPath("Bullet_ZP"));
			}

			//blackhole bullet
			{
				GameObject* Bullet = Instantiate().get();
				Bullet->Layer = "Bullet";
				Bullet->AddComponent<MeshRenderer>();
				Bullet->GetComponent<MeshRenderer>()->CreateMesh(5, 1);
				Bullet->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/blackhole_bullet.png");

				Bullet->AddComponent<Rigidbody>();
				Bullet->GetComponent<Rigidbody>()->Init(7, 7);
				Bullet->GetComponent<Rigidbody>()->SetGravityScale(0.0000001f);

				Bullet->AddComponent<SoundPlayer>();

				Bullet->AddComponent<ParticleSystem>();
				Bullet->GetComponent<ParticleSystem>()->emitter->isEnabled = true;

				Bullet->m_transform->SetScale(glm::vec3(30, 30, 1));

				Bullet->AddComponent<BlackholeGunBullet>();

				Bullet->SetActive(false);

				Serialization::SaveObject(*Bullet, PrefabPath("Bullet_BH"));
			}

			//fume bullet
			{
				GameObject* Bullet = Instantiate().get();
				Bullet->Layer = "EnemyBullet";

				Bullet->m_transform->SetScale(glm::vec3(10, 10, 1));


				Bullet->AddComponent<MeshRenderer>();
				Bullet->GetComponent<MeshRenderer>()->CreateMesh(5, 1);
				Bullet->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/blackhole_bullet.png");
				Bullet->GetComponent<MeshRenderer>()->SetReplaceColor(glm::vec3(50.0 / 255.0, 168.0 / 255.0, 82.0 / 255.0));

				Bullet->AddComponent<Rigidbody>();
				//Bullet->GetComponent<Rigidbody>()->Init(7, 7);
				Bullet->GetComponent<Rigidbody>()->SetGravityScale(0.0000001f);

				Bullet->AddComponent<BoxCollider>()->ReScale(1, 1);
				//add fume bullet component

				Bullet->AddComponent<ParticleSystem>();
				Serialization::LoadObject(*Bullet->GetComponent<ParticleSystem>(), ParticlePath("Bullet_Fume_Traverse"));

				Bullet->SetActive(false);

				Serialization::SaveObject(*Bullet, PrefabPath("Bullet_Fume"));
			}

			{
				GameObject* Item = Instantiate().get();

				Item->Layer = "Item";
				Item->m_transform->SetScale(glm::vec3(50, 50, 1));

				Item->AddComponent<MeshRenderer>();
				Item->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
				Item->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/White.jpg");

				Item->AddComponent<Rigidbody>();
				Item->GetComponent<Rigidbody>()->SetGravityScale(0.25f);
				Item->AddComponent<BoxCollider>()->ReScale(1, 1);

				Item->AddComponent<ItemDrop>();

				Item->SetActive(false);

				Serialization::SaveObject(*Item, PrefabPath("ItemDrop"));
			}


			//Add Sound
			Bg2->AddComponent<SoundPlayer>();
			Bg2->GetComponent<SoundPlayer>()->CreateSoundPlayer();
			Bg2->GetComponent<SoundPlayer>()->SetSound("Sources/Assets/BGMPrototype.mp3");
			Bg2->GetComponent<SoundPlayer>()->SetLoop(true);
			Bg2->GetComponent<SoundPlayer>()->SetVolume(0.5);
			//Bg2->GetComponent<SoundPlayer>()->PlaySound();


			GAME_INFO(*Rabbit);
			
			Serialization::SaveObject(*(SceneManagement::ActiveScene), ScenePath("SerializationTest"));
			//this point is where it all ends =========================================
		}

		SceneManagement::ActiveScene->Init();

	}

	void FixedUpdate(float dt)
	{

		//Bg1->m_transform->SetPosition(Graphic::getCamera()->m_position * parlx1);
		//Bg2->m_transform->SetPosition(Graphic::getCamera()->m_position * parlx2);

		//Update Physics Scene
		static float accumulator = 0.0f;
		accumulator += dt;

		while (accumulator >= c_targetDT)
		{
			//Update Physic
			g_physicScene->Update(c_targetDT);

			//Update Components
			FactoryCollection::FixedUpdateComponents(c_targetDT);

			accumulator -= c_targetDT;

			g_physicScene->SendCollisionMsg();
		}

	}

	void Update(float dt)
	{
		//Update Sound
		//Bg2->GetComponent<SoundPlayer>()->UpdateVolume();

		//Update All Systems
		//Update Input
		Input::Update();
		mouseCursor->m_transform->SetPosition(glm::vec3(Input::GetMouseScreenPosition(), 0));

		//Test Only
		//DebugInput(dt);

		FactoryCollection::UpdateComponents(dt);

		//ENGINE_INFO("FPS: {}", (1/dt));
		//Update Graphic
		Graphic::Render();

	}

	void Loop(void)
	{
		while (!g_gameInfo->m_shouldClose)
		{

			g_gameInfo->StartFrame();

			FixedUpdate(g_gameInfo->m_deltaTime);
			Update(g_gameInfo->m_deltaTime);


			if (Input::GetKeyDown(Input::KeyCode::KEY_ESCAPE))
			{
				g_gameInfo->GameShouldClose();
				Graphic::Window::SetWindowShouldClose(true);
			}

			g_gameInfo->EndFrame();
		}
	}

	void Terminate(void)
	{
		//Terminate All System
		//Terminate Game

		//Terminate Input
		Input::Terminate();
		//Terminate Graphic
		Graphic::Terminate();
		//Terminate Physics
		delete g_physicScene;
		//Terminate Game Info
		ENGINE_WARN("Engine Terminated");
	}

}
