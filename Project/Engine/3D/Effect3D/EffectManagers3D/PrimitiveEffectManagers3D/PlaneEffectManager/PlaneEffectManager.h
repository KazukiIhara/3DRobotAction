#pragma once

// C++
#include <vector>

// MyHedder
#include "Structs/EffectStruct.h"

// 前方宣言
class DeltaTimer;
class TransformManager;
class PlaneDrawer3D;

/// <summary>
/// 板ポリエフェクトマネージャ
/// </summary>
class PlaneEffectManager {
public:
	PlaneEffectManager(DeltaTimer* deltaTimer, TransformManager* transformManager, PlaneDrawer3D* planeDrawer);
	~PlaneEffectManager();

	void Add(const PlaneEffectParam& param);
	void Update();
	void Draw();
	void DeleteGarbage();

private:
	// 板ポリエフェクトのリスト
	std::vector<PlaneEffect> effects_;
	// 現在のインデックス
	uint32_t currentIndex_ = 0;
	// インスタンスの数
	uint32_t instanceCount_ = 0;

private:
	DeltaTimer* deltaTimer_ = nullptr;
	TransformManager* transformManager_ = nullptr;
	PlaneDrawer3D* planeDrawer_ = nullptr;
};