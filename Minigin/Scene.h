#pragma once
#include "SceneManager.h"
#include <SDL.h>

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void Initialize();
		void PostInitialize();
		void Update();
		void Render() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		std::shared_ptr<GameObject> GetGameObject(std::string name);

		std::vector<std::shared_ptr<GameObject>> GetGameObjects(std::string name, bool isCompleteWord = true);
		std::vector<std::shared_ptr<GameObject>>& GetGameObjects();

		std::vector<GameObject*> GetOverlappingObjects(GameObject* objectToOverlap, std::string id = "", std::string holderName = "", bool isCompleteWord = true, bool sourceHasDimensions = false, bool targetsHaveDimensions = true);
		bool IsOverlap(const SDL_Rect& square1, const SDL_Rect& square2);

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_objects{ std::vector<std::shared_ptr<GameObject>>() };

		static unsigned int m_idCounter; 
	};

}
