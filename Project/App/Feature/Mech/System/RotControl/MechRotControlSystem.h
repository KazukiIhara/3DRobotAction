#pragma once

// 機体
class BaseMech;

/// <summary>
/// 機体の向きをコントロールするクラス
/// </summary>
class MechRotControlSystem {
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
	MechRotControlSystem(BaseMech* mech);
	~MechRotControlSystem() = default;

	void Update();

	LookAtFlag& GetLookAtFlag();

	void SetTurnToMoveDir(bool flag);

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