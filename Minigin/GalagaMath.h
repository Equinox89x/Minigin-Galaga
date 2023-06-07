#pragma once
#include <SDL_rect.h>
#include <glm/ext/vector_float2.hpp>
#include <vector>

namespace GalagaMath {

	inline float Square(float val) {
		return val * val;
	}

	inline int BinomialCoeff(int n, int i) {
		if (i == 0 || i == n)
			return 1;

		int numerator = 1;
		int denominator = 1;

		for (int j = 1; j <= i; j++) {
			numerator *= (n - j + 1);
			denominator *= j;
		}

		return numerator / denominator;
	}

	inline glm::vec2 CalculateBezierPoint(float t, const std::vector<glm::vec2>& controlPoints) {
		int n{ static_cast<int>(controlPoints.size()) - 1 };
		float x{ 0.0 };
		float y{ 0.0 };

		for (int i = 0; i <= n; i++) {
			float blend{ static_cast<float>(BinomialCoeff(n, i) * pow(1 - t, n - i) * pow(t, i)) };
			x += controlPoints[i].x * blend;
			y += controlPoints[i].y * blend;
		}

		return { x, y };
	}

	inline bool IsOverlapping(const SDL_Rect& source, const SDL_Rect& target) {
		// Check if the squares overlap on the x-axis
		bool doesXOverlap = (source.x < target.x + target.w) && (source.x + source.w > target.x);

		// Check if the squares overlap on the y-axis
		bool doesYOverlap = (source.y < target.y + target.h) && (source.y + source.h > target.y);

		// Return true if there is overlap on both axes
		return doesXOverlap && doesYOverlap;
	}
}