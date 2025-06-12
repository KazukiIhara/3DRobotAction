#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Structs/EmitterStruct.h"

#include "3D/ParticleGroups3D/BaseParticleGroup3D/BaseParticleGroup3D.h"
#include "Transform3D/Transform3D.h"

/// <summary>
/// 3Dパーティクルエミッター
/// </summary>
class Emitter3D {
public:
	Emitter3D(const std::string& name, const Vector3& position);
	~Emitter3D();

	// 初期化
	void Initialize(const Vector3& position);

	// 更新
	void Update();

	// パーティクル発生
	void EmitAll();

	// 新規パーティクルグループ追加
	void AddParticleGroup(BaseParticleGroup3D* particleGroup);

	// パーティクルグループ削除
	void RemoveParticleGroup(const std::string& groupName);

	void SetEmitPosition(const Vector3& position);

	// 設定構造体の取得
	EmitterSetting& GetEmitterSetting();

	std::string GetName()const;

	std::vector<std::string> GetParticleNames() const;

public:
	// トランスフォーム
	std::unique_ptr<Transform3D> transform_{};
private:
	// エミッターの設定構造体
	EmitterSetting emitterSetting_{};
	std::string name_;
	// パーティクルの名前リスト
	std::vector<std::string> particleNames_;
	// パーティクルグループのマップ
	std::map<std::string, BaseParticleGroup3D*> particleGroups_;

	// 頻度用時刻
	float frequencyTimer_ = 0.0f;
};