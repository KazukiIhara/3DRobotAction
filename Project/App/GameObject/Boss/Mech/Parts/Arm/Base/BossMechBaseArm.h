#pragma once

// C++
#include <string>

// MyHeader
#include "Structs/ModelStruct.h"
#include "GameObject/Boss/Mech/Parts/IBossMechParts.h"

// 前方宣言
class Transform3D;
class BossMech;

/// <summary>
/// ボス頭
/// </summary>
class BossMechBaseArm :public IBossMechParts {
public:
	// 初期化パラメータ構造体
	struct InitParam {
		// モデル名
		std::string upperModelName;
		std::string lowerModelName;
		std::string handModelName;
	};
	// 右左
	enum class Side {
		Left,
		Right
	};
public:
	BossMechBaseArm(const BossMechBaseArm::InitParam& param, BossMech* mech);
	~BossMechBaseArm()override = default;

	virtual void Update()override;
	void Draw()override;

	void DebugDraw()override;

	Transform3D* GetUpperTransform();
	Transform3D* GetLowerTransform();
	Transform3D* GetHandTransform();

protected:
	// 左右どっちなのか
	Side side_;
private:
	// 上腕の情報
	std::string upperModelName_;
	Transform3D* upperTrans_;

	// 下腕の情報
	std::string lowerModelName_;
	Transform3D* lowerTrans_;

	// 手の情報
	std::string handModelName_;
	Transform3D* handTrans_;

	// ひとまず共通のマテリアル
	ModelMaterial armMat_;

	// 機体の参照ポインタ
	BossMech* mech_;
};