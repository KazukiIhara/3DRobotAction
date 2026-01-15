#pragma once

// C++
#include <string>

// MyHeader
#include "Structs/ModelStruct.h"

// 前方宣言
class Transform3D;
class BossMech;

/// <summary>
/// ボス頭
/// </summary>
class BossMechBaseArm {
public:
	struct InitParam {
		// モデル名
		std::string upperModelName;
		std::string lowerModelName;
		std::string handModelName;
	};
public:
	BossMechBaseArm(const BossMechBaseArm::InitParam& param, BossMech* mech);
	~BossMechBaseArm() = default;

	virtual void Update();
	void Draw();

	Transform3D* GetUpperTransform();
	Transform3D* GetLowerTransform();
	Transform3D* GetHandTransform();

private:
	// 上腕の情報
	std::string upperModelName_;
	Transform3D* upperTrans_;

	// 下腕の情報
	std::string lowerModelName_;
	Transform3D* lowerTrans_;

	// 手の情報
	std::string headModelName_;
	Transform3D* headTrans_;

	// ひとまず共通のマテリアル
	ModelMaterial armMat_;

	// 機体の参照ポインタ
	BossMech* mech_;
};