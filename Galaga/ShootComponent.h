#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include <list>
using namespace dae;

	class ShootComponent final : public Component
	{

	public:
		ShootComponent(dae::Scene* scene, int id = 0) : m_Scene{ scene }, Id{ id }, StrId{ std::to_string(id) } {};
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
	};


