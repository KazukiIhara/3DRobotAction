#pragma once

// C++
#include <string>

// MyHeader
#include "Structs/ModelStruct.h"

// 前方宣言
class Transform3D;
class BossMech;

/// <summary>
/// ボス胴体
/// </summary>
class BossMechBody {
public:
	struct InitParam {
		// モデル名
		std::string bodyModelName;
	};

public:
	BossMechBody(const BossMechBody::InitParam& param, BossMech* mech);
	~BossMechBody() = default;

	void Update();
	void Draw();

	void DebugDraw();

	Transform3D* GetBodyTransform();

private:
	// 胴体の情報
	std::string bodyModelName_;
	Transform3D* bodyTrans_;
	ModelMaterial bodyMat_;

	BossMech* mech_;
};