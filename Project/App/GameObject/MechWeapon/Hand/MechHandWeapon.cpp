#include "MechHandWeapon.h"

// フレームワーク
#include "MAGI.h"

std::string MechHandWeapon::ComvertTypeToString(const MechHandWeapon::Type& type) {
	switch (type) {
		case Type::Gun:
			return "GUN";
		case Type::Melee:
			return "Melee";
		default:
			return "";
	}
}

MechHandWeapon::Type MechHandWeapon::ComvertStringToEnum(const std::string& typeString) {
	if (typeString == "GUN") {
		return Type::Gun;
	} else if (typeString == "Melee") {
		return Type::Melee;
	} else {
		return Type{};
	}
}

MechHandWeapon::MechHandWeapon(const MechHandWeapon::Param& param) {
	// トランスフォーム作成
	std::unique_ptr<Transform3D> transform = std::make_unique<Transform3D>();
	// マネージャに追加
	transform_ = MAGISYSTEM::AddTransform3D(std::move(transform));

	// パラメータ挿入
	param_ = param;

	// データ初期化
	data_ = Data{};

	// 手持ち武器のデバッグ描画球体データ
	firePosSphereData_.radius = 0.2f;

	// 手持ち武器のデバッグ描画マテリアルを設定
	firePosMaterial_.textureName = "White.png";
	firePosMaterial_.blendMode = BlendMode::Add;
	firePosMaterial_.baseColor = Color::Red;
}

void MechHandWeapon::Update() {
	// 弾発射位置を計算
	CulFirePos();
}

void MechHandWeapon::Draw() {
	// 描画
	MAGISYSTEM::DrawModel(param_.modelName, transform_->GetWorldMatrix(), material_);

	// 攻撃座標デバッグ描画
	MAGISYSTEM::DrawSphere3D(data_.fireOffsetWorldMatrix, firePosSphereData_, firePosMaterial_);
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
	data_.fireOffsetLocalMatrix = MakeTranslateMatrix(param_.fireOffsetLocalPos);
	// ワールド行列を計算
	data_.fireOffsetWorldMatrix = data_.fireOffsetLocalMatrix * transform_->GetWorldMatrix();
	// ワールド座標を計算
	data_.fireOffsetWorldPos = ExtractionWorldPos(data_.fireOffsetWorldMatrix);
}
