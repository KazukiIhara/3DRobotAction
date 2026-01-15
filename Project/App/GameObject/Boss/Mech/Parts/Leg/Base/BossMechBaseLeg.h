#pragma once

// C++
#include <string>

// MyHeader
#include "Structs/ModelStruct.h"

// 前方宣言
class Transform3D;
class BossMech;

/// <summary>
/// ボス足
/// </summary>
class BossMechBaseLeg {
public:
	struct InitParam {
		// モデル名
		std::string upperModelName;
		std::string lowerModelName;
		std::string footModelName;
	};
	// 右左
	enum class Side {
		Left,
		Right
	};
public:
	BossMechBaseLeg(const BossMechBaseLeg::InitParam& param, BossMech* mech);
	~BossMechBaseLeg() = default;

	virtual void Update();
	void Draw();

	void DebugDraw();

	Transform3D* GetUpperTransform();
	Transform3D* GetLowerTransform();
	Transform3D* GetFootTransform();

protected:
	// 左右どっちなのか
	Side side_;

private:
	// 上足の情報
	std::string upperModelName_;
	Transform3D* upperTrans_;

	// 下足の情報
	std::string lowerModelName_;
	Transform3D* lowerTrans_;

	// 足の情報
	std::string footModelName_;
	Transform3D* footTrans_;

	// ひとまず共通のマテリアル
	ModelMaterial legMat_;

	// 機体の参照ポインタ
	BossMech* mech_;
};