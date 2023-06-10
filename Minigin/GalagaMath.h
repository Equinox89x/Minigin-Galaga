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

	inline glm::vec2 RandomCurvedPath(const glm::vec2& start, const glm::vec2& end, float t) {
		// Randomize the control point between start and end
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(0.2f, 3.8f);
		float controlX = start.x + dis(gen) * (end.x - start.x);
		float controlY = start.y + dis(gen) * (end.y - start.y);

		// Compute the curved path using quadratic Bezier interpolation
		float oneMinusT = 1.0f - t;
		float oneMinusTSquared = oneMinusT * oneMinusT;
		float tSquared = t * t;

		float x = oneMinusTSquared * start.x + 2.0f * oneMinusT * t * controlX + tSquared * end.x;
		float y = oneMinusTSquared * start.y + 2.0f * oneMinusT * t * controlY + tSquared * end.y;

		return { x, y };
	}

	inline glm::vec3 sphericalRand(float radius)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

		float x = dist(gen);
		float y = dist(gen);
		float z = dist(gen);

		// Normalize the vector
		float length = std::sqrt(x * x + y * y + z * z);
		x /= length;
		y /= length;
		z /= length;

		// Scale the vector by the radius
		x *= radius;
		y *= radius;
		z *= radius;

		return glm::vec3(x, y, z);
	}

	// Easing function to create an S-shaped curve
	inline float easeInOutSine(float t)
	{
		return 0.5f * (1.0f - std::cosf(t * 3.14f));
	}

	// Move a point from a to b in a continuous S-shaped path
	inline glm::vec2 movePointSPath(const glm::vec2& a, const glm::vec2& b, float t)
	{
		// Apply easing function to the interpolation parameter
		float easedT = easeInOutSine(t);

		// Calculate the position based on the eased interpolation parameter
		glm::vec2 position = a + (b - a) * easedT;

		return position;
	}

	inline glm::vec2 movePointSwirling(const glm::vec2& a, const glm::vec2& b, float t, float size)
	{
		// Calculate the angle based on the interpolation parameter
		float angle = t * 2.0f * 3.14f;

		// Calculate the position along the S-shaped swirling path
		float x = std::sin(angle) * size;
		float y = std::sin(angle * 2.0f) * size;

		glm::vec2 position = a + ((b - a) * t) + glm::vec2(x, y);

		return position;
	}
}