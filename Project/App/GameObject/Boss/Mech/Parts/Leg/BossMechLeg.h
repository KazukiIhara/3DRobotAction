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
/// ボス足
/// </summary>
class BossMechLeg:public IBossMechParts {
public:
	struct InitParam {
		// 腰モデル名
		std::string waistModelName;
		Vector3 waistTranslate;

		// 上足モデル名
		std::string upperModelNameLeft;
		Vector3 upperTranslateLeft;
		std::string upperModelNameRight;
		Vector3 upperTranslateRight;

		// 下足モデル名
		std::string lowerModelNameLeft;
		Vector3 lowerTranslateLeft;
		std::string lowerModelNameRight;
		Vector3 lowerTranslateRight;

		// 足モデル名
		std::string footModelNameLeft;
		Vector3 footTranslateLeft;
		std::string footModelNameRight;
		Vector3 footTranslateRight;

	};
public:
	BossMechLeg(const BossMechLeg::InitParam& param, BossMech* mech);
	~BossMechLeg()override = default;

	void Update()override;
	void Draw()override;

	void DebugDraw()override;

	void SetInitParam(const InitParam& param);
	void SetInitTranslate(const InitParam& param);

	Transform3D* GetWaistTransform();

	Transform3D* GetUpperTransformLeft();
	Transform3D* GetUpperTransformRight();

	Transform3D* GetLowerTransformLeft();
	Transform3D* GetLowerTransformRight();

	Transform3D* GetFootTransformLeft();
	Transform3D* GetFootTransformRight();

private:
	// 腰の情報
	std::string waistModelName_;
	Transform3D* waistTrans_;

	// 上足の情報
	std::string upperModelNameLeft_;
	std::string upperModelNameRight_;
	Transform3D* upperTransLeft_;
	Transform3D* upperTransRight_;

	// 下足の情報
	std::string lowerModelNameLeft_;
	std::string lowerModelNameRight_;
	Transform3D* lowerTransLeft_;
	Transform3D* lowerTransRight_;

	// 足の情報
	std::string footModelNameLeft_;
	std::string footModelNameRight_;
	Transform3D* footTransLeft_;
	Transform3D* footTransRight_;

	// ひとまず共通のマテリアル
	ModelMaterial legMat_;

	// 機体の参照ポインタ
	BossMech* mech_;
};