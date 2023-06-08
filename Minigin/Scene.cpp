#include "Scene.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "GalagaMath.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

std::shared_ptr<GameObject> Scene::GetGameObject(std::string name) {
	for (const std::shared_ptr<GameObject>& obj : m_objects)
	{
		if (obj && obj->GetName() == name)
			return obj;
	}
	return nullptr;
}

std::vector<std::shared_ptr<GameObject>> Scene::GetGameObjects(std::string name)
{
	std::vector<std::shared_ptr<GameObject>> children;

	for (const std::shared_ptr<GameObject>& obj : m_objects)
	{
		if (obj && obj->GetName() == name)
			children.push_back(obj);
	}
	return children;
}

std::vector<std::shared_ptr<GameObject>>& dae::Scene::GetGameObjects()
{
	return m_objects;
}

Scene::Scene(const std::string& name) : m_name(name) {
}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	object->Initialize();
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
 	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void dae::Scene::Initialize()
{
	for (auto& object : m_objects)
	{
		object->Initialize();
	}
}

void Scene::Update()
{
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		if (m_objects[i]->IsMarkedForDestroy()) {
			Remove(m_objects[i]);
			i--;
		}
	}
	/*for (auto& object : m_objects)
	{
		if (object->IsMarkedForDestroy()) {
			Remove(object);
			break;
		}
	}*/
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		m_objects[i]->Update();
	}
	//for(auto& object : m_objects)
	//{
	//	object->Update();
	//}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

std::vector<dae::GameObject*> Scene::GetOverlappingObjects(GameObject* objectToOverlap, std::string id, std::string holderName, bool sourceHasDimensions, bool targetsHaveDimensions)
{
	SDL_Rect rect{};
	if (sourceHasDimensions) {
		rect = objectToOverlap->GetComponent<TextureComponent>()->GetRect();
	}
	else {
		auto pos{ objectToOverlap->GetTransform()->GetPosition() };
		rect = { static_cast<int>(pos.x), static_cast<int>(pos.y) ,1,1 };
	}

	std::vector<dae::GameObject*> children;

	if (holderName != "") {
		for (auto obj : GetGameObject(holderName)->GetChildren(id)) {
			if (!obj->IsCollisionEnabled()) continue;
			auto overlapPos{ obj->GetTransform()->GetPosition() };
			SDL_Rect overlapRect;
			if (targetsHaveDimensions) {
				overlapRect = obj->GetComponent<TextureComponent>()->GetRect();
			}
			else {
				overlapRect = { static_cast<int>(overlapPos.x), static_cast<int>(overlapPos.y) ,1,1 };
			}

			if (GalagaMath::IsOverlapping(overlapRect, rect)) {
				children.push_back(obj);
			}
		}
	}
	else {
		for (auto obj : GetGameObjects(id)) {
			if (!obj->IsCollisionEnabled()) continue;
			auto overlapPos{ obj->GetTransform()->GetPosition() };
			SDL_Rect overlapRect;
			if (targetsHaveDimensions) {
				overlapRect = obj->GetComponent<TextureComponent>()->GetRect();
			}
			else {
				overlapRect = { static_cast<int>(overlapPos.x), static_cast<int>(overlapPos.y) ,1,1 };
			}

			if (GalagaMath::IsOverlapping(overlapRect, rect)) {
				children.push_back(obj.get());
			}
		}
	}
	return children;
}