#pragma once

// C++
#include <memory>

// DirectX
#include <d3d12.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"

#include "Structs/CameraStruct.h"

/// <summary>
/// 3D用カメラ
/// </summary>
class Camera3D {
public:
	Camera3D();
	virtual ~Camera3D();

	// 初期化
	virtual void Initialize();
	// 更新
	virtual void Update();
	// データ更新
	virtual void UpdateData();

	// カメラを揺らす
	void Shake(float duration, float intensity);
	void ApplyShake();

	// カメラのデバッグ描画
	void DrawFrustum();

	// 定数バッファに転送
	void TransferCamera(uint32_t rootParameterIndex);

	void TransferCameraInv(uint32_t rootParameterIndex);

	void TransferCameraFrustum(uint32_t rootParameterIndex);

	// ビュープロジェクションマトリックスを送る
	Matrix4x4 GetViewProjectionMatrix()const;

	// ファークリップ距離を送る
	float GetFarClipRange()const;

	// 視点を送る
	Vector3 GetEye()const;

	float GetYaw()const;
	float GetPitch()const;

	void SetIsUseYawPitch(bool isUseYawPitch);
	void SetEye(const Vector3& eye);
	void SetTarget(const Vector3& target);
	void SetYaw(float yaw);
	void SetPitch(float pitch);

private:
	// カメラのリソースを作成
	void CreateCameraResource();
	// カメラのデータをマップ
	void MapCameraData();
	// カメラのデータを更新
	void UpdateCameraData();

protected:
	// カメラの初期トランスフォーム
	const Vector3 kDefaultCameraTranslate_ = { 0.0f,3.0f,-5.0f };
	const float kDefaultPitch_ = -0.5f;

	// カメラの位置
	Vector3 eye_ = { 0.0f,0.0f,0.0f };
	// カメラのターゲット
	Vector3 target_ = { 0.0f,0.0f,0.0f };
	// 上方向
	Vector3 up_ = { 0.0f,1.0f,0.0f };

	float yaw_ = 0.0f;
	float pitch_ = 0.0f;

	// ビューマトリックス
	Matrix4x4 viewMatrix_{};
	// プロジェクション行列
	Matrix4x4 projectionMatrix_{};
	// ビュープロジェクションマトリックス
	Matrix4x4 viewProjectionMatrix_{};
	// 水平視野角(度数法)
	float fovYDegrees_ = 60.0f;
	// 水平方向視野角
	float fovY_ = fovYDegrees_ * (std::numbers::pi_v<float> / 180.0f);
	// アスペクト比
	float aspectRaito_ = 16.0f / 9.0f;
	// ニアクリップ距離
	float nearClipRange_ = 0.1f;
	// ファークリップ距離
	float farClipRange_ = 10000.0f;
	// ビルボード行列
	Matrix4x4 billboardMatrix_{};
	// frustumPlane
	Vector4 frustumPlanes_[6];
	// 有効フラグ
	bool isActive_ = true;
	// yawPicthを使うかどうか
	bool isUseYawPitch_ = true;

	// カメラシェイク用変数
	float shakeTime_ = 0;
	float shakeDuration_ = 0;
	float shakeIntensity_ = 0.0f;
	Vector3 shakeStartTranslate_ = { 0.0f,0.0f,0.0f };
private:
	// Camera用リソース
	ComPtr<ID3D12Resource> cameraResource_ = nullptr;
	// Camera用データ
	Camera3DForGPU* cameraData_ = nullptr;

	// CameraInv用リソース
	ComPtr<ID3D12Resource> cameraInvResource_ = nullptr;
	// CameraInv用データ
	Camera3DInverseForGPU* cameraInvData_ = nullptr;

	// CameraFrustum用リソース
	ComPtr<ID3D12Resource> frustumResource_ = nullptr;
	// CameraFrustum用データ
	Camera3DFrustumForGPU* frustumData_ = nullptr;
};