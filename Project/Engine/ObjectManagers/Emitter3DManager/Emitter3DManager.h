#pragma once

#include <vector>
#include <string>
#include <memory>

#include "3D/Emitter3D/Emitter3D.h"

/// <summary>
/// 3Dエミッターマネージャ
/// </summary>
class Emitter3DManager {
public:
	Emitter3DManager();
	~Emitter3DManager();

	void Update();

	// エミッター作成
	std::string CreateEmitter(const std::string& emitterName, const Vector3& position);
	// 名前からエミッターを削除する
	void Remove(const std::string& emitterName);
	// 名前からエミッターを検索する
	Emitter3D* Find(const std::string& emitterName);
	// エミッターをクリア
	void Clear();
	// 削除フラグが立っているエミッターを削除
	void DeleteGarbage();

private:
	// エミッターコンテナ
	std::vector<std::unique_ptr<Emitter3D>> emitters3D_;
};