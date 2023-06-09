#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include <list>
using namespace dae;

	class ShootComponent final : public Component
	{

	public:
		ShootComponent(dae::Scene* scene, int id = 0, bool isVersus = false, bool isOpposer = false, bool isEnemy = false) : 
			m_Scene{ scene },
			Id{ id },
			StrId{ std::to_string(id) }, 
			IsVersus{ isVersus }, 
			IsOpposer{ isOpposer } ,
			IsEnemy{ isEnemy }
		{};
		~ShootComponent() = default;
		ShootComponent(const ShootComponent&) = delete;
		ShootComponent(ShootComponent&&) noexcept = delete;
		ShootComponent& operator=(const ShootComponent&) = delete;
		ShootComponent& operator=(ShootComponent&&) noexcept = delete;

		virtual void Initialize() override;
		virtual void FixedUpdate() override;
		virtual void Update() override;
		void HandleOpposerOverlap(std::shared_ptr<dae::GameObject>& bullet);
		void HandlePlayerOverlap(std::shared_ptr<dae::GameObject>& bullet);
		void HandleEnemyOverlap(std::shared_ptr<dae::GameObject>& bullet);
		virtual void Render() const override;

		void Shoot();
		void CreateBullet(std::string fileName, std::string name, std::string extention);

		//std::vector<GameObject*> GetOverlappingObjects(std::string name, bool sourceHasDimensions = false, bool targetsHaveDimensions = true);
		//bool IsOverlap(const SDL_Rect& square1, const SDL_Rect& square2);


	private:
		//std::list<std::shared_ptr<GameObject>> m_Bullets{};
		int m_MaxBullets{ 2 };
		int m_MoveSpeed{ 800 };
		Scene* m_Scene;
		std::shared_ptr<GameObject> Bullets{};

		int Id{0};
		std::string StrId;

		bool IsVersus{ false }, IsOpposer{ false }, IsEnemy{ false };
	};


