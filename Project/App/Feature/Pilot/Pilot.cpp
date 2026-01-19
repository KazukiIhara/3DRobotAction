#include "Pilot.h"

#include "MAGI.h"

using namespace Magi;

Pilot::Pilot(BaseMech::RefContext ref, TPSCamera3D* camera) {
	// パラメータ作成
	LoadMechInitParam();

	// 機体の作成
	mech_ = std::make_unique<PilotMech>(initParam_, ref);

	// カメラセット
	camera_ = camera;
	// 機体の胴体を追従対象にする
	camera_->SetFollowTransform(mech_->GetPartsTransform(MechAnimation::TransType::Body));


	// 機体操作クラス
	operator_ = std::make_unique<PilotOperator>(mech_.get(), camera_);
}

void Pilot::Update() {
#if defined (DEBUG)|(DEVELOP)
	LoadMechInitParam();
#endif
	// 操作更新
	operator_->Update();

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