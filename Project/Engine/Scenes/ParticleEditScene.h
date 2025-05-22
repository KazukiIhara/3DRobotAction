#pragma once

#include <vector>
#include <memory>

#include "BaseScene/BaseScene.h"
#include "Framework/MAGI.h"

// パーティクル作成シーン
template <typename Data>
class ParticleEditScene : public BaseScene<Data> {
public:
	using BaseScene<Data>::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~ParticleEditScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	// シーンUI描画
	void ShowSceneUI();

	// エミッターのエディットUI表示
	void ShowEmitterEditUI();
	// パーティクルのエディットUI表示
	void ShowParticleEditUI();

	// パーティクルのリストを表示する関数
	void ShowParticleListUI();

	// Primitiveパーティクルの設定を描画する関数
	void ShowPrimitiveParticleSettingUI();

private:
	// カメラ
	std::unique_ptr<Camera3D> sceneCamera_ = nullptr;

	// エディットするエミッター
	Emitter3D* emitter3D_ = nullptr;
	// パーティクルリスト
	std::vector<BaseParticleGroup3D*> particles_;
	// 選択中のパーティクル
	BaseParticleGroup3D* selectedParticle_ = nullptr;
};

template<typename Data>
inline void ParticleEditScene<Data>::Initialize() {

	MAGISYSTEM::LoadTexture("pronama_chan.png");
	MAGISYSTEM::LoadTexture("circle.png");

	// シーンカメラ作成
	sceneCamera_ = std::make_unique<Camera3D>("SceneCamera");
	MAGISYSTEM::AddCamera3D(std::move(sceneCamera_));

	// カメラの設定
	MAGISYSTEM::SetCurrentCamera3D("SceneCamera");

	// エディター用のエミッターを作成
	MAGISYSTEM::CreatePrimitiveParticleGroup3D("EditParticle", Primitive3DType::Plane);

	// エミッター作成
	MAGISYSTEM::CreateEmitter3D("EditEmitter", Vector3(0.0f, 0.0f, 0.0f));

	emitter3D_ = MAGISYSTEM::FindEmitter3D("EditEmitter");
	// エディター用のパーティクルを一つ作成
	emitter3D_->AddParticleGroup(MAGISYSTEM::FindParticleGroup3D("EditParticle"));
	// エミットタイプ変更
	emitter3D_->GetEmitterSetting().emitType = EmitType::Random;
}

template<typename Data>
inline void ParticleEditScene<Data>::Update() {
	// シーンのUI描画
	ShowSceneUI();
}

template<typename Data>
inline void ParticleEditScene<Data>::Draw() {

}

template<typename Data>
inline void ParticleEditScene<Data>::Finalize() {

}

template<typename Data>
inline void ParticleEditScene<Data>::ShowSceneUI() {
	// ウィンドウを作成
	ImGui::Begin("EmitterEditUI");

	// タブを作成
	if (ImGui::BeginTabBar("ParticleEditTab")) {
		// エミッターのエディットUI描画
		ShowEmitterEditUI();
		// パーティクルのエディットUI描画
		ShowParticleEditUI();
		// タブ終了
		ImGui::EndTabBar();
	}
	// ウィンドウを終了
	ImGui::End();
}

