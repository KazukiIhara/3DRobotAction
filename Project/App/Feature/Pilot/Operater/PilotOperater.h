#pragma once

// 前方宣言
class PilotMech;
class TPSCamera3D;

/// <summary>
/// パイロット操作クラス
/// </summary>
class PilotOperator {
public:
	PilotOperator(PilotMech* mech, TPSCamera3D* camera);
	~PilotOperator() = default;

	void Update();

private:
	void CameraOperation();

private:
	PilotMech* mech_;
	TPSCamera3D* camera_;

};