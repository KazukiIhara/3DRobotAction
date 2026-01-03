#pragma once

// C++
#include <memory>
#include <string>
#include <cstdint>

// MyHeader
#include "Math/Utility/MathUtility.h"

// 前方宣言
class GameObject3D;
class Transform3D;
class MechCore;

/// <summary>
/// 機体の腕基底クラス
/// </summary>
class BaseMechArm {
public:
	struct Param {
		// ゲーム内で表示されるパラメータ
		std::string name = "Arm";
		uint32_t ap = 2000;

		// 内部パラメータ
		std::string modelName = "Arm";
		Vector3 offSetPos = { 0.0f,0.0f,0.0f };
		Vector3 modelSize = { 0.0f,0.0f,0.0f };
	};

public:
	explicit BaseMechArm(const Param& param);
	virtual ~BaseMechArm() = default;

	void Update(MechCore* mechCore);

	std::weak_ptr<GameObject3D> GetGameObject() const;

	Transform3D* GetShoulderTransform();
	Transform3D* GetHandTransform();

	const Vector3& GetForward() const;

	void DebugDraw();

protected:
	virtual Vector3 GetFireOffsetWorldPos(MechCore* mechCore) const = 0;
	virtual float GetBulletSpeed(MechCore* mechCore) const = 0;

private:
	void UpdateAimToTarget(MechCore* mechCore);
	void UpdateAimToCamera(MechCore* mechCore);

protected:
	Param param_;

	std::weak_ptr<GameObject3D> arm_;
	Vector3 forward_ = { 0.0f,0.0f,1.0f };

	Transform3D* shoulderTransform_ = nullptr;
	Transform3D* handTransform_ = nullptr;

};