template<typename Data>
inline void ParticleEditScene<Data>::ShowEmitterEditUI() {
	// タブのアイテムとして作成
	if (ImGui::BeginTabItem("EmitterSettings")) {
		// 名前を描画
		ImGui::Text("%s", emitter3D_->name.c_str());
		// 発生ボタン
		if (ImGui::Button("Emit")) {
			emitter3D_->EmitAll();
		}
		ImGui::SameLine();
		// セーブボタン
		if (ImGui::Button("Save")) {

		}
		ImGui::SameLine();
		if (ImGui::Button("Load")) {

		}

		// 描画サンプルのエミッターをいじるUI
		ImGui::Text("EmitterParamater");
		// 発生パターン

		// 1度の発生個数
		int count = static_cast<int>(emitter3D_->GetEmitterSetting().count);
		ImGui::DragInt("EmitCount", &count);
		// 発生個数が0未満にならないようにする
		count = std::max(0, count);
		emitter3D_->GetEmitterSetting().count = static_cast<int>(count);
		// 発生頻度
		ImGui::DragFloat("Frequency", &emitter3D_->GetEmitterSetting().frequency, 0.01f);
		// 繰り返し発生するかどうか
		ImGui::Checkbox("IsRepeat", &emitter3D_->GetEmitterSetting().isRepeat);
		// 最小の発生地点
		ImGui::DragFloat3("MinTranslate", &emitter3D_->GetEmitterSetting().minTranslate.x, 0.01f);
		// 最大の発生地点
		ImGui::DragFloat3("MaxTranslate", &emitter3D_->GetEmitterSetting().maxTranslate.x, 0.01f);
		// 最小移動量
		ImGui::DragFloat3("MinVelocity", &emitter3D_->GetEmitterSetting().minVelocity.x, 0.01f);
		// 最大移動量
		ImGui::DragFloat3("MaxVelocity", &emitter3D_->GetEmitterSetting().maxVelocity.x, 0.01f);
		// 最小サイズ
		ImGui::DragFloat("MinScale", &emitter3D_->GetEmitterSetting().minScale, 0.01f);
		// 最大サイズ
		ImGui::DragFloat("MaxScale", &emitter3D_->GetEmitterSetting().maxScale, 0.01f);
		// 最短生存時間
		ImGui::DragFloat("MinLifeTime", &emitter3D_->GetEmitterSetting().minLifeTime, 0.01f);
		// 最長生存時間
		ImGui::DragFloat("MaxLifeTime", &emitter3D_->GetEmitterSetting().maxLifeTime, 0.01f);
		// 最小の色
		ImGui::ColorEdit4("MinColor", &emitter3D_->GetEmitterSetting().minColor.x);
		// 最大の色
		ImGui::ColorEdit4("MaxColor", &emitter3D_->GetEmitterSetting().maxColor.x);


		// タブ終了
		ImGui::EndTabItem();
	}

}

template<typename Data>
inline void ParticleEditScene<Data>::ShowParticleEditUI() {

	// タブのアイテムとして作成
	if (ImGui::BeginTabItem("ParticleSettings")) {

		// パーティクルリストのUIを作成
		ShowParticleListUI();
		// 選択中のパーティクルがあれば
		if (selectedParticle_) {
			// パーティクル名を描画
			ImGui::Text("%s", selectedParticle_->name.c_str());
			// 描画タイプ取得
			Renderer3DType type = selectedParticle_->GetRendererType();
			// タイプごとにキャスト
			switch (type) {
			case Renderer3DType::Primitive: {
				ShowPrimitiveParticleSettingUI();
				break;
			}
			case Renderer3DType::Static:

				break;
			case Renderer3DType::Skinning:

				break;
			}
		}


		// タブ終了
		ImGui::EndTabItem();
	}
}

template<typename Data>
inline void ParticleEditScene<Data>::ShowParticleListUI() {
	// リストであることを明示する
	ImGui::Text("Particle List");

	// 右側にパーティクルリスト
	ImGui::BeginChild("ParticleList", ImVec2(400, 50), true);
	// パーティクルマネージャーからパーティクルグループを取得
	const auto& particleGroups = MAGISYSTEM::GetParticleGroupList();

	for (const auto& particle : particleGroups) {
		if (particle) {
			// パーティクル名を取得
			const std::string& particleName = particle->name;

			// 選択されたら `selectedParticle_` を更新
			if (ImGui::Selectable(particleName.c_str(), selectedParticle_ == particle.get())) {
				selectedParticle_ = particle.get();
			}
		}
	}

	ImGui::EndChild();
}

template<typename Data>
inline void ParticleEditScene<Data>::ShowPrimitiveParticleSettingUI() {
	PrimitiveParticleGroup3D* primitive = dynamic_cast<PrimitiveParticleGroup3D*>(selectedParticle_);
	if (primitive) {
		// テクスチャリストを表示
		ImGui::Text("TextureList");
		ImGui::BeginChild("TextureList", ImVec2(400, 50), true);
		// 読み込んでいるテクスチャリストを取得
		const auto& textures = MAGISYSTEM::GetTextureContainer();
		for (const auto& texture : textures) {
			// テクスチャ名を取得
			const std::string& textureName = texture.first;
			// 選択可能にする選択したら現在のプリミティブパーティクルグループにセット
			if (ImGui::Selectable(textureName.c_str())) {
				primitive->GetTextureName() = textureName;
			}
		}
		ImGui::EndChild();

	}
}