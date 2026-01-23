#pragma once

// 基底クラス
#include "Feature/Pilot/Mech/State/IPilotMechState.h"

// 実装メモ
/*
	ジャスト回避の後の攻撃　太めのビームを放つ
	主にエクバを見てリファレンスを決める
	一番こだわりたい攻撃であとから変わりそうなので、まずはパッと作る
*/

/// <summary>
/// ジャスト回避後攻撃ステート
/// </summary>
class PilotMechStateJustDodgeAttack:public IPilotMechState {
public:
	PilotMechStateJustDodgeAttack();
	~PilotMechStateJustDodgeAttack() = default;
	void Enter(PilotMech* mech)override;
	void Update(PilotMech* mech)override;
	void Exit(PilotMech* mech)override;

private:

};