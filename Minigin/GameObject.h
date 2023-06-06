#pragma once
#include <string>
#include <memory>
#include <vector>
#include <SDL.h>
#include <glm/ext/vector_float3.hpp>

namespace dae
{
	class Texture2D;
	class Component;
	class TransformComponent;

	class GameObject final
	{
	public:

		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void Initialize();
		virtual void Update();
		virtual void Render() const;

		void MarkForDestroy() { MarkedForDelete = true; };
		bool IsMarkedForDestroy() { return MarkedForDelete; };
		void EnableCollision(bool isEnabled) { CollisionEnabled = isEnabled; };
		bool IsCollisionEnabled() { return CollisionEnabled; };

		template<typename Comp>
		std::enable_if_t<std::is_base_of_v<Component, Comp>, Comp*>
		AddComponent(Comp* comp) {
			if (comp) {
				m_pComponents.push_back(comp);
				comp->m_pGameObject = this;
				dynamic_cast<Component*>(comp)->Initialize();
				return comp;
			}
			return nullptr;
		}

		template<typename Comp>
		Comp* GetComponent() {
			const type_info& ti = typeid(Comp);
			for (auto* component : m_pComponents)
			{
				if (component && typeid(*component) == ti)
					return static_cast<Comp*>(component);
			}
			return nullptr;
		}
		
		template<typename Comp>
		Comp* GetComponent(std::string name) {
			const type_info& ti = typeid(Comp);
			for (auto* component : m_pComponents)
			{
				if (component && typeid(*component) == ti && component->GetName() == name)
					return static_cast<Comp*>(component);
			}
			return nullptr;
		}

		void RemoveComponent(Component* comp);

		void SetParent(GameObject* const parent);
		GameObject* GetParent() const;
		void RemoveChild(GameObject* pObject);
		void AddChild(GameObject* const go);
		void SetName(std::string name) { m_ObjectName = name; };
		std::string GetName() { return m_ObjectName; };

		TransformComponent* GetTransform() const { return m_pTransform; };
		std::vector<GameObject*>& GetChildren() { return m_pChildren; };
		GameObject* GetChild(std::string name);
		std::vector<GameObject*> GetChildren(std::string name);


		void SetIsHidden(bool isHidden);

	private:
		std::vector<GameObject*> m_pChildren{ std::vector<GameObject*>() };
		std::vector<Component*> m_pComponents{ std::vector<Component*>() };
		GameObject* m_pParent{ nullptr };
		TransformComponent* m_pTransform{};
		std::string m_ObjectName{};
		bool m_IsHidden{ false };
		bool MarkedForDelete{ false }, NeedsUpdate{ true }, CollisionEnabled{ true };
	};
}