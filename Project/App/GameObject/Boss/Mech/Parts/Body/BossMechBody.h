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
/// ボス胴体
/// </summary>
class BossMechBody:public IBossMechParts {
public:
	struct InitParam {
		// モデル名
		std::string modelName;
		Vector3 translate;
	};

public:
	BossMechBody(const BossMechBody::InitParam& param, BossMech* mech);
	~BossMechBody()override = default;

	void Update()override;
	void Draw()override;

	void DebugDraw()override;

	void SetInitParam(const InitParam& param);
	void SetInitTranslate(const InitParam& param);

	Transform3D* GetTransform();

private:
	// 胴体の情報
	std::string bodyModelName_;
	Transform3D* bodyTrans_;
	ModelMaterial bodyMat_;

	BossMech* mech_;
};