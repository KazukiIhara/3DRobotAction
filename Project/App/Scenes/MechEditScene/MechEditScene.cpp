#include "MechEditScene.h"

void MechEditScene::Initialize() {
	// 2Dカメラ作成
	std::unique_ptr<Camera2D> sceneCamera2D = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");


	//-------------------------------------------------------
	// アセットのロード
	//-------------------------------------------------------
	uint32_t skyBoxTexutreIndex = MAGISYSTEM::LoadTexture("kloppenheim_06_puresky_2k.dds");

	MAGISYSTEM::LoadModel("AssultRifle");
	MAGISYSTEM::CreateModelDrawer("AssultRifle", MAGISYSTEM::FindModel("AssultRifle"));

	MAGISYSTEM::LoadTexture("White.png");

	//-------------------------------------------------------
	// シーン固有の初期化処理
	//-------------------------------------------------------

	// スカイボックスを設定
	MAGISYSTEM::SetSkyBoxTextureIndex(skyBoxTexutreIndex);


	// 手武器作成
	handWeapon_ = std::make_unique<MechHandWeapon>(handWeaponParam_);

	// 手持ち武器のデバッグ描画球体データ
	firePosSphereData_.radius = 0.2f;

	// 手持ち武器のデバッグ描画マテリアルを設定
	firePosMaterial_.textureName = "White.png";
	firePosMaterial_.blendMode = BlendMode::Add;
	firePosMaterial_.baseColor = Color::Red;
}

void MechEditScene::Update() {
	// 手武器更新
	handWeapon_->Update();

	// GUI更新
	UpdateGUI();
}

void MechEditScene::Draw() {

	// 手持ち武器描画
	HandWeaponEditDraw();
}

void MechEditScene::Finalize() {
	MAGISYSTEM::DeleteAll();
}

void MechEditScene::UpdateGUI() {
	ImGui::Begin("EditUI");

	// 手持ち武器のエディット更新
	HandWeaponEditUpdate();

	ImGui::End();
}

void MechEditScene::HandWeaponEditUpdate() {
	// パラメータ取得
	MechHandWeapon::Param &param = handWeapon_->GetParam();

	// 発射位置ローカル座標
	ImGui::DragFloat3("FirePos", &param.fireOffsetLocalPos.x, 0.01f);


}

void MechEditScene::HandWeaponEditDraw() {
	// 手武器描画
	handWeapon_->Draw();

	// パラメータ取得
	MechHandWeapon::Param& param = handWeapon_->GetParam();


	// 攻撃座標デバッグ描画
	MAGISYSTEM::DrawSphere3D(param.fireOffsetWorldMatrix, firePosSphereData_, firePosMaterial_);
}
