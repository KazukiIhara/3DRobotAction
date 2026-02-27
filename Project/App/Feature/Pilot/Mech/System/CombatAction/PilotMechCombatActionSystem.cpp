#include "PilotMechCombatActionSystem.h"

#include "MAGI.h"

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/GameInputSystem/GameInputSystem.h"

using namespace Magi;

PilotMechCombatActionSystem::PilotMechCombatActionSystem(PilotMech* mech) {
	// 機体のポインタを受け取る
	mech_ = mech;
	assert(mech_);

	// パラメータ作成

	// 
	//  機体の傾き
	// 

	// 傾きの最大値
	MAGISYSTEM::AddParameterData({ "PilotMech","CombatAction","MechSlope","MaxSlope" }, ParamType::Float);
	// 最大の傾きになる速度
	MAGISYSTEM::AddParameterData({ "PilotMech","CombatAction","MechSlope","MaxSlopeSpeed" }, ParamType::Float);

	// 
	// 攻撃アニメーション
	// 

	// SetUpの時間
	MAGISYSTEM::AddParameterData({ "PilotMech","CombatAction","RHAttack","SetUpTime" }, ParamType::Float);

}

void PilotMechCombatActionSystem::Update() {
	// 速度による機体の傾き更新
	MechSlopeUpdate();

	// 右手武器攻撃ステート更新
	RightWeaponAttackStateUpdate();
}

void PilotMechCombatActionSystem::MechSlopeUpdate() {
	// 移動方向を取得
	Vector3 dir = mech_->GetMoveSystem()->GetDir();
	// 速度を取得
	const float speed = mech_->GetMoveSystem()->GetSpeed();
	// 最大の傾きを取得
	const float maxSlope = MAGISYSTEM::GetParameterValue<float>({ "PilotMech","CombatAction","MechSlope","MaxSlope" });
	// 最大の傾きになる速度を取得
	const float maxSlopeSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotMech","CombatAction","MechSlope","MaxSlopeSpeed" });

	// 地面方向に固定
	dir.y = 0.0f;

	// 入力が無いときは何もしない
	if (Dot(dir, dir) < 1e-6f || maxSlopeSpeed <= 1e-6f) {
		return;
	}

	// ワールド空間の前・右軸
	const Vector3 fwdWorld = Normalize(dir);
	const Vector3 upWorld = MakeUpVector3();
	const Vector3 rightWorld = Normalize(Cross(upWorld, fwdWorld));

	// 傾き角
	const float rate = std::min(speed / maxSlopeSpeed, 1.0f);
	const float slopeAngle = Lerp(0.0f, maxSlope, rate);

	// モデルの回転（ワールド）
	const Quaternion modelQ = mech_->GetModelTransform()->GetQuaternion();

	// rightWorld をモデルローカルへ変換
	const Vector3 rightLocal = Normalize(Transform(rightWorld, Inverse(modelQ)));

	// ローカル空間で傾ける回転を作る
	const Quaternion leanLocalQ = MakeRotateAxisAngleQuaternion(rightLocal, slopeAngle);

	// 腰の回転をセット（ローカル回転）
	auto* waist = mech_->GetPartsTransform(MechAnimation::TransType::Waist);
	waist->SetQuaternion(leanLocalQ);

}

void PilotMechCombatActionSystem::SetEnableRightWeapon(bool e) {
	enableRightWeapon_ = e;
}

void PilotMechCombatActionSystem::RightWeaponAttackStateUpdate() {
	// デルタタイム取得
	const float dt = MAGISYSTEM::GetDeltaTime();

	// ステートごとに目標の回転角を設定
	switch (rightWeaponAttackState_) {
	case PilotMechCombatActionSystem::RightWeaponAttackState::Idle:
		UpdateIdle();
		break;
	case PilotMechCombatActionSystem::RightWeaponAttackState::SetUp:
		UpdateSetUp();
		break;
	case PilotMechCombatActionSystem::RightWeaponAttackState::Attack:
		UpdateAttack();
		break;
	}

	UpdateSetUp();

}

void PilotMechCombatActionSystem::UpdateIdle() {

}

void PilotMechCombatActionSystem::UpdateSetUp() {
	// SetUp時間
	const float time = MAGISYSTEM::GetParameterValue<float>({ "PilotMech","CombatAction","RHAttack","SetUpTime" });

	// ターゲット座標
	const Vector3 targetPos = mech_->GetTargetWorldPos();

	// 右上腕
	auto* upperArm = mech_->GetPartsTransform(MechAnimation::TransType::UpperArmRight);

	// 腕の支点（ワールド）
	const Vector3 armPos = upperArm->GetWorldPosition();

	// 目標方向（ワールド）
	const Vector3 forward = MAGIMath::Normalize(targetPos - armPos);

	const Quaternion worldQ = DirectionToQuaternion_s(forward);

	const Quaternion targetQZ = Inverse(mech_->GetModelTransform()->GetQuaternion()) * worldQ;

	const Quaternion targetQY = targetQZ * MakeFixForwardZToMinusY();

	// 反映
	upperArm->SetQuaternion(targetQY);
}

void PilotMechCombatActionSystem::UpdateAttack() {

}
