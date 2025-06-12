#include "Emitter3D.h"

#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"
#include "Random/Random.h"

#include <cassert>

using namespace MAGIMath;
using namespace MAGIUtility;

Emitter3D::Emitter3D(const std::string& name, const Vector3& position) {
	Initialize(position);
	name_ = name;
}

Emitter3D::~Emitter3D() {

}

void Emitter3D::Initialize(const Vector3& position) {
	transform_ = std::make_unique<Transform3D>(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), position);
	transform_->Update();
}

void Emitter3D::Update() {
	// ワールドトランスフォームの更新
	transform_->Update();

	// 繰り返しフラグがオンなら繰り返し発生
	if (emitterSetting_.isRepeat) {

		// エミッターの処理
		// 時刻を進める
		frequencyTimer_ += MAGISYSTEM::GetDeltaTime();

		// 頻度より大きいなら発生
		if (emitterSetting_.frequency <= frequencyTimer_) {
			EmitAll();
			// 余計に過ぎた時間も加味して頻度計算する
			frequencyTimer_ -= emitterSetting_.frequency;
		}
	}
}

void Emitter3D::EmitAll() {
	// パーティクル発生パラメータ
	EmitParamater emitParamater{};
	// パーティクル発生ポジション
	Vector3	emitterPos = transform_->GetWorldPosition();
	emitParamater.position = emitterPos;
	// 発生タイプごとの処理
	switch (emitterSetting_.emitType) {
		// デフォルト
	case EmitType::Default:
		// パーティクルグループの個数分ループ
		for (auto particleGroup : particleGroups_) {
			// 発生個数分ループ
			for (uint32_t i = 0; i < emitterSetting_.count; i++) {

				// サイズ
				float scale = Random::GenerateFloat(emitterSetting_.minScale, emitterSetting_.maxScale);
				emitParamater.scale.x = scale;
				emitParamater.scale.y = scale;
				emitParamater.scale.z = scale;

				// 発生座標
				emitParamater.position.x = emitterPos.x + Random::GenerateFloat(emitterSetting_.minTranslate.x, emitterSetting_.maxTranslate.x);
				emitParamater.position.y = emitterPos.y + Random::GenerateFloat(emitterSetting_.minTranslate.y, emitterSetting_.maxTranslate.y);
				emitParamater.position.z = emitterPos.z + Random::GenerateFloat(emitterSetting_.minTranslate.z, emitterSetting_.maxTranslate.z);

				// 色
				emitParamater.color.x = Random::GenerateFloat(emitterSetting_.minColor.x, emitterSetting_.maxColor.x);
				emitParamater.color.y = Random::GenerateFloat(emitterSetting_.minColor.y, emitterSetting_.maxColor.y);
				emitParamater.color.z = Random::GenerateFloat(emitterSetting_.minColor.z, emitterSetting_.maxColor.z);
				emitParamater.color.w = Random::GenerateFloat(emitterSetting_.minColor.w, emitterSetting_.maxColor.w);

				// 生存時間
				emitParamater.lifeTime = Random::GenerateFloat(emitterSetting_.minLifeTime, emitterSetting_.maxLifeTime);

				particleGroup.second->AddNewParticle(emitParamater);
			}
		}
		break;
	case EmitType::Random:
		// パーティクルグループの個数分ループ
		for (auto particleGroup : particleGroups_) {
			// 発生個数分ループ
			for (uint32_t i = 0; i < emitterSetting_.count; i++) {
				// 発生座標
				emitParamater.position.x = emitterPos.x + Random::GenerateFloat(emitterSetting_.minTranslate.x, emitterSetting_.maxTranslate.x);
				emitParamater.position.y = emitterPos.y + Random::GenerateFloat(emitterSetting_.minTranslate.y, emitterSetting_.maxTranslate.y);
				emitParamater.position.z = emitterPos.z + Random::GenerateFloat(emitterSetting_.minTranslate.z, emitterSetting_.maxTranslate.z);
				// サイズ
				float size = Random::GenerateFloat(emitterSetting_.minScale, emitterSetting_.maxScale);
				emitParamater.scale.x = size;
				emitParamater.scale.y = size;
				emitParamater.scale.z = size;
				// 移動量
				emitParamater.velocity.x = Random::GenerateFloat(emitterSetting_.minVelocity.x, emitterSetting_.maxVelocity.x);
				emitParamater.velocity.y = Random::GenerateFloat(emitterSetting_.minVelocity.y, emitterSetting_.maxVelocity.y);
				emitParamater.velocity.z = Random::GenerateFloat(emitterSetting_.minVelocity.z, emitterSetting_.maxVelocity.z);
				// 色
				emitParamater.color.x = Random::GenerateFloat(emitterSetting_.minColor.x, emitterSetting_.maxColor.x);
				emitParamater.color.y = Random::GenerateFloat(emitterSetting_.minColor.y, emitterSetting_.maxColor.y);
				emitParamater.color.z = Random::GenerateFloat(emitterSetting_.minColor.z, emitterSetting_.maxColor.z);

				// 生存時間
				emitParamater.lifeTime = Random::GenerateFloat(emitterSetting_.minLifeTime, emitterSetting_.maxLifeTime);

				particleGroup.second->AddNewParticle(emitParamater);
			}
		}
		break;
	}

}

void Emitter3D::AddParticleGroup(BaseParticleGroup3D* particleGroup) {
	assert(particleGroup);
	// 名前を取得してマップに入れる
	particleGroups_.insert(std::pair(particleGroup->name, particleGroup));
}

void Emitter3D::RemoveParticleGroup(const std::string& groupName) {
	// particleGroups_からの削除
	auto it = particleGroups_.find(groupName);
	if (it != particleGroups_.end()) {
		particleGroups_.erase(it);

		auto nameIt = std::find(particleNames_.begin(), particleNames_.end(), groupName);
		if (nameIt != particleNames_.end()) {
			particleNames_.erase(nameIt);
		}
	} else {
		std::cerr << "Error: Particle group '" << groupName << "' not found!" << std::endl;
	}
}

void Emitter3D::SetEmitPosition(const Vector3& position) {
	transform_->SetTranslate(position);
}

EmitterSetting& Emitter3D::GetEmitterSetting() {
	return emitterSetting_;
}

std::string Emitter3D::GetName()const {
	return name_;
}

std::vector<std::string> Emitter3D::GetParticleNames() const {
	return particleNames_;
}
