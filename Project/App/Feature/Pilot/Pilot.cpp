#include "Pilot.h"

#include "Feature/TPSCamera3D/TPSCamera3D.h"
#include "Feature/GameInputSystem/GameInputSystem.h"

#include "MAGI.h"
using namespace Magi;

Pilot::Pilot(BaseMech::RefContext ref, RefContext pRef) {
	// 入力システム受け取り
	inputSys_ = pRef.inputSys;
	// カメラセット
	camera_ = pRef.camera;

	// パラメータ読み込み
	LoadMechInitParam();

	// 機体の作成
	mech_ = std::make_unique<PilotMech>(initParam_, ref, inputSys_);

}

void Pilot::Update() {
#if defined (DEBUG)|(DEVELOP)
	LoadMechInitParam();
#endif

	// カメラ更新
	CameraControl();

	// 機体の更新
	mech_->Update(flag_.isDebugDraw, initParam_);

}

void Pilot::Draw() {
	// 機体の描画
	mech_->Draw();
}

PilotMech* Pilot::GetMech() {
	return mech_.get();
}

Pilot::Flag Pilot::GetFlag() const {
	return flag_;
}

void Pilot::SwitchDebugDraw() {
	flag_.isDebugDraw = !flag_.isDebugDraw;
}

void Pilot::SwitchIsPause() {
	flag_.isPause = !flag_.isPause;
}

GameInputSystem* Pilot::GetInputSys() {
	return inputSys_;
}

void Pilot::LoadMechInitParam() {
	initParam_.position = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","InitPos" });

	initParam_.head.modelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Pilot","Head","ModelName" });
	initParam_.head.translate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","Head","Translate" });

	initParam_.body.modelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Pilot","Body","ModelName" });
	initParam_.body.translate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","Body","Translate" });

	initParam_.armR.upperModelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Pilot","UpperArmRight","ModelName" });
	initParam_.armR.upperTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","UpperArmRight","Translate" });
	initParam_.armR.lowerModelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Pilot","LowerArmRight","ModelName" });
	initParam_.armR.lowerTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","LowerArmRight","Translate" });
	initParam_.armR.handModelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Pilot","HandRight","ModelName" });
	initParam_.armR.handTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","HandRight","Translate" });

	initParam_.armL.upperModelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Pilot","UpperArmLeft","ModelName" });
	initParam_.armL.upperTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","UpperArmLeft","Translate" });
	initParam_.armL.lowerModelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Pilot","LowerArmLeft","ModelName" });
	initParam_.armL.lowerTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","LowerArmLeft","Translate" });
	initParam_.armL.handModelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Pilot","HandLeft","ModelName" });
	initParam_.armL.handTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","HandLeft","Translate" });

	initParam_.leg.waistModelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Pilot","Waist","ModelName" });
	initParam_.leg.waistTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","Waist","Translate" });

	initParam_.leg.upperModelNameLeft = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Pilot","UpperLegLeft","ModelName" });
	initParam_.leg.upperTranslateLeft = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","UpperLegLeft","Translate" });
	initParam_.leg.lowerModelNameLeft = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Pilot","LowerLegLeft","ModelName" });
	initParam_.leg.lowerTranslateLeft = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","LowerLegLeft","Translate" });
	initParam_.leg.footModelNameLeft = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Pilot","FootLeft","ModelName" });
	initParam_.leg.footTranslateLeft = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","FootLeft","Translate" });

	initParam_.leg.upperModelNameRight = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Pilot","UpperLegRight","ModelName" });
	initParam_.leg.upperTranslateRight = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","UpperLegRight","Translate" });
	initParam_.leg.lowerModelNameRight = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Pilot","LowerLegRight","ModelName" });
	initParam_.leg.lowerTranslateRight = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","LowerLegRight","Translate" });
	initParam_.leg.footModelNameRight = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Pilot","FootRight","ModelName" });
	initParam_.leg.footTranslateRight = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Pilot","FootRight","Translate" });

	initParam_.tag = FriendlyTag::PlayerSide;
}

void Pilot::CameraControl() {
	if (!camera_) {
		return;
	}
	// コマンドペア取得
	auto commandPair = inputSys_->GetPilotCommand();
	if (!commandPair.first) {
		return;
	}

	// ロックオン対象がいる場合
	if (ILockOnTarget* target = mech_->GetLockOnSystem()->GetTarget()) {
		camera_->SetLockOnTarget(target);
	}

	// コマンド取得
	auto command = commandPair.second;

	const Vector2 rs = command.common.StickR;
	const float dt = MAGISYSTEM::GetDeltaTime();
	const float yawSens = MAGISYSTEM::GetParameterValue<float>({ "PilotCameraParam" ,"YawSens" });
	const float pitchSens = MAGISYSTEM::GetParameterValue<float>({ "PilotCameraParam" ,"PitchSens" });

	// yaw/pitch 反映
	camera_->AddYaw(rs.x * yawSens * dt);
	camera_->AddPitch(rs.y * pitchSens * dt);
}