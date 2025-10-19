#include "MechHandWeapon.h"

// フレームワーク
#include "MAGI.h"

MechHandWeapon::MechHandWeapon(const MechHandWeapon::Param& param) {
	// トランスフォーム作成
	std::unique_ptr<Transform3D> transform = std::make_unique<Transform3D>();
	// マネージャに追加
	transform_ = MAGISYSTEM::AddTransform3D(std::move(transform));

	// パラメータ挿入
	param_ = param;
}

void MechHandWeapon::Update() {
	// 弾発射位置を計算
	CulFirePos();
}

void MechHandWeapon::Draw() {
	// 描画
	MAGISYSTEM::DrawModel(param_.modelName, transform_->GetWorldMatrix(), material_);
}

void MechHandWeapon::Attack() {
	
}

MechHandWeapon::Param& MechHandWeapon::GetParam() {
	return param_;
}

Transform3D* MechHandWeapon::GetTransform() {
	return transform_;
}

void MechHandWeapon::CulFirePos() {
	// ローカル行列を計算
	param_.fireOffsetLocalMatrix = MakeTranslateMatrix(param_.fireOffsetLocalPos);	
	// ワールド行列を計算
	param_.fireOffsetWorldMatrix = param_.fireOffsetLocalMatrix * transform_->GetWorldMatrix();
	// ワールド座標を計算
	param_.fireOffsetWorldPos = ExtractionWorldPos(param_.fireOffsetWorldMatrix);
}
