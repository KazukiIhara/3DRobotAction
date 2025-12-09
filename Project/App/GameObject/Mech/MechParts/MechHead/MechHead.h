#pragma once

// C++
#include <memory>
#include <string>

// MyHedder
#include "Math/Utility/MathUtility.h"

// 前方宣言
class GameObject3D;
class MechCore;

/// <summary>
/// 頭
/// </summary>
class MechHead {
public:
	// パラメータ
	struct Param {
		// 
		// ゲーム内で表示されるパラメータ
		// 

		// パーツの名前
		std::string name = "Head";

		// 体力
		uint32_t ap = 2000;

		// 
		// 内部パラメータ
		// 

		// モデル名
		std::string modelName = "Head";

		// オフセット
		Vector3 offSetPos = { 0.0f,0.0f,0.0f };

		// モデルサイズ
		Vector3 modelSize = { 0.0f,0.0f,0.0f };
	};
public:
	MechHead();
	~MechHead() = default;

	void Update(MechCore* mechCore);

	std::weak_ptr<GameObject3D> GetGameObject()const;

private:
	// オブジェクト
	std::weak_ptr<GameObject3D> head_;

};