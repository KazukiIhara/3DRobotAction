#pragma once

// Windows
#include <windows.h>

// C++
#include <cstdint>

namespace Magi {
	class DeltaTimer {
	public:
		DeltaTimer();
		~DeltaTimer();

		void Reset();

		void Update();
		void EndFrame();

		float GetDeltaTime()const;
		float GetRawDeltaTime() const;

		void SetMultiplier(float mul);
	private:
		// デルタタイム
		float deltaTime_;
		int64_t lastTime_;
		double frequency_;

		float multiplier_ = 1.0f;
		float tempMultiplier_ = 1.0f;
		bool multiplierChanged_ = false;
	};
}