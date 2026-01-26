#include "BossMechStateLaserShot.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "MAGI.h"

#include "Feature/Boss/Mech/BossMech.h"

#include "Feature/Boss/Mech/State/PhaseSys/IBossMechStatePhase.h"

using namespace Magi;

namespace {

	/// <summary>
	/// 攻撃準備
	/// </summary>
	class PhaseStartUp final: public IBossMechStatePhase {
	public:
		void Enter([[maybe_unused]] BossMech* mech) override {
			// タイマー初期化
			const float timeStartUp = MAGISYSTEM::GetParameterValue<float>({ "BossMechStateParam","LaserShot","TimeStartUp" });
			timer_ = timeStartUp;

			mech->GetAnimator()->PlayAnimation("BossLaserShot_StartUp", timeStartUp, 0.0f, EasingType::EaseInOutCubic);
			end_ = false;
		}

		void Update([[maybe_unused]] BossMech* mech) override {
			// dt取得
			const float dt = MAGISYSTEM::GetDeltaTime();

			// タイマー更新
			timer_ -= dt;
			timer_ = std::max(0.0f, timer_);

			// 終了判定
			if (timer_ == 0.0f) {
				end_ = true;
			}
		}

		void Exit([[maybe_unused]] BossMech* mech) override {

		}

		bool EndRequest() override {
			return end_;
		}

	private:
		float timer_ = 0.0f;
		bool end_ = false;
	};


	/// <summary>
	/// 射撃
	/// </summary>
	class PhaseShot final: public IBossMechStatePhase {
	public:
		void Enter(BossMech* mech) override {
			const float timeShot = MAGISYSTEM::GetParameterValue<float>({ "BossMechStateParam","LaserShot","TimeShot" });
			timer_ = timeShot;
			mech->GetAnimator()->PlayAnimation("BossLaserShot_Shot", timeShot, 0.1f, EasingType::EaseInOutCubic);
		}

		void Update([[maybe_unused]] BossMech* mech) override {
			// dt取得
			const float dt = MAGISYSTEM::GetDeltaTime();
			// タイマー更新
			timer_ -= dt;
			timer_ = std::max(0.0f, timer_);

			// 終了判定
			if (timer_ == 0.0f) {
				end_ = true;
			}
		}

		void Exit([[maybe_unused]] BossMech* mech) override {
			// 攻撃
			mech->GetWeapon("LaserGun")->Attack();
		}

		bool EndRequest() override {
			return end_;
		}

	private:
		float timer_ = 0.0f;
		bool end_ = false;
	};

	/// <summary>
	/// 終了
	/// </summary>
	class PhaseEndLag final: public IBossMechStatePhase {
	public:
		void Enter([[maybe_unused]] BossMech* mech) override {
			// タイマー初期化
			const float timeEndLag = MAGISYSTEM::GetParameterValue<float>({ "BossMechStateParam","LaserShot","TimeEndLag" });
			mech->GetAnimator()->PlayAnimation("BossLaserShot_EndLag", timeEndLag, 0.1f, EasingType::EaseInOutCubic);
			timer_ = timeEndLag;
			end_ = false;

		}

		void Update([[maybe_unused]] BossMech* mech) override {
			// dt取得
			const float dt = MAGISYSTEM::GetDeltaTime();

			// タイマー更新
			timer_ -= dt;
			timer_ = std::max(0.0f, timer_);

			// 終了判定
			if (timer_ == 0.0f) {
				end_ = true;
			}
		}

		void Exit([[maybe_unused]] BossMech* mech) override {}

		bool EndRequest() override {
			return end_;
		}

	private:
		float timer_ = 0.0f;
		bool end_ = false;
	};

}

void BossMechStateLaserShot::SetupPhases() {

	// Phase登録
	runner_->RegisterFactory("StartUp", []() { return std::make_unique<PhaseStartUp>(); });
	runner_->RegisterFactory("Shot", []() { return std::make_unique<PhaseShot>(); });
	runner_->RegisterFactory("EndLag", []() { return std::make_unique<PhaseEndLag>(); });

	// 実行順
	runner_->SetSequence({ "StartUp", "Shot", "EndLag" });
}

void BossMechStateLaserShot::Enter(BossMech* mech) {
	// Runner作成
	runner_ = std::make_unique<BossMechStatePhaseRunner>();

	// Phase登録
	SetupPhases();

	// 開始
	runner_->Start(mech);
}

void BossMechStateLaserShot::Update(BossMech* mech) {
	// Phase更新
	runner_->Update(mech);

	// 終了で待機へ
	if (!runner_->IsActive()) {
		mech->ChangeState(BossMech::State::Idle);
	}
}

void BossMechStateLaserShot::Exit(BossMech* mech) {

	// Runner停止
	runner_->Stop(mech);
	runner_.reset();
}
