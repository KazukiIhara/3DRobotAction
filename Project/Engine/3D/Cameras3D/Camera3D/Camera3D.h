#pragma once

// C++
#include <string>
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
	Camera3D(const std::string& name, bool isUseYawPitch = true);
	Camera3D(const std::string& name, const Vector3& eye, float yaw, float pitch);
	Camera3D(const std::string& name, const Vector3& eye, const Vector3& target);
	virtual ~Camera3D();

	// 更新
	virtual void Update();
	// データ更新
	void UpdateData();

	// このカメラを現在のカメラに適用
	void ApplyCurrent();

	// アニメーション用のコントロールポイント追加
	void AddEyeControlPoint(const Vector3& eye);
	void AddTargetControlPoint(const Vector3& target);

	void SetEyeControlPoints(const std::vector<Vector3>& eyeCps);
	void SetTargetControlPoints(const std::vector<Vector3>& targetCps);

	// アニメーション開始
	void StartEyeAnimation();
	void StartTargetAnimation();
	// アニメーション停止
	void StopEyeAnimation();
	void StopTargetAnimation();

	// アニメーションさせる
	void PlayAnimation();

	// カメラを揺らす
	void Shake(float duration, const Vector3& intensity);
	void ApplyShake();

	// カメラのデバッグ描画
	void DrawFrustum();

	// 定数バッファに転送
	void TransferCamera(uint32_t rootParameterIndex);
	void TransferCameraInv(uint32_t rootParameterIndex);
	void TransferCameraFrustum(uint32_t rootParameterIndex);
	void TransferCameraVector(uint32_t rootParameterIndex);

	const std::string& GetName()const;
	Matrix4x4 GetViewProjectionMatrix()const;
	float GetFarClipRange()const;
	float GetYaw()const;
	float GetPitch()const;
	const Vector3& GetEye()const;
	const Vector3& GetTarget()const;


	void SetIsUseYawPitch(bool isUseYawPitch);
	void SetEye(const Vector3& eye);
	void SetTarget(const Vector3& target);
	void SetYaw(float yaw);
	void SetPitch(float pitch);
	void SetIsAlive(bool isAlive);
	void SetIsUnique(bool isUnique);
private:
	// カメラのリソースを作成
	void CreateCameraResource();
	// カメラのデータをマップ
	void MapCameraData();
	// カメラのデータを更新
	void UpdateCameraData();

protected:
	// 名前
	std::string name_;

	// カメラの初期トランスフォーム
	const Vector3 kDefaultCameraTranslate_ = { 0.0f,3.0f,-5.0f };
	const float kDefaultPitch_ = -0.5f;

	// カメラの位置
	Vector3 eye_ = { 0.0f,0.0f,0.0f };
	// カメラのターゲット
	Vector3 target_ = { 0.0f,0.0f,0.0f };
	// 上方向
	Vector3 up_ = { 0.0f,1.0f,0.0f };
	// カメラの前方ベクトル
	Vector3 forward_ = { 0.0f,0.0f,1.0f };
	// 右ベクトル
	Vector3 right_ = { 1.0f,0.0f,0.0f };

	// ヨー角
	float yaw_ = 0.0f;
	// ピッチ角
	float pitch_ = 0.0f;

	// ビューマトリックス
	Matrix4x4 viewMatrix_{};
	// プロジェクション行列
	Matrix4x4 projectionMatrix_{};

	// ビュープロジェクションマトリックス
	Matrix4x4 viewProjectionMatrix_{};

	// ビルボード行列
	Matrix4x4 billboardMatrix_{};
	// frustumPlane
	Vector4 frustumPlanes_[6];
	// yawPicthを使うかどうか
	bool isUseYawPitch_ = true;
	// 生存フラグ
	bool isAlive_ = true;
	// ユニークフラグ
	bool isUnique_ = false;

	// カメラシェイク用変数
	float shakeTime_ = 0;
	float shakeDuration_ = 0;
	Vector3 shakeIntensity_ = { 0.0f,0.0f,0.0f };
	Vector3 shakeStartTranslate_ = { 0.0f,0.0f,0.0f };
	Vector3 shakeCumulative_ = { 0.0f,0.0f,0.0f };


	//
	// パラメータ
	//

	// 垂直視野角(度数法)
	float fovYDegrees_ = 65.0f;
	// 垂直方向視野角
	float fovY_ = fovYDegrees_ * (std::numbers::pi_v<float> / 180.0f);
	// カリング用の垂直方向視野角
	float culFovY_ = 90.0f * (std::numbers::pi_v<float> / 180.0f);

	// アスペクト比
	float aspectRaito_ = 16.0f / 9.0f;
	// ニアクリップ距離
	float nearClipRange_ = 0.1f;
	// ファークリップ距離
	float farClipRange_ = 10000.0f;

	// 
	// カメラアニメーション変数
	// 

	// 視点のコントロールポイント
	std::vector<Vector3> eyeControlPoints_;
	// 注視点のコントロールポイント
	std::vector<Vector3> targetControlPoints_;

	// アニメーション再生
	bool isAnimatedEye_ = false;
	bool isAnimatedTarget_ = false;

	// アニメーション用t
	float animationEyeTime_ = 5.0f;
	float animationTargetTime_ = 5.0f;

	float animationEyeT_ = 0.0f;
	float animationTargetT_ = 0.0f;

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

	// CameraVector用リソース
	ComPtr<ID3D12Resource> vectorResource_ = nullptr;
	// CameraVector用データ
	CameraVector* vectorData_ = nullptr;

};