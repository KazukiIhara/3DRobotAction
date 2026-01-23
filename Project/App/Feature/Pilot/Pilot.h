#pragma once

// C++
#include <memory>

// 機体クラス
#include "Feature/Pilot/Mech/PilotMech.h"

// 前方宣言
class DamageObjectManager;
class GameEffectManager;
class MechAnimationContainer;
class GameInputSystem;
class TPSCamera3D;

// 実装メモ
/*
	実質プレイヤークラス
	名称衝突をさけるために命名を変えた　
	コード整理後名称をプレイヤーにするかもしれない
*/

/// <summary>
/// パイロット
/// </summary>
class Pilot {
public:
	struct Flag {
		bool isPause = false;
		bool isDebugDraw = false;
	};
	struct RefContext {
		TPSCamera3D* camera;
		GameInputSystem* inputSys;
	};
public:
	Pilot(BaseMech::RefContext ref, RefContext pRef);
	~Pilot() = default;

	void Update();
	void Draw();

	PilotMech* GetMech();

	Flag GetFlag()const;

	void SwitchDebugDraw();
	void SwitchIsPause();

	GameInputSystem* GetInputSys();

private:
	// 機体初期化パラメータ取得
	void LoadMechInitParam();
	// カメラ操作
	void CameraControl();
private:
	// 機体
	std::unique_ptr<PilotMech> mech_;
	PilotMech::InitParam initParam_;

	// カメラの参照ポインタ
	TPSCamera3D* camera_ = nullptr;
	// インプットシステムの参照ポインタ
	GameInputSystem* inputSys_ = nullptr;

	// フラグ構造体
	Flag flag_;

};