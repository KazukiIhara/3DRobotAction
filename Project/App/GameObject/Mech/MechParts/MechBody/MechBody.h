#pragma once

// C++
#include <memory>
#include <string>

// MyHedder
#include "Math/Utility/MathUtility.h"
#include "GameCommon/GameCommon.h"

// 前方宣言
class GameObject3D;
class MechCore;

/// <summary>
/// 胴体
/// </summary>
class MechBody {
public:
	// パラメータ
	struct Param {
		// 
		// ゲーム内で表示されるパラメータ
		// 

		// パーツの名前
		std::string name = "Body";

		// 体力
		uint32_t ap = 2000;

		// 
		// 内部パラメータ
		// 

		// モデル名
		std::string modelName = "Body";

		// オフセット
		Vector3 offSetPos = { 0.0f,0.0f,0.0f };

	};

public:
	MechBody();
	~MechBody() = default;

	void Update(MechCore* mechCore);

	std::weak_ptr<GameObject3D> GetGameObject()const;

private:
	void DirectionToLockOnView(MechCore* mechCore);

	void RotateToMoveDirection(MechCore* mechCore);

	void RotateToQuickBoost(MechCore* mechCore);

private:
	// オブジェクト
	std::weak_ptr<GameObject3D> body_;

};