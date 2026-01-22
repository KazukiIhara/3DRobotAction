#pragma once

class BaseMech;

/// <summary>
/// 機体の向きをコントロールするクラス
/// </summary>
class MechDirController {
public:
	// ターゲット座標に向きを向けるフラグ
	struct LookAt {
		bool yaw = false;
		bool pitch = false;
	};
	struct LookAtFlag {
		// 全身
		LookAt mech{};
		// 頭
		LookAt head{};
		// 体
		LookAt body{};
	};
public:
	MechDirController(BaseMech* mech);
	~MechDirController() = default;

	void Update();

	LookAtFlag& GetLookAtFlag();
private:
	void TurnToMoveDirection();
	void RotateMech();
	void RotateHead();
	void RotateBody();
private:
	LookAtFlag lookAtFlag_{};
	bool turnToMoveDir_ = false;
	BaseMech* mech_ = nullptr;
};