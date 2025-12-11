#define NOMINMAX
using namespace MAGI;

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
	// 残弾を最大にする
	data_.ammo_ = param_.ammoCapacity;

	// 手持ち武器のデバッグ描画球体データ
	firePosSphereData_.radius = 0.2f;

	// 手持ち武器のデバッグ描画マテリアルを設定
	firePosMaterial_.textureName = "White.png";
	firePosMaterial_.blendMode = BlendMode::Add;
	firePosMaterial_.baseColor = Color::Red;

	// マズルフラッシュエフェクト初期化
	muzzleFlashEffect_ = std::make_unique<MuzzleFlashEffect>();
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

	// マズルフラッシュエフェクトを更新
	muzzleFlashEffect_->Update(data_.fireOffsetWorldPos);
}

void MechHandWeapon::Draw() {
	// 描画
	MAGISYSTEM::DrawModel(param_.modelName, transform_->GetWorldMatrix(), material_);

	// マズルフラッシュエフェクト描画
	muzzleFlashEffect_->Draw();

	//// 攻撃座標デバッグ描画
	//MAGISYSTEM::DrawSphere3D(data_.fireOffsetWorldMatrix, firePosSphereData_, firePosMaterial_);
}

void MechHandWeapon::Attack(MechCore* mechCore) {
	// 残弾なしならリロードして早期リターン
	if (data_.ammo_ == 0) {
		Reload();
		return;
	}

	// クールタイム、リロード中なら早期リターン
	if (data_.isCoolTime_ || data_.isReload_) {
		return;
	}

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

		// 硬直時間をセット
		mechCore->GetStatusComponent()->SetRecoveryTime(param_.coolTime);
		// 硬直ステートに遷移
		mechCore->ChangeState(MechCoreState::Recovery);

		break;
	}

	// 残弾を減らす
	data_.ammo_--;

	// マズルフラッシュ発火
	muzzleFlashEffect_->Emit();

	// クールタイマーセット フラグを立てる
	data_.coolTimer_ = param_.coolTime;
	data_.isCoolTime_ = true;
}

void MechHandWeapon::Reload() {
	// リロード中なら早期リターン
	if (data_.isReload_) {
		return;
	}
	// リロードタイマーセット
	data_.reloadTimer_ = param_.reloadTime;
	// リロードフラグを立てる
	data_.isReload_ = true;
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
	// クールタイム中でなければ早期リターン
	if (!data_.isCoolTime_) {
		return;
	}
	// クールタイム更新
	data_.coolTimer_ -= MAGISYSTEM::GetDeltaTime();
	// 0を下回らないようにする
	data_.coolTimer_ = std::max(data_.coolTimer_, 0.0f);
	// クールタイム中かチェック
	if (data_.coolTimer_ == 0.0f) {
		data_.isCoolTime_ = false;
	}
}

void MechHandWeapon::UpdateReload() {
	// リロード中でなければ早期リターン
	if (!data_.isReload_) {
		return;
	}
	// リロードタイマー更新
	data_.reloadTimer_ -= MAGISYSTEM::GetDeltaTime();
	// 0を下回らないようにする
	data_.reloadTimer_ = std::max(data_.reloadTimer_, 0.0f);
	// リロード終了処理
	if (data_.reloadTimer_ == 0.0f) {
		data_.isReload_ = false;
		data_.ammo_ = param_.ammoCapacity;
	}
}
