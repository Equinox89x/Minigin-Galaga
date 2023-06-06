#pragma once
#include <vector>
#include "Observer.h"


namespace dae
{
	class Observer;
	class GameObject;
	enum class Event;

	class Callback final
	{
	public:
		Callback() = default;
		~Callback();

		Callback(const Callback&) = delete;
		Callback(Callback&&) noexcept = delete;
		Callback& operator= (const Callback&) = delete;
		Callback& operator= (const Callback&&) noexcept = delete;

		void AddObserver(Observer* const observer);
		void RemoveObserver(Observer* const observer);

		void Notify(GameObject* go, Event event);

	private:
		std::vector<Observer*> m_pObservers;

	};
}

