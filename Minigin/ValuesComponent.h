#pragma once
#include "Callback.h"
#include "Component.h"

namespace dae
{

	class ValuesComponent final : public Component
	{

	public:
		ValuesComponent() : m_pCallback{} {};
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

		//Setter
		void SetCallback(Callback* const subject);
		void SetLives(int lives);
		void SetName(std::string name) { m_ComponentName = name; };

		//Public functions
		void Damage();
		void IncreaseScore(int score);
		void ResetObserver();

		void Reset();

	private:
		int m_Lives{ 3 };
		int m_Score{ 0 };
		std::string m_ComponentName;

		Callback* m_pCallback;
	};
}

