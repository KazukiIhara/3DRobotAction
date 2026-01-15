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
class BossMechHead {
public:
	struct InitParam {
		// モデル名
		std::string modelName;
	};
public:
	BossMechHead(const BossMechHead::InitParam& param, BossMech* mech);
	~BossMechHead() = default;

	void Update();
	void Draw();

	void DebugDraw();

	Transform3D* GetHeadTransform();

private:
	// 頭の情報
	std::string headModelName_;
	Transform3D* headTrans_;
	ModelMaterial headMat_;

	BossMech* mech_;
};