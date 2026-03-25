#include "PilotMechStateDodge.h"

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/GameInputSystem/GameInputSystem.h"

#include "MAGI.h"
#include "Math/Utility/MathUtility.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace Magi;
using namespace MAGIMath;
using namespace MAGIUtility;

void PilotMechStateDodge::Enter(PilotMech* mech) {
	// タイマー
	const float time = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Dodge","Time" });
	// 初速
	const float firstSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Dodge","FirstSpeed" });

	// ステートのタイマーセット
	timer_ = time;

	// 移動システムを取得
	auto ms = mech->GetMoveSystem();
	// 加速量を0にする
	ms->SetAcc(0.0f);
	// 初速と最大速度をセット
	ms->SetSpeed(firstSpeed);
	ms->SetMaxSpeed(firstSpeed);

	// 入力取得
	auto commandPair = mech->GetInputSys()->GetPilotCommand();
	if (commandPair.first) {
		auto command = commandPair.second;
		Camera3D* cuCamera = MAGISYSTEM::GetCurrentCamera3D();
		Vector2 dir{};
		// 入力がない場合は前方に回避
		if (Length(command.common.StickL)) {
			dir = command.common.StickL; ;
		} else {
			dir = { 0.0f,1.0f };
		}
		// カメラの向きを前方に変換
		dir = StickToMoveDirXZ(dir, cuCamera->GetEye(), cuCamera->GetTarget());
		ms->SetDirXZ(dir);

	}


}

void PilotMechStateDodge::Update(PilotMech* mech) {
	// デルタタイム更新
	const float dt = MAGISYSTEM::GetDeltaTime();

	// ジャスト回避判定
	if (mech->GetJustDodgeCollider()->GetIsHit()) {
		mech->ChangeState(PilotMech::State::JustDodge);
		return;
	}

	// タイマー更新
	timer_ -= dt;
	timer_ = std::max(0.0f, timer_);
	// タイマー終了で移動状態に遷移
	if (timer_ == 0.0f) {
		mech->ChangeState(PilotMech::State::Move);
		return;
	}


	// ラジアルブラーエフェクト
	const float kMaxTime = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Dodge","Time" });
	// ブラーの最大値
	const float kMaxBlurWidth = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","Dodge","MaxBlur" });
	// ブラーの値を補完計算
	const float blurWitdh = (timer_ / kMaxTime) * kMaxBlurWidth;

	// ブラーの座標計算
	const Vector3 blurWorldPos = mech->GetTargetWorldPos();
	const Vector2 blurScreenPos = TransformWorldToScreen(blurWorldPos).second;
	const Vector2 blurScreenPosClamped = {
		std::clamp(blurScreenPos.x / Magi::WindowApp::kClientWidth,0.0f,1.0f),
		std::clamp(blurScreenPos.y / Magi::WindowApp::kClientHeight,0.0f,1.0f)
	};
	// ブラーを掛ける
	MAGISYSTEM::ApplyPostEffectRadialBlur(blurScreenPosClamped, blurWitdh);
}

void PilotMechStateDodge::Exit([[maybe_unused]] PilotMech* mech) {

}