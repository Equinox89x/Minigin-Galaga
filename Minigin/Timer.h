#pragma once
#include <SDL.h>
#include "Singleton.h"

namespace dae {

    class Timer : public Singleton<Timer>
    {
	public:
		void Init();
		void Update();
		void Start();
		void Stop();


		float GetDeltaTime() const { return static_cast<float>(m_ElapsedTime); };
		uint32_t GetFPS() const { return m_FPS; };
		float GetTotal() const { return m_TotalTime; };


	private:
		friend class Singleton<Timer>;
		Timer() = default;

		uint64_t m_BaseTime = 0;
		uint64_t m_PausedTime = 0;
		uint64_t m_StopTime = 0;
		uint64_t m_PreviousTime = 0;
		uint64_t m_CurrentTime = 0;
		uint64_t m_DeltaTime = 0;

		uint32_t m_FPS = 0;
		float m_dFPS = 0.0f;
		uint32_t m_FPSCount = 0;

		float m_TotalTime = 0.0f;
		float m_ElapsedTime = 0.0f;
		float m_SecondsPerCount = 0.0f;
		float m_ElapsedUpperBound = 0.03f;
		float m_FPSTimer = 0.0f;

		bool m_IsStopped = true;
		bool m_ForceElapsedUpperBound = false;
	};
}