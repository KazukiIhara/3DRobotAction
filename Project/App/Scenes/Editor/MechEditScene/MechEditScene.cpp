#include "MechEditScene.h"

using namespace Magi;

void MechEditScene::Initialize() {
	// 2Dカメラ作成
	std::unique_ptr<Camera2D> sceneCamera2D = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");

	// 3Dカメラ作成
	std::unique_ptr<Camera3D> sceneCamera3D = std::make_unique<Camera3D>("SceneCamera3D");
	// マネージャに追加
	MAGISYSTEM::AddCamera3D(std::move(sceneCamera3D))->ApplyCurrent();


	//-------------------------------------------------------
	// アセットのロード
	//-------------------------------------------------------

	MAGISYSTEM::LoadModel("AssultRifle");
	MAGISYSTEM::CreateModelDrawer("AssultRifle", MAGISYSTEM::FindModel("AssultRifle"));

	MAGISYSTEM::LoadTexture("white.png");

	//-------------------------------------------------------
	// シーン固有の初期化処理
	//-------------------------------------------------------


	// 手武器作成
	//handWeapon_ = std::make_unique<MechHandWeapon>(MechHandWeapon::Param{});


	// 
	// データIOクラス初期化
	// 

	// 手持ち武器
	handWeaponDataIO_ = std::make_unique<HandWeaponDataIO>();

}

void MechEditScene::Update() {
	// 手武器更新
	handWeapon_->Update();

	// GUI更新
	UpdateGUI();
}

void MechEditScene::Draw() {
	// 手武器描画
	handWeapon_->Draw();
}

void MechEditScene::Finalize() {
	MAGISYSTEM::DeleteAll();
}

void MechEditScene::UpdateGUI() {
	// 手持ち武器のエディット更新
	HandWeaponEditUpdate();
}

void MechEditScene::HandWeaponEditUpdate() {
	ImGui::Begin("WeaponEditUI");

	// パラメータ取得
	MechHandWeapon::Param& param = handWeapon_->GetParam();

	// セーブボタン
	if (ImGui::Button("Save")) {
		// ファイルセーブ
		handWeaponDataIO_->SaveFile(param);
	}
	// ロードボタン
	if (ImGui::Button("Load")) {

	}
	ImGui::SameLine();
	GUI::InputTextStd("LoadName", handWeaponLoadNameBuf_);

	ImGui::Separator();

	// 表示名
	GUI::InputTextStd("Name", param.name);

	// 武器タイプ
	HandWeaponTypeUI(param);

	// ダメージ
	ImGui::DragInt("Damage", &param.damage);

	// リロード時間
	ImGui::DragFloat("ReloadTime", &param.reloadTime, 0.01f);

	// 攻撃間隔
	ImGui::DragFloat("CoolTime", &param.coolTime, 0.01f);

	// 装弾数
	ImGui::DragInt("Capacity", &param.ammoCapacity);

	// スピード
	ImGui::DragFloat("Speed", &param.speed, 0.01f);

	// モデル名
	if (ImGui::Button("ChangeModel")) {
		param.modelName = handWeaponLoadNameBuf_;
		handWeaponLoadNameBuf_ = "";
	}
	ImGui::SameLine();
	GUI::InputTextStd("ModelName", handWeaponModelNameBuf_);

	// 発射位置ローカル座標
	ImGui::DragFloat3("FirePos", &param.fireOffsetLocalPos.x, 0.01f);


	ImGui::End();
}

void MechEditScene::HandWeaponTypeUI(MechHandWeapon::Param& param) {
	// Enumをstring配列に変換
	std::vector<std::string> names = {
		MechHandWeapon::ComvertTypeToString(MechHandWeapon::Type::Gun),
		MechHandWeapon::ComvertTypeToString(MechHandWeapon::Type::Melee),
	};

	std::vector<const char*> items;
	for (auto& s : names) items.push_back(s.c_str());

	// 現在のインデックスを取得
	int currentIndex = static_cast<int>(param.type);

	// コンボボックス描画
	if (ImGui::Combo("Weapon Type", &currentIndex, items.data(), static_cast<int>(items.size()))) {
		// 変更をenumに反映
		param.type = static_cast<MechHandWeapon::Type>(currentIndex);
	}
}
