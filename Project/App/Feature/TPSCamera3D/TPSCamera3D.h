#pragma once

#include "Cameras3D/Camera3D/Camera3D.h"

class ILockOnTarget;

namespace Magi {

	class Transform3D;

	class TPSCamera3D : public Magi::Camera3D {
	public:
		struct Param {
			float distance = 6.0f;
			float height = 2.0f;
			float shoulder = 0.6f;

			float yawSpeed = 2.5f;
			float pitchSpeed = 2.0f;

			float pitchMin = -1.2f;
			float pitchMax = 0.6f;

			float posSmooth = 14.0f;
			float targetSmooth = 18.0f;
		};

	public:
		TPSCamera3D(const std::string& name);
		~TPSCamera3D() override = default;

		void Update() override;

		void SetFollowTransform(Transform3D* follow);
		void SetLockOnTarget(ILockOnTarget* lockOn);

		void AddYaw(float delta);
		void AddPitch(float delta);

		Param& GetParam();
		const Param& GetParam() const;

	private:
		void LoadParam();

		// 追従点
		Vector3 CalcPivot() const;

		// 注視点
		Vector3 CalcLookAt(const Vector3& pivot) const;

		// カメラ位置
		Vector3 CalcDesiredEye(const Vector3& pivot) const;

		// ロックオン角度更新
		void UpdateLockOnAngles(const Vector3& pivot, float dt);

		// ロックオン入力オフセット初期化
		void ResetLockOffset();

	private:
		Transform3D* follow_ = nullptr;
		ILockOnTarget* lockOn_ = nullptr;

		Param param_{};

		Vector3 smoothedEye_{};
		Vector3 smoothedTarget_{};

		float lockOffsetYaw_ = 0.0f;
		float lockOffsetPitch_ = 0.0f;
		bool wasLockOn_ = false;
	};
}
