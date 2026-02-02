#include "BossMechStateLaserBladeSlash.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "MAGI.h"

#include "Feature/Boss/Mech/BossMech.h"
#include "Feature/Boss/Mech/Weapon/LaserBlade/BossMechWeaponLaserBlade.h"
#include "Feature/Boss/Mech/State/PhaseSys/IBossMechStatePhase.h"

#include "Feature/Effect/BossAttackWarning/BossAttackWarningEffect.h"
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"

using namespace Magi;

// フェーズクラス
namespace {

	/// <summary>
	/// 攻撃準備
	/// </summary>
	class StartUp final: public IBossMechStatePhase {
	public:
		void Enter([[maybe_unused]] BossMech* mech) override {
			// タイマー初期化
			const float time = MAGISYSTEM::GetParameterValue<float>({ "BossMechStateParam","LaserBladeSlash","TimeStartUp" });
			timer_ = time;

			mech->GetAnimator()->PlayAnimation("BossLaserBladeSlash_StartUp", time, 0.1f, EasingType::EaseInOutCubic);

			// ブレードの設定
			auto blade = dynamic_cast<BossMechWeaponLaserBlade*>(mech->GetWeapon("LaserBlade"));
			blade->SetBladeLength(0.0f);
			const float innerRad = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Boss","LaserBlade","InnerRadius" });
			const float outerRad = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Boss","LaserBlade","OuterRadius" });
			blade->SetInnerRadius(innerRad);
			blade->SetOuterRadius(outerRad);

			// プレイヤーの位置に向かう処理
			const Vector3 targetPos = mech->GetTargetWorldPos();
			const Vector3 curPos = mech->GetCenterPos();
			const Vector3 dir = targetPos - curPos;
			const float len = Length(dir);
			const Vector3 dirN = Normalize(dir);
			auto ms = mech->GetMoveSystem();
			ms->SetDir(dirN);
			ms->SetSpeed(len);
			ms->SetMaxSpeed(len);

			// 攻撃発生前エフェクト
			const Vector3 headPos = mech->GetPartsTransform(MechAnimation::TransType::Head)->GetWorldPosition();
			effect_ = mech->GetGameEffectManager()->Add(std::move(std::make_unique<BossAttackWarningEffect>(headPos, mech)));

			end_ = false;
		}

		void Update([[maybe_unused]] BossMech* mech) override {
			// dt取得
			const float dt = MAGISYSTEM::GetDeltaTime();

			const float time = MAGISYSTEM::GetParameterValue<float>({ "BossMechStateParam","LaserBladeSlash","TimeStartUp" });

			if (timer_ <= time * 0.5f) {
				auto blade = dynamic_cast<BossMechWeaponLaserBlade*>(mech->GetWeapon("LaserBlade"));
				const float curLen = blade->GetLength();
				const float tarLen = MAGISYSTEM::GetParameterValue<float>({ "WeaponParam","Boss","LaserBlade","InnerLength" });
				const float bladeT = CalExpT(dt, time * 0.5f, 1.0f);
				const float len = Lerp(curLen, tarLen, bladeT);
				blade->SetBladeLength(len);
			}

			// タイマー更新
			timer_ -= dt;
			timer_ = std::max(0.0f, timer_);

			// 終了判定
			if (timer_ == 0.0f) {
				end_ = true;
			}
		}

		void Exit([[maybe_unused]] BossMech* mech) override {
			auto ms = mech->GetMoveSystem();
			ms->Reset();
		}

		bool EndRequest() override {
			return end_;
		}

	private:
		float timer_ = 0.0f;
		bool end_ = false;
		BaseGameEffect* effect_ = nullptr;
	};

	class Attack1 final: public IBossMechStatePhase {
	public:
		void Enter([[maybe_unused]] BossMech* mech) override {
			// タイマー初期化
			const float time = MAGISYSTEM::GetParameterValue<float>({ "BossMechStateParam","LaserBladeSlash","TimeAttack1" });
			timer_ = time;

			mech->GetAnimator()->PlayAnimation("BossLaserBladeSlash_Attack1", time, 0.1f, EasingType::EaseInOutCubic);

			// 攻撃オブジェクト追加
			mech->GetWeapon("LaserBlade")->Attack(Damage::Power::Mid);

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
			// プレイヤーの位置に向かう処理
			const Vector3 targetPos = mech->GetTargetWorldPos();
			const Vector3 curPos = mech->GetCenterPos();
			const Vector3 dir = targetPos - curPos;
			const float len = Length(dir);
			const Vector3 dirN = Normalize(dir);
			auto ms = mech->GetMoveSystem();
			ms->SetDir(dirN);
			ms->SetSpeed(len);
			ms->SetMaxSpeed(len);

			// 攻撃発生前エフェクト
			const Vector3 headPos = mech->GetPartsTransform(MechAnimation::TransType::Head)->GetWorldPosition();
			effect_ = mech->GetGameEffectManager()->Add(std::move(std::make_unique<BossAttackWarningEffect>(headPos, mech)));
		}

		bool EndRequest() override {
			return end_;
		}

	private:
		float timer_ = 0.0f;
		bool end_ = false;
		BaseGameEffect* effect_ = nullptr;
	};

	class Attack2 final: public IBossMechStatePhase {
	public:
		void Enter([[maybe_unused]] BossMech* mech) override {
			// タイマー初期化
			const float time = MAGISYSTEM::GetParameterValue<float>({ "BossMechStateParam","LaserBladeSlash","TimeAttack2" });
			timer_ = time;

			mech->GetAnimator()->PlayAnimation("BossLaserBladeSlash_Attack2", time, 0.1f, EasingType::EaseInOutCubic);

			mech->GetWeapon("LaserBlade")->Attack(Damage::Power::Large);


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
			auto ms = mech->GetMoveSystem();
			ms->Reset();
		}

		bool EndRequest() override {
			return end_;
		}

	private:
		float timer_ = 0.0f;
		bool end_ = false;
	};

	class EndLag final: public IBossMechStatePhase {
	public:
		void Enter([[maybe_unused]] BossMech* mech) override {
			// タイマー初期化
			const float time = MAGISYSTEM::GetParameterValue<float>({ "BossMechStateParam","LaserBladeSlash","TimeEndLag" });
			timer_ = time;
			end_ = false;

		}

		void Update([[maybe_unused]] BossMech* mech) override {
			// dt取得
			const float dt = MAGISYSTEM::GetDeltaTime();

			const float time = MAGISYSTEM::GetParameterValue<float>({ "BossMechStateParam","LaserBladeSlash","TimeEndLag" });

			auto blade = dynamic_cast<BossMechWeaponLaserBlade*>(mech->GetWeapon("LaserBlade"));
			const float curInnerRad = blade->GetInnerRad();
			const float curOuterRad = blade->GetOuterRad();
			const float bladeT = CalExpT(dt, time, 1.0f);
			const float innerRad = Lerp(curInnerRad, 0.0f, bladeT);
			const float outerRad = Lerp(curOuterRad, 0.0f, bladeT);
			blade->SetInnerRadius(innerRad);
			blade->SetOuterRadius(outerRad);

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