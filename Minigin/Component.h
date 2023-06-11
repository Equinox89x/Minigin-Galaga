#pragma once
#include "GameObject.h"

namespace dae
{
	class Callback;
	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Initialize() {};
		virtual void PostInitialize() {};
		virtual void FixedUpdate() {};
		virtual void Update() {};
		virtual void Render() const {};

		GameObject* GetGameObject() const { return m_pGameObject; };
		std::string GetName() { return Name; };
		void SetName(std::string name) { Name = name; };

		void SetCallback(Callback* const subject);

	protected:
		GameObject* m_pGameObject{};
		Callback* m_pCallback{};

	private:
		friend class GameObject;
		std::string Name{ "" };
	};
}
