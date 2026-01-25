#pragma once

// C++
#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Feature/Boss/Mech/State/PhaseSys/IBossMechStatePhase.h"

class BossMech;

class BossMechStatePhaseRunner {
public:
	BossMechStatePhaseRunner() = default;
	~BossMechStatePhaseRunner() = default;

	// Phase 登録（キー -> 生成関数）
	bool RegisterFactory(const std::string& key, std::function<std::unique_ptr<IBossMechStatePhase>()> factory);

	// 実行順（同じキーを複数回入れてOK）
	void SetSequence(const std::vector<std::string>& keys);

	// 開始（sequence_ の先頭から実行）
	bool Start(BossMech* mech);

	// 更新（EndRequestで次のPhaseへ）
	void Update(BossMech* mech);

	// 強制停止（現在PhaseのExit呼んで破棄）
	void Stop(BossMech* mech);

	// 状態
	bool IsActive() const { return isActive_; }
	const std::string& GetCurrentKey() const { return currentKey_; }

private:
	bool BeginNextPhase(BossMech* mech);

private:
	std::unordered_map<std::string, std::function<std::unique_ptr<IBossMechStatePhase>()>> factories_;

	std::deque<std::string> sequence_;

	bool isActive_ = false;
	std::string currentKey_;
	std::unique_ptr<IBossMechStatePhase> currentPhase_;
};
