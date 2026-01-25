#include "BossMechStatePhaseRunner.h"

#include "Feature/Boss/Mech/State/PhaseSys/IBossMechStatePhase.h"

bool BossMechStatePhaseRunner::RegisterFactory(
	const std::string& key,
	std::function<std::unique_ptr<IBossMechStatePhase>()> factory
) {
	if (key.empty() || !factory) {
		return false;
	}

	factories_[key] = std::move(factory);
	return true;
}

void BossMechStatePhaseRunner::SetSequence(const std::vector<std::string>& keys) {
	sequence_.clear();
	for (const auto& k : keys) {
		sequence_.push_back(k);
	}
}

bool BossMechStatePhaseRunner::Start(BossMech* mech) {
	if (!mech) {
		return false;
	}

	// 既に動いてたら一旦止める
	Stop(mech);

	// シーケンスが空なら動けない
	if (sequence_.empty()) {
		isActive_ = false;
		return false;
	}

	isActive_ = true;

	// 先頭を開始
	if (!BeginNextPhase(mech)) {
		isActive_ = false;
		return false;
	}

	return true;
}

void BossMechStatePhaseRunner::Update(BossMech* mech) {
	if (!isActive_ || !mech) {
		return;
	}
	if (!currentPhase_) {
		// currentが無いなら終了
		if (!BeginNextPhase(mech)) {
			isActive_ = false;
		}
		return;
	}

	// 更新
	currentPhase_->Update(mech);

	// 終了要求が無ければ継続
	if (!currentPhase_->EndRequest()) {
		return;
	}

	// Exitして破棄
	currentPhase_->Exit(mech);
	currentPhase_.reset();
	currentKey_.clear();

	// 次へ（無ければ停止）
	if (!BeginNextPhase(mech)) {
		isActive_ = false;
	}
}

void BossMechStatePhaseRunner::Stop(BossMech* mech) {
	if (!mech) {
		isActive_ = false;
		currentKey_.clear();
		currentPhase_.reset();
		return;
	}

	if (currentPhase_) {
		// 停止時Exit
		currentPhase_->Exit(mech);
	}

	isActive_ = false;
	currentKey_.clear();
	currentPhase_.reset();
}

bool BossMechStatePhaseRunner::BeginNextPhase(BossMech* mech) {
	if (!mech) {
		return false;
	}

	// もう残ってないなら終了
	if (sequence_.empty()) {
		return false;
	}

	// 次キー
	const std::string nextKey = sequence_.front();
	sequence_.pop_front();

	// Factoryを探す
	auto it = factories_.find(nextKey);
	if (it == factories_.end()) {
		// 未登録キーはスキップして次へ
		return BeginNextPhase(mech);
	}

	// 生成
	currentPhase_ = it->second();
	currentKey_ = nextKey;

	// Enter
	currentPhase_->Enter(mech);

	return true;
}
