#define NOMINMAX

#include "Beam.h"

#include "Feature/Damage/CollisionSystem/DamageCollisionSystem.h"
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"

#include "MAGI.h"

using namespace Magi;

Beam::Beam(const Beam::InitParam& initParam, RefContext ref) :
	BaseLinearAttack(initParam.baseInit, ref.damageCollisionSystem) {

	// 初期化パラメータを受け取る
	thickness_ = initParam.thickness;
	effectManager_ = ref.effectManager;


	// エフェクト作成、追加


}

void Beam::Update() {
	// 基底クラスのアップデート
	BaseLinearAttack::Update();

}

void Beam::Draw() {

}

void Beam::Finalize() {
	BaseLinearAttack::Finalize();
}
