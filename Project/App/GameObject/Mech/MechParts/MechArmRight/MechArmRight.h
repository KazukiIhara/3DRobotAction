#pragma once

// C++
#include <memory>
#include <string>

// MyHedder
#include "Math/Utility/MathUtility.h"

// 前方宣言
class GameObject3D;
class Transform3D;
class MechCore;

/// <summary>
/// 右腕
/// </summary>
class MechArmRight {
public:
	// パラメータ
	struct Param {
		// 
		// ゲーム内で表示されるパラメータ
		// 

		// パーツの名前
		std::string name = "ArmR";

		// 体力
		uint32_t ap = 2000;

		// 
		// 内部パラメータ
		// 

		// モデル名
		std::string modelName = "ArmR";

		// オフセット
		Vector3 offSetPos = { 0.0f,0.0f,0.0f };

		// モデルサイズ
		Vector3 modelSize = { 0.0f,0.0f,0.0f };
	};
public:
	MechArmRight();
	~MechArmRight() = default;

	void Update(MechCore* mechCore);

	std::weak_ptr<GameObject3D> GetGameObject()const;

	Transform3D* GetHandTransform();

	const Vector3& GetForward()const;

private:
	// オブジェクト
	std::weak_ptr<GameObject3D> rightArm_;
	// 前方ベクトル
	Vector3 forward_ = { 0.0f,0.0f,1.0f };

	// 武器を親子付けするトランスフォーム
	Transform3D* handTransform_;
};