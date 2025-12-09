#pragma once

// C++
#include <memory>

// MyHedder
#include "Math/Utility/MathUtility.h"

// 前方宣言
class GameObject3D;
class MechCore;

/// <summary>
/// 足
/// </summary>
class MechLeg {
public:
	// パラメータ
	struct Param {
		// 
		// ゲーム内で表示されるパラメータ
		// 

		// パーツの名前
		std::string name = "Leg";

		// 体力
		uint32_t ap = 2000;

		// 
		// 内部パラメータ
		// 

		// モデル名
		std::string modelName = "Leg";

		// オフセット座標
		Vector3 offSetPos = { 0.0f,0.0f,0.0f };

		// モデルサイズ
		Vector3 modelSize = { 0.0f,0.0f,0.0f };
	};
public:
	MechLeg();
	~MechLeg() = default;

	void Update(MechCore* mechCore);

	std::weak_ptr<GameObject3D> GetGameObject()const;

private:
	void RotateAngleAsVelocity(MechCore* core);

private:
	// オブジェクト
	std::weak_ptr<GameObject3D> leg_;

};