#include "TransformComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

dae::TransformComponent::TransformComponent():
	m_Position{ 0, 0, 0 },
	m_WorldPosition{ 0, 0, 0 },
	m_Rotation{ 0, 0, 0 },
	m_WorldRotation{ 0, 0, 0 }
{}

void dae::TransformComponent::UpdateTransforms()
{
	if (const GameObject* parent{ GetGameObject()->GetParent() }) {
		auto pos{ parent->GetTransform()->GetPosition() };
		auto rot{ parent->GetTransform()->GetRotation() };
		m_WorldRotation = rot + GetRotation();
		m_WorldPosition = pos + GetPosition();
		//m_Rotation += m_WorldRotation;
		//m_Position += m_WorldPosition;
	}
	else{
		m_Rotation += m_WorldRotation;
		m_Position += m_WorldPosition;
	}


	//// Define the rotation angle in radians
	//float angle = glm::radians(45.0f);

	//// Create a rotation matrix using glm::rotate
	//glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

	//// Apply the rotation to the initial vector
	//m_Position = rotationMatrix * glm::vec4(1,1,1, 1.0f);



	m_IsDirty = false;
}

bool dae::TransformComponent::CheckIfDirty()
{
	//If Parent is dirty == update required (spatial relation)
	if (const GameObject* pParent = GetGameObject()->GetParent())
	{
		if (pParent->GetTransform()->IsDirty())
		{
			return true;
		}
	}

	return false;
}

void dae::TransformComponent::Update()
{
	m_IsDirty = CheckIfDirty();

	if (m_IsDirty)
		UpdateTransforms();
	
}

void dae::TransformComponent::Translate(glm::vec3 pos) { Translate(pos.x, pos.y, pos.z); }
void dae::TransformComponent::Translate(glm::vec2 pos) { Translate(pos.x, pos.y, 0); }
void dae::TransformComponent::Translate(float x, float y) { Translate(x, y, 0); };
void dae::TransformComponent::Translate(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
	UpdateTransforms();
}

void dae::TransformComponent::SetScale(glm::vec3 pos) { SetScale(pos.x, pos.y, pos.z); }
void dae::TransformComponent::SetScale(glm::vec2 pos) { SetScale(pos.x, pos.y, 1); }
void dae::TransformComponent::SetScale(float x, float y) { SetScale(x, y, 1); };
void dae::TransformComponent::SetScale(float scale) { SetScale(scale, scale, scale); };
void dae::TransformComponent::SetScale(float x, float y, float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}

void dae::TransformComponent::TranslateWorld(glm::vec3 pos){ TranslateWorld(pos.x, pos.y, pos.z); }
void dae::TransformComponent::TranslateWorld(glm::vec2 pos){ TranslateWorld(pos.x, pos.y, 0); }
void dae::TransformComponent::TranslateWorld(float x, float y) { TranslateWorld(x, y, 0); };
void dae::TransformComponent::TranslateWorld(float x, float y, float z)
{
	m_WorldPosition.x = x;
	m_WorldPosition.y = y;
	m_WorldPosition.z = z;
	UpdateTransforms();
}

void dae::TransformComponent::AddTranslate(glm::vec3 pos){ AddTranslate(pos.x, pos.y, pos.z); }
void dae::TransformComponent::AddTranslate(glm::vec2 pos){ AddTranslate(pos.x, pos.y, 0); }
void dae::TransformComponent::AddTranslate(float x, float y) { AddTranslate(x, y, 0); };
void dae::TransformComponent::AddTranslate(float x, float y, float z)
{
	m_Position.x += x;
	m_Position.y += y;
	m_Position.z += z;
	m_IsDirty = true;
	UpdateTransforms();
}

void dae::TransformComponent::AddTranslateWorld(glm::vec3 pos){ AddTranslateWorld(pos.x, pos.y, pos.z); }
void dae::TransformComponent::AddTranslateWorld(glm::vec2 pos){ AddTranslateWorld(pos.x, pos.y, 0); }
void dae::TransformComponent::AddTranslateWorld(float x, float y) { AddTranslateWorld(x, y, 0); }
void dae::TransformComponent::AddTranslateWorld(float x, float y, float z)
{
	m_WorldPosition.x += x;
	m_WorldPosition.y += y;
	m_WorldPosition.z += z;
	UpdateTransforms();
}

void dae::TransformComponent::Rotate(const float angle) { Rotate(0, angle, 0); };
void dae::TransformComponent::Rotate(float x, float y, float z)
{
	m_Rotation.x = x;
	m_Rotation.y = y;
	m_Rotation.z = z;

	// Define the rotation angle in radians
	float angle = glm::radians(y);

	// Create a rotation matrix using glm::rotate
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1.f, 0));

	// Apply the rotation to the initial vector
	glm::vec3 rotatedVector = rotationMatrix * glm::vec4(1,1,1, 1);
	m_Position += rotatedVector;
	UpdateTransforms();
}

const glm::vec3 dae::TransformComponent::GetWorldPosition()
{
	glm::vec3 pos{ m_Position };
	if (m_pGameObject) {
		if (m_pGameObject->GetParent())
			pos += m_pGameObject->GetParent()->GetTransform()->GetWorldPosition();
	}
	return pos;
}

const glm::vec3 dae::TransformComponent::GetWorldRotation() const
{
	glm::vec3 rot{ m_Rotation };
	if (m_pGameObject) {
		if (m_pGameObject->GetParent())
			rot += m_pGameObject->GetParent()->GetTransform()->GetWorldRotation();
	}
	return rot;
}


glm::vec3 dae::TransformComponent::Rotate(const float cx, const float cy, float angle, glm::vec3 point, bool isDegrees)
{
	if(isDegrees) {
		angle = glm::radians(angle);
	}
	float s = sinf(angle);
	float c = cosf(angle);

	// translate point back to origin:
	point.x -= cx;
	point.y -= cy;

	// rotate point
	float xnew = point.x * c - point.y * s;
	float ynew = point.x * s + point.y * c;

	// translate point back:
	point.x = xnew + cx;
	point.y = ynew + cy;
	return point;
}
