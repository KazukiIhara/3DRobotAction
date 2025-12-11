#pragma once

// C++
#include <memory>
#include <string>

// MyHedder
#include "Math/Utility/MathUtility.h"


namespace magi {
// 前方宣言
class GameObject3D;
class Transform3D;
class MechCore;

/// <summary>
/// 左腕
/// </summary>
class MechArmLeft {
public:
	// パラメータ
	struct Param {
		// 
		// ゲーム内で表示されるパラメータ
		// 

		// パーツの名前
		std::string name = "ArmL";

		// 体力
		uint32_t ap = 2000;

		// 
		// 内部パラメータ
		// 

		// モデル名
		std::string modelName = "ArmL";

		// オフセット
		Vector3 offSetPos = { 0.0f,0.0f,0.0f };

		// モデルサイズ
		Vector3 modelSize = { 0.0f,0.0f,0.0f };
	};
public:
	MechArmLeft();
	~MechArmLeft() = default;

	void Update(MechCore* mechCore);

	std::weak_ptr<GameObject3D> GetGameObject()const;

	Transform3D* GetHandTransform();

	const Vector3& GetForward()const;

	void DebugDraw();

private:
	// オブジェクト
	std::weak_ptr<GameObject3D> leftArm_;
	// 前方ベクトル
	Vector3 forward_ = { 0.0f,0.0f,1.0f };

	// 肘のトランスフォーム
	Transform3D* elbowTransform_;

	// 手のトランスフォーム
	Transform3D* handTransform_;
};
}
