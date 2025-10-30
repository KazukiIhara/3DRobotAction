#define NOMINMAX

#include "MechHandWeapon.h"

// フレームワーク
#include "MAGI.h"

#include "GameObject/Mech/MechCore/MechCore.h"
#include "GameObject/AttackObjectManager/AttackObjectManager.h"

std::string MechHandWeapon::ComvertTypeToString(const MechHandWeapon::Type& type) {
	switch (type) {
	case Type::Gun:
		return "GUN";
	case Type::Melee:
		return "Melee";
	case Type::RocketLauncher:
		return "RocketLauncher";

	default:
		return "";
	}
}

MechHandWeapon::Type MechHandWeapon::ComvertStringToEnum(const std::string& typeString) {
	if (typeString == "GUN") {
		return Type::Gun;
	} else if (typeString == "Melee") {
		return Type::Melee;
	} else if (typeString == "RocketLauncher") {
		return Type::RocketLauncher;
	} else {
		return Type{};
	}
}

MechHandWeapon::MechHandWeapon(const MechHandWeapon::Param& param, AttackObjectManager* attackObjectManager) {
	// パラメータ挿入
	param_ = param;

	// 攻撃オブジェクトマネージャを挿入
	attackObjectManager_ = attackObjectManager;

	// トランスフォーム作成
	std::unique_ptr<Transform3D> transform = std::make_unique<Transform3D>();
	// マネージャに追加
	transform_ = MAGISYSTEM::AddTransform3D(std::move(transform));

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
	// 前方ベクトルを計算
	CulForward();
	// クールタイムを更新
	UpdateCoolTime();
	// リロードを更新
	UpdateReload();
}

void MechHandWeapon::Draw() {
	// 描画
	MAGISYSTEM::DrawModel(param_.modelName, transform_->GetWorldMatrix(), material_);

	//// 攻撃座標デバッグ描画
	//MAGISYSTEM::DrawSphere3D(data_.fireOffsetWorldMatrix, firePosSphereData_, firePosMaterial_);
}

void MechHandWeapon::Attack(MechCore* mechCore) {
	// クールタイム、リロード中なら早期リターン
	if (data_.isCoolTime_ || data_.isReload_)return;

	// タグを取得
	const FriendlyTag tag = mechCore->GetFriendlyTag();

	// 武器タイプごとの攻撃処理
	switch (param_.type) {
	case Type::Gun:
		// 通常の弾を追加
		attackObjectManager_->AddBullet(tag, forward_, param_.speed, data_.fireOffsetWorldPos, param_.damage);
		break;

	case Type::RocketLauncher:
		// ロケラン用の弾を追加
		attackObjectManager_->AddRocket(tag, forward_, param_.speed, data_.fireOffsetWorldPos, param_.damage);

		break;
	default:

		break;
	}

	// 残弾を減らす
	data_.ammo_--;

	// クールタイマーセット
	data_.coolTimer_ = param_.coolTime;
}

void MechHandWeapon::Reload() {
	// リロードタイマーセット
	data_.reloadTimer_ = param_.reloadTime;

}

void MechHandWeapon::SetParam(const MechHandWeapon::Param& param) {
	param_ = param;
}

MechHandWeapon::Param& MechHandWeapon::GetParam() {
	return param_;
}

MechHandWeapon::Data MechHandWeapon::GetData() const {
	return data_;
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

void MechHandWeapon::CulForward() {
	// ワールド行列から前方ベクトルを取得
	forward_ = ExtractionForward(transform_->GetWorldMatrix());
}

void MechHandWeapon::UpdateCoolTime() {
	// クールタイム更新
	data_.coolTimer_ -= MAGISYSTEM::GetDeltaTime();
	// 0を下回らないようにする
	data_.coolTimer_ = std::max(data_.coolTimer_, 0.0f);

	// クールタイム中かチェック
	if (data_.coolTimer_ != 0.0f) {
		data_.isCoolTime_ = true;
	} else {
		data_.isCoolTime_ = false;
	}
}

void MechHandWeapon::UpdateReload() {
	// リロードタイマー更新

	// リロード時間が終了したら残弾を回復、タイマーリセット

}
