#include "Boss.h"

#include "MAGI.h"

using namespace Magi;

Boss::Boss(BaseMech::RefContext ref) {

	// パラメータ作成
	LoadMechInitParam();

	// 機体の作成
	mech_ = std::make_unique<BossMech>(initParam_, ref);
	// AIの作成


}

void Boss::Update() {
#if defined (DEBUG)|(DEVELOP)
	LoadMechInitParam();
#endif

	// 機体の更新
	mech_->Update(flag_.isDebugDraw, initParam_);

}

void Boss::Draw() {
	// 機体の描画
	mech_->Draw();
}

BossMech* Boss::GetMech() {
	return mech_.get();
}

void Boss::LoadMechInitParam() {
	initParam_.head.modelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Boss","Head","ModelName" });
	initParam_.head.translate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Boss","Head","Translate" });

	initParam_.body.modelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Boss","Body","ModelName" });
	initParam_.body.translate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Boss","Body","Translate" });

	initParam_.armR.upperModelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Boss","UpperArmRight","ModelName" });
	initParam_.armR.upperTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Boss","UpperArmRight","Translate" });
	initParam_.armR.lowerModelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Boss","LowerArmRight","ModelName" });
	initParam_.armR.lowerTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Boss","LowerArmRight","Translate" });
	initParam_.armR.handModelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Boss","HandRight","ModelName" });
	initParam_.armR.handTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Boss","HandRight","Translate" });

	initParam_.armL.upperModelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Boss","UpperArmLeft","ModelName" });
	initParam_.armL.upperTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Boss","UpperArmLeft","Translate" });
	initParam_.armL.lowerModelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Boss","LowerArmLeft","ModelName" });
	initParam_.armL.lowerTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Boss","LowerArmLeft","Translate" });
	initParam_.armL.handModelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Boss","HandLeft","ModelName" });
	initParam_.armL.handTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Boss","HandLeft","Translate" });

	initParam_.leg.waistModelName = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Boss","Waist","ModelName" });
	initParam_.leg.waistTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Boss","Waist","Translate" });

	initParam_.leg.upperModelNameLeft = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Boss","UpperLegLeft","ModelName" });
	initParam_.leg.upperTranslateLeft = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Boss","UpperLegLeft","Translate" });
	initParam_.leg.lowerModelNameLeft = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Boss","LowerLegLeft","ModelName" });
	initParam_.leg.lowerTranslateLeft = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Boss","LowerLegLeft","Translate" });
	initParam_.leg.footModelNameLeft = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Boss","FootLeft","ModelName" });
	initParam_.leg.footTranslateLeft = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Boss","FootLeft","Translate" });

	initParam_.leg.upperModelNameRight = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Boss","UpperLegRight","ModelName" });
	initParam_.leg.upperTranslateRight = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Boss","UpperLegRight","Translate" });
	initParam_.leg.lowerModelNameRight = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Boss","LowerLegRight","ModelName" });
	initParam_.leg.lowerTranslateRight = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Boss","LowerLegRight","Translate" });
	initParam_.leg.footModelNameRight = MAGISYSTEM::GetParameterValue<std::string>({ "MechInitParam","Boss","FootRight","ModelName" });
	initParam_.leg.footTranslateRight = MAGISYSTEM::GetParameterValue<Vector3>({ "MechInitParam","Boss","FootRight","Translate" });
}

Boss::Flag Boss::GetFlag() const {
	return flag_;
}

void Boss::SwitchDebugDraw() {
	flag_.isDebugDraw = !flag_.isDebugDraw;
}

void Boss::SwitchAIActive() {
	flag_.isAIActive = !flag_.isAIActive;
}

void Boss::SwitchIsPause() {
	flag_.isPause = !flag_.isPause;
}