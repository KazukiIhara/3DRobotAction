#pragma once

// C++
#include <cstdint>
#include <limits>
#include <random>

class BossMech;

class BossAI {
public:
	struct Param {
		// 行動決定の更新間隔
		float thinkIntervalSec = 0.20f;
		// 行動を選んだ直後のクールダウン
		float commitAfterSelectSec = 0.15f;
		// 同点のときのランダム性
		float randomJitter = 0.05f;

		// 距離しきい値
		float nearDist = 20.0f;
		float farDist = 30.0f;
	};

public:
	explicit BossAI(BossMech* mech);
	~BossAI() = default;

	void Update(bool isActive, bool isPause);
	void SetParam(const Param& param);

private:
	enum class Action {
		Idle,
		LaserShot,
		LaserBladeSlash,
	};

	struct ActionScore {
		Action action = Action::Idle;
		float score = -std::numeric_limits<float>::infinity();
	};

private:
	// 思考タイマー更新
	bool UpdateThinkTimer();
	// 行動候補の中から1つ選ぶ
	Action SelectAction();

	// 距離評価
	float CalcDistanceToPilot() const;

	// 各行動のスコア
	float ScoreIdle(float dist) const;
	float ScoreLaserShot(float dist) const;
	float ScoreLaserBladeSlash(float dist) const;

	// ステートへ反映
	void ApplyAction(Action action);

	// ランダム微調整
	float Jitter();

private:
	BossMech* mech_ = nullptr;

	Param param_{};

	float thinkTimerSec_ = 0.0f;
	float commitTimerSec_ = 0.0f;

	std::mt19937 rng_{ 0x1234567u };
	mutable std::uniform_real_distribution<float> dist01_{ 0.0f, 1.0f };
};
