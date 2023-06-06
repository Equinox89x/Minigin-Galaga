#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include <list>
using namespace dae;

	class ShootComponent final : public Component
	{

	public:
		ShootComponent(dae::Scene& scene) : m_Scene{scene} {};
		~ShootComponent() = default;
		ShootComponent(const ShootComponent&) = delete;
		ShootComponent(ShootComponent&&) noexcept = delete;
		ShootComponent& operator=(const ShootComponent&) = delete;
		ShootComponent& operator=(ShootComponent&&) noexcept = delete;

		virtual void Initialize() override;
		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void Render() const override;

		void Shoot();

		float GetMissRatio() { return (NrOfHits / NrOfShotsFired) * 100.f; };
		//std::vector<GameObject*> GetOverlappingObjects(std::string name, bool sourceHasDimensions = false, bool targetsHaveDimensions = true);
		//bool IsOverlap(const SDL_Rect& square1, const SDL_Rect& square2);


	private:
		//std::list<std::shared_ptr<GameObject>> m_Bullets{};
		int m_MaxBullets{ 2 };
		int m_MoveSpeed{ 800 };
		Scene& m_Scene;
		std::shared_ptr<GameObject> Bullets{};

		int NrOfShotsFired{ 0 }, NrOfHits{ 0 };
	};


