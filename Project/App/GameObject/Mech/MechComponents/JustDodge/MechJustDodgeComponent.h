#pragma once


#include <memory>

#include "Math/Types/AllMathTypes.h"

class MechCore;
class AttackCollisionManager;

// 実装メモ
/*
	MechCoreに持たせる
	QuickBoost中判定が有効になるようにする
	攻撃判定マネージャを参照して衝突していれば弾の削除フラグをオンにし、
	機体がジャスト回避ステートに入るようにする
*/

/// <summary>
/// ジャスト回避判定クラス
/// </summary>
class MechJustDodgeComponent {
public:
	// コライダー
	struct Collider {
		Vector3 worldPos{};
		float radius{};
	};

public:
	MechJustDodgeComponent(MechCore* core);
	~MechJustDodgeComponent() = default;

	// QuickBoost中に呼ぶ
	void Update();

	// デバッグ描画関数
	void DrawDebug();

private:
	// コライダー
	Collider collider_{};
	// 機体の参照用ポインタ
	MechCore* core_;
};