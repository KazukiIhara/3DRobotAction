#include "PilotMechStateKnockBack.h"

#include "Feature/Pilot/Mech/PilotMech.h"

#include "MAGI.h"
#include "Math/Utility/MathUtility.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace Magi;
using namespace MAGIMath;
using namespace MAGIUtility;

void PilotMechStateKnockBack::Enter([[maybe_unused]] PilotMech* mech) {
	// パラメータ取得
	const float time = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","KnockBack","Time" });
	const float impactSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","KnockBack","ImpactSpeed" });
	const float impactDirY = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","KnockBack","ImpactDirY" });
	const float acc = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","KnockBack","Acc" });

	// タイマーセット
	timer_ = time;
	// 移動システムを取得
	auto ms = mech->GetMoveSystem();
	// 移動システムリセット
	ms->Reset();

	// 吹っ飛ぶ方向と速度を計算
	const Vector3 hitPos = mech->GetStatus()->GetHitPos();
	const Vector3 mechCenter = mech->GetCenterPos();
	const Vector3 dir = Normalize(mechCenter - hitPos);

	// 吹っ飛ぶ方向と速度、加速度をセット
	ms->SetDirXZ(Vector2(dir.x, dir.z));
	ms->SetDirY(impactDirY);
	ms->SetSpeed(impactSpeed);
	ms->SetAcc(acc);

}

void PilotMechStateKnockBack::Update([[maybe_unused]] PilotMech* mech) {
	// デルタタイム取得
	const float dt = MAGISYSTEM::GetDeltaTime();
	// タイマー更新
	timer_ -= dt;
	timer_ = std::max(0.0f, timer_);
	if (timer_ <= 0.0f) {
		mech->ChangeState(PilotMech::State::Move);
	}

}

void PilotMechStateKnockBack::Exit([[maybe_unused]] PilotMech* mech) {
	// 移動システムを取得
	auto ms = mech->GetMoveSystem();
	ms->Reset();
}
