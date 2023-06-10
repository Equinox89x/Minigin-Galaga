#pragma once
#include <SDL_rect.h>
#include <glm/glm.hpp>
//#include <glm/gtc/random.hpp>
#include <glm/ext/vector_float2.hpp>
#include <vector>
#include <cmath>
#include <random>


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

	inline glm::vec2 MoveRectTowards(SDL_Rect& rect1, const SDL_Rect& rect2, float speed)
	{
		// Calculate the center points of the rectangles
		int rect1CenterX = rect1.x + rect1.w / 2;
		int rect1CenterY = rect1.y + rect1.h / 2;
		int rect2CenterX = rect2.x + rect2.w / 2;
		int rect2CenterY = rect2.y + rect2.h / 2;

		// Calculate the distance between the center points
		float distanceX = static_cast<float>(rect2CenterX - rect1CenterX);
		float distanceY = static_cast<float>(rect2CenterY - rect1CenterY);

		// Calculate the magnitude of the distance
		float distanceMagnitude = std::sqrt(distanceX * distanceX + distanceY * distanceY);

		// Calculate the normalized direction vector
		float directionX = distanceX / distanceMagnitude;
		float directionY = distanceY / distanceMagnitude;

		// Calculate the movement based on the speed
		float movementX = directionX * speed;
		float movementY = directionY * speed;

		return glm::vec2{ movementX, movementY };
	}
}