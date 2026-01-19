#pragma once

// C++
#include <memory>

#include "Feature/Pilot/Mech/PilotMech.h"

#include "Feature/TPSCamera3D/TPSCamera3D.h"

// 前方宣言
class DamageObjectManager;
class GameEffectManager;
class MechAnimationContainer;

/// <summary>
/// パイロット(要はプレイヤー)
/// </summary>
class Pilot {
	struct Flag {
		bool isPause = false;
		bool isDebugDraw = false;
	};
public:
	Pilot(BaseMech::RefContext ref, TPSCamera3D* camera);
	~Pilot() = default;

	void Update();
	void Draw();

	PilotMech* GetMech();

	void LoadMechInitParam();

	Flag GetFlag()const;

	void SwitchDebugDraw();
	void SwitchIsPause();

private:
	// カメラ操作
	void CameraOperation();
private:
	// 機体
	std::unique_ptr<PilotMech> mech_;
	PilotMech::InitParam initParam_;

	// カメラの参照ポインタ
	TPSCamera3D* camera_ = nullptr;

	// フラグ構造体
	Flag flag_;

};