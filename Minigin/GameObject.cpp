#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "TextureComponent.h"

dae::GameObject::GameObject() : 
	m_pTransform(new TransformComponent())
{
	AddComponent(m_pTransform);
}

dae::GameObject::~GameObject() {
	for (auto comp : m_pComponents) {
		delete comp;
	}
	m_pComponents.clear();
};

void dae::GameObject::PostInitialize(){
	for (Component* comp : m_pComponents) {
		comp->PostInitialize();
	}

	for (GameObject* child : m_pChildren) {
		child->PostInitialize();
	}
}

void dae::GameObject::Initialize()
{
	for (Component* comp : m_pComponents) {
		comp->Initialize();
	}

	for (GameObject* child : m_pChildren) {
		child->Initialize();
	}
}

void dae::GameObject::Update(){
	if (!m_IsHidden) {
		for (size_t i = 0; i < m_pChildren.size(); i++)
		{
			if (m_pChildren[i]->IsMarkedForDestroy()) {
				RemoveChild(m_pChildren[i]);
				break;
			}
		}

		for (Component* comp : m_pComponents) {
			comp->Update();
		}

		for (GameObject* child : m_pChildren) {
			child->Update();
		}
	}
}

void dae::GameObject::Render() const
{
	if(!m_IsHidden){
		for (const Component* comp : m_pComponents) {
			comp->Render();
		}

		for (GameObject* child : m_pChildren) {
			child->Render();
		}
	}
}

#pragma region Component
void dae::GameObject::RemoveComponent(Component* comp)
{
	if (comp) {
		auto it = find(m_pComponents.begin(), m_pComponents.end(), comp);
		m_pComponents.erase(it);
		delete comp;
		comp = nullptr;
	}
}


void dae::GameObject::SetParent(GameObject* const parent)
{

	//Remove itself as a child from the previous parent(if any).
	if (parent->m_pParent) {
		auto it = find(parent->m_pParent->m_pChildren.begin(), parent->m_pParent->m_pChildren.end(), parent);
		parent->m_pParent->m_pChildren.erase(it);
	}
	//Set the given parent on itself.
	parent->m_pParent = this;
	//Add itself as a child to the given parent.
	m_pChildren.push_back(parent);
	//Update position, rotationand scale
	m_pTransform->UpdateTransforms();

}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pParent;
}
#pragma endregion


void dae::GameObject::RemoveChild(GameObject* pObject)
{
	//Remove the given child from the children list
	auto it = find(m_pChildren.begin(), m_pChildren.end(), pObject);
	m_pChildren.erase(it);
	//	• Remove itself as a parent of the child.
	pObject->m_pParent = nullptr;
	//	• Update position, rotationand scale
	m_pTransform->UpdateTransforms();
}

void dae::GameObject::AddChild(GameObject* const go)
{
	// Remove the given child from the child's previous parent
	//go->m_pParent->
	//Set itself as parent of the child
	go->m_pParent = this;
	//Add the child to its children list.
	m_pChildren.push_back(go);
	//Update position, rotationand scale
	go->GetTransform()->UpdateTransforms();

	go->Initialize();
	go->PostInitialize();
}

dae::GameObject* dae::GameObject::GetChild(std::string name) {
	for (auto obj : m_pChildren)
	{
		if (obj && obj->GetName() == name)
			return obj;
	}
	return nullptr;
}

std::vector<dae::GameObject*> dae::GameObject::GetChildren(std::string name)
{
	//if(m_pChildren.size() <= 0)return std::vector<dae::GameObject*>();
	//if (m_pChildren.empty()) return std::vector<dae::GameObject*>();
	std::vector<GameObject*> children;

	for (auto obj : m_pChildren)
	{
		if (obj && obj->GetName() == name)
			children.push_back(obj);
	}
	return children;
}

void dae::GameObject::SetIsHidden(bool isHidden)
{
	m_IsHidden = isHidden;
	for (auto child : m_pChildren) {
		child->SetIsHidden(isHidden);
	}
}