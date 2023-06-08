#pragma once
#include "Component.h"
using namespace dae;

class EndScreenComponent final : public Component
{
public:
	EndScreenComponent() = default;
	~EndScreenComponent() = default;
	EndScreenComponent(const EndScreenComponent&) = delete;
	EndScreenComponent(EndScreenComponent&&) noexcept = delete;
	EndScreenComponent& operator=(const EndScreenComponent&) = delete;
	EndScreenComponent& operator=(EndScreenComponent&&) noexcept = delete;

	//void Initialize() override;
	void Update() override;

private:
	float EndTimer{ 5 };

};
