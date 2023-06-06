#include "Scene.h"
#include "GameObject.h"
#include "TextureComponent.h"

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
	for (auto& object : m_objects)
	{
		if (object->IsMarkedForDestroy()) {
			Remove(object);
			break;
		}
	}

	for(auto& object : m_objects)
	{
		object->Update();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

std::vector<dae::GameObject*> Scene::GetOverlappingObjects(GameObject* objectToOverlap, std::string id, bool sourceHasDimensions, bool targetsHaveDimensions)
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

		if (IsOverlap(overlapRect, rect)) {
			children.push_back(obj.get());
		}
	}
	return children;
}

bool Scene::IsOverlap(const SDL_Rect& square1, const SDL_Rect& square2) {
	// Check if the squares overlap on the x-axis
	bool xOverlap = (square1.x < square2.x + square2.w) && (square1.x + square1.w > square2.x);

	// Check if the squares overlap on the y-axis
	bool yOverlap = (square1.y < square2.y + square2.h) && (square1.y + square1.h > square2.y);

	// Return true if there is overlap on both axes
	return xOverlap && yOverlap;
}