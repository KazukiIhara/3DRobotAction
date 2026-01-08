#pragma once


#include <memory>

#include "Math/Types/AllMathTypes.h"

class MechCore;
class AttackCollisionManager;

// 実装メモ
/*
	MechCoreに持たせる
	QuickBoost中判定が有効になるようにする
	攻撃判定マネージャを参照して衝突していれば弾を削除し、
	機体がジャスト回避ステートに入るようにする
*/

/// <summary>
/// ジャスト回避判定クラス
/// </summary>
class MechJustDodgeSystem {
public:
	// コライダー
	struct Collider {
		Vector3 worldPos{};
		float radius{};
	};

public:
	MechJustDodgeSystem(MechCore* core);
	~MechJustDodgeSystem() = default;

	void Update();

	// デバッグ描画関数
	void DrawDebug();

private:
	// コライダー
	Collider collider_{};
	// 機体の参照用ポインタ
	MechCore* core_;
};