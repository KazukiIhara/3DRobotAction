#pragma once

// C++
#include <memory>
#include <string>

// MyHedder
#include "Math/Utility/MathUtility.h"

// 前方宣言
class GameObject3D;
class magi::Transform3D;
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
		magi::Vector3 offSetPos = { 0.0f,0.0f,0.0f };

		// モデルサイズ
		magi::Vector3 modelSize = { 0.0f,0.0f,0.0f };
	};
public:
	MechArmLeft();
	~MechArmLeft() = default;

	void Update(MechCore* mechCore);

	std::weak_ptr<GameObject3D> GetGameObject()const;

	magi::Transform3D* GetHandTransform();

	const magi::Vector3& GetForward()const;

	void DebugDraw();

private:
	// オブジェクト
	std::weak_ptr<GameObject3D> leftArm_;
	// 前方ベクトル
	magi::Vector3 forward_ = { 0.0f,0.0f,1.0f };

	// 肘のトランスフォーム
	magi::Transform3D* elbowTransform_;

	// 手のトランスフォーム
	magi::Transform3D* handTransform_;
};