#pragma once

// C++
#include <array>
#include <memory>

#include "Feature/Effect/System/BaseGameEffect/BaseGameEffect.h"
#include "Structs/Primitive3DStruct.h"
#include "Math/Types/AllMathTypes.h"

// 前方宣言
class Transform3D;

// 実装メモ

/*
	発生地点とレーザーの頂点座標からエフェクトを作る
	ひとまずシリンダーを置いてみる
*/

/// <summary>
/// レーザー攻撃のエフェクト
/// </summary>
class LaserEffect:public BaseGameEffect {
public:
	enum class Col {
		RED,
		BLUE,
	};
	struct InitParam {
		Vector3 emitPos{};
		Vector3 dir{};
		float life = 0.0f;
		Col color = Col::RED;
	};
public:
	LaserEffect(const InitParam& initParam);
	~LaserEffect()override = default;

	void Update()override;
	void Draw()override;
	void Finalize()override;

	void SetLaserEndPos(const Vector3& endPos);

private:
	void DebugUpdate();

private:
	// 色
	Col color_ = Col::RED;

	// タイマー
	float life_ = 0.0f;
	// 方向
	Vector3 dir_{};

	// レーザーの親トランスフォーム
	Transform3D* laserParent_;

	// レーザーの終点
	Vector3 laserEndPos_ = {};

	// 板ポリ
	std::array<Transform3D*, 2> planeTrans_;
	std::array<PlaneData3D, 2> planeData_;
	MaterialData3D planeMat_;

	// シリンダー
	Transform3D* cylinderTrans_;
	CylinderData3D cylinderData_;
	MaterialData3D cylinderMat_;

};