#pragma once
#include <string>
#include <memory>
#include "TextObjectComponent.h"

namespace dae
{
	class Font;
	class Texture2D;
	class FPSCounterComponent final : public TextObjectComponent
	{
	public:
		void Update() override;
		virtual void FixedUpdate() override;
		void Render() const override;

		void SetPosition(float x, float y);

		FPSCounterComponent(const std::string& text, std::shared_ptr<Font> font);
		~FPSCounterComponent();
		FPSCounterComponent(const FPSCounterComponent& other) = delete;
		FPSCounterComponent(FPSCounterComponent&& other) = delete;
		FPSCounterComponent& operator=(const FPSCounterComponent& other) = delete;
		FPSCounterComponent& operator=(FPSCounterComponent&& other) = delete;

	private:
		uint32_t m_prevFPS;

	};
}