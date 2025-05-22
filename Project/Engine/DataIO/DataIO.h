#pragma once

// C++
#include <string>
#include <vector>
#include <memory>

// 前方宣言
class BaseCollider3D;
class Renderer3DManager;
class ColliderManager;
class GameObject3DManager;

/// <summary>
/// データ入出力クラス
/// </summary>
class DataIO {
public:
	DataIO(
		Renderer3DManager* renderer3DManager,
		ColliderManager* colliderManager,
		GameObject3DManager* gameObject3DManager
	);
	~DataIO();

	// 初期化
	void Initialize();

	// フレーム開始時の処理
	void BeginFrame();
	// フレーム終了時の処理
	void EndFrame();

	// 
	// 所有しているマネージャのインスタンスを送る
	// 

	// 3D描画マネージャ
	Renderer3DManager* GetRenderer3DManager();
	// コライダーマネージャ
	ColliderManager* GetColliderManager();
	// 3Dゲームオブジェクトマネージャ
	GameObject3DManager* GetGameObject3DManager();

private:


private:
	// 3D描画マネージャのセット
	void SetRenderer3DManager(Renderer3DManager* renderer3DManager);
	// コライダーマネージャのセット
	void SetColliderManager(ColliderManager* colliderManager);
	// 3Dゲームオブジェクトマネージャをセット
	void SetGameObject3DManager(GameObject3DManager* gameObject3DManager);
private:
	// 
	// マネージャクラスのポインタ
	// 

	// 3D描画マネージャクラス
	Renderer3DManager* renderer3DManager_ = nullptr;
	// コライダーマネージャクラス
	ColliderManager* colliderManager_ = nullptr;
	// 3Dゲームオブジェクトマネージャ
	GameObject3DManager* gameObject3DManager_ = nullptr;
};