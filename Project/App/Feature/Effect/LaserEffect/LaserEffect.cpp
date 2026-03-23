#define NOMINMAX

#include "LaserEffect.h"

#include "MAGI.h"

using namespace MAGIMath;
using namespace Magi;

LaserEffect::LaserEffect(const InitParam& initParam) :
	BaseGameEffect(initParam.emitPos) {
	// エディットシーン用に一応ロード処理を書く
	MAGISYSTEM::LoadTexture("laserBlue.png");
	MAGISYSTEM::LoadTexture("laserRed.png");

	// パラメータ初期化
	life_ = initParam.life;
	dir_ = initParam.dir;
	laserEndPos_ = initParam.emitPos;

	// 親トランスフォーム初期化
	std::unique_ptr<Magi::Transform3D> parentTrans = std::make_unique<Magi::Transform3D>(initParam.emitPos);
	laserParent_ = MAGISYSTEM::AddTransform3D(std::move(parentTrans));

	// 板ポリ初期データ取得
	const std::array<Vector3, 2> planeInitRotate = {
		MAGISYSTEM::GetParameterValue<Vector3>({"EffectParam","Laser","PlaneRotate0"}),
		MAGISYSTEM::GetParameterValue<Vector3>({"EffectParam","Laser","PlaneRotate1"})
	};
	const Vector3 planeInitTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "EffectParam","Laser","PlaneTranslate" });

	// 板ポリ初期化
	for (size_t i = 0; i < 2; i++) {
		std::unique_ptr<Magi::Transform3D> planeTrans = std::make_unique<Magi::Transform3D>(Vector3(1.0f, 1.0f, 1.0f), planeInitRotate[i], planeInitTranslate);
		planeTrans_[i] = MAGISYSTEM::AddTransform3D(std::move(planeTrans));
		planeMat_.uvRotate = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","Laser","PlaneUVRotate" });
		planeTrans_[i]->SetParent(laserParent_, false);
		planeMat_.uvRotate = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","Laser","PlaneUVRotate" });
	}

	// 加算合成
	planeMat_.blendMode = BlendMode::Add;

	if (color_ == Col::RED) {
		planeMat_.textureName = "laserRed.png";
	} else {
		planeMat_.textureName = "laserBule.png";
	}
}

void LaserEffect::Update() {

	// デルタタイムを取得
	const float kDt = MAGISYSTEM::GetDeltaTime();
	// タイマーを更新
	life_ -= kDt;
	life_ = std::max(0.0f, life_);
	// タイマーが0になったら生存フラグを切る
	if (life_ == 0.0f) {
		isAlive_ = false;
		return;
	}


	Vector3 dir = laserEndPos_ - worldPos_;
	float length = std::max(0.01f, Length(dir));
	const Quaternion q = DirectionToQuaternion_s(dir);
	laserParent_->SetQuaternion(q);
	laserParent_->SetScaleZ(length * 0.5f);
}

void LaserEffect::Draw() {
	// 板ポリ描画
	for (size_t i = 0; i < 2; i++) {
		MAGISYSTEM::DrawPlane3D(planeTrans_[i]->GetWorldMatrix(), planeData_[i], planeMat_);
	}
}

void LaserEffect::Finalize() {
	// トランスフォームを消す
	laserParent_->SetIsAlive(false);
	for (size_t i = 0; i < 2; i++) {
		planeTrans_[i]->SetIsAlive(false);
	}
}

void LaserEffect::SetLaserEndPos(const Vector3& endPos) {
	laserEndPos_ = endPos;
}

void LaserEffect::DebugUpdate() {
	// パラメータ調整用処理

	// 角度調整用処理
	const std::array<Vector3, 2> planeInitRotate = {
		MAGISYSTEM::GetParameterValue<Vector3>({"EffectParam","Laser","PlaneRotate0"}),
		MAGISYSTEM::GetParameterValue<Vector3>({"EffectParam","Laser","PlaneRotate1"})
	};

	const Vector3 planeInitTranslate = MAGISYSTEM::GetParameterValue<Vector3>({ "EffectParam","Laser","PlaneTranslate" });
	for (size_t i = 0; i < 2; i++) {
		planeTrans_[i]->SetRotate(planeInitRotate[i]);
		planeTrans_[i]->SetTranslate(planeInitTranslate);
		planeMat_.uvRotate = MAGISYSTEM::GetParameterValue<float>({ "EffectParam","Laser","PlaneUVRotate" });
	}

}
