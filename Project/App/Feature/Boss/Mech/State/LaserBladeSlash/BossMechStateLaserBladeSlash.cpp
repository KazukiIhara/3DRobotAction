#include "BossMechStateLaserBladeSlash.h"


#include <algorithm>
#include <memory>
#include <vector>

#include "MAGI.h"

#include "Feature/Boss/Mech/BossMech.h"

#include "Feature/Boss/Mech/State/PhaseSys/IBossMechStatePhase.h"

using namespace Magi;

// フェーズクラス
namespace {

	/// <summary>
	/// 攻撃準備
	/// </summary>
	class StartUp final: public IBossMechStatePhase {
	public:
		void Enter([[maybe_unused]] BossMech* mech) override {
			
		}

		void Update([[maybe_unused]] BossMech* mech) override {}

		void Exit([[maybe_unused]] BossMech* mech) override {}

		bool EndRequest() override {
			return end_;
		}

	private:
		bool end_ = false;
	};

	class Attack1 final: public IBossMechStatePhase {
	public:
		void Enter([[maybe_unused]] BossMech* mech) override {}

		void Update([[maybe_unused]] BossMech* mech) override {}

		void Exit([[maybe_unused]] BossMech* mech) override {}

		bool EndRequest() override {
			return end_;
		}

	private:
		bool end_ = false;
	};

	class Attack2 final: public IBossMechStatePhase {
	public:
		void Enter([[maybe_unused]] BossMech* mech) override {}

		void Update([[maybe_unused]] BossMech* mech) override {}

		void Exit([[maybe_unused]] BossMech* mech) override {}

		bool EndRequest() override {
			return end_;
		}

	private:
		bool end_ = false;
	};

	class EndLag final: public IBossMechStatePhase {
	public:
		void Enter([[maybe_unused]] BossMech* mech) override {}

		void Update([[maybe_unused]] BossMech* mech) override {}

		void Exit([[maybe_unused]] BossMech* mech) override {}

		bool EndRequest() override {
			return end_;
		}

	private:
		bool end_ = false;
	};

}


void BossMechStateLaserBladeSlash::SetupPhases() {
	// Phase登録
	runner_->RegisterFactory("StartUp", []() { return std::make_unique<StartUp>(); });
	runner_->RegisterFactory("Attack1", []() { return std::make_unique<Attack1>(); });
	runner_->RegisterFactory("Attack2", []() { return std::make_unique<Attack2>(); });
	runner_->RegisterFactory("EndLag", []() { return std::make_unique<EndLag>(); });

	// 実行順番登録
	runner_->SetSequence({ "StartUp","Attack1","Attack2","EndLag" });
}

void BossMechStateLaserBladeSlash::Enter([[maybe_unused]] BossMech* mech) {
	// Runner作成
	runner_ = std::make_unique<BossMechStatePhaseRunner>();

	// Phase登録
	SetupPhases();

	// 開始
	runner_->Start(mech);
}

void BossMechStateLaserBladeSlash::Update([[maybe_unused]] BossMech* mech) {
	// Phase更新
	runner_->Update(mech);

	// 終了で待機へ
	if (!runner_->IsActive()) {
		mech->ChangeState(BossMech::State::Idle);
	}
}

void BossMechStateLaserBladeSlash::Exit([[maybe_unused]] BossMech* mech) {
	// Runner停止
	runner_->Stop(mech);
	runner_.reset();
}