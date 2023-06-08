#pragma once
#include "Component.h"
#include "Scene.h"

namespace dae
{

	class ValuesComponent final : public Component
	{

	public:
		ValuesComponent(Scene* scene) : m_Scene{scene} {};
		~ValuesComponent() override;
		ValuesComponent(const ValuesComponent&) = delete;
		ValuesComponent(ValuesComponent&&) noexcept = delete;
		ValuesComponent& operator=(const ValuesComponent&) = delete;
		ValuesComponent& operator=(ValuesComponent&&) noexcept = delete;

		//Core
		void Update();
		virtual void FixedUpdate() override;
		void Render() const;

		//Getter
		int GetLives() const;
		int GetScores() const;
		int GetShotsFired() { return NrOfShotsFired; };
		int GetNumberOfHits() { return NrOfHits; };
		int GetMissRatio();

		//Setter
		void SetLives(int lives);
		void SetName(std::string name) { m_ComponentName = name; };

		//Public functions
		void Damage();
		void IncreaseScore(int score);
		void ResetObserver();
		void IncreaseHits() { NrOfHits++; };
		void IncreaseShots() { NrOfShotsFired++; };

		void Reset();



	private:
		int m_Lives{ 3 };
		int m_Score{ 0 };
		std::string m_ComponentName;

		Scene* m_Scene;
		int NrOfShotsFired{ 0 }, NrOfHits{ 0 };
	};
}

