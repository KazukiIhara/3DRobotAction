#include "MechJustDodgeComponent.h"

#include "MAGI.h"

using namespace Magi;

MechJustDodgeComponent::MechJustDodgeComponent(MechCore* core) {
	core_ = core;

	// パラメータ作成
	MAGISYSTEM::AddParameterData({ "MechParam","JustDodge","ColliderRadius" }, ParamType::Float);


}


void MechJustDodgeComponent::Update() {
	// パラメータ取得
	collider_.radius = MAGISYSTEM::GetParameterValue<float>({ "MechParam","JustDodge","ColliderRadius" });

	// 当たり判定を取る


}

void MechJustDodgeComponent::DrawDebug() {
	// デバッグ描画

}
