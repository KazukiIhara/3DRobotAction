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
		std::string modelName;
	};
public:
	BossMechHead(const BossMechHead::InitParam& param, BossMech* mech);
	~BossMechHead() = default;

	void Update();
	void Draw();

	Transform3D* GetHeadTransform();

private:
	Transform3D* headTrans_;
	BossMech* mech_;
};