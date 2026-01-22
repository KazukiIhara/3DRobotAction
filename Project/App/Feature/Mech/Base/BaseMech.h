#pragma once

// C++
#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// 数学
#include "Math/Utility/MathUtility.h"

// ロックオン対象のインターフェース
#include "Feature/ILockOnTarget/ILockOnTarget.h"
// 機体のパーツ
#include "Feature/Mech/Parts/Head/MechPartsHead.h"
#include "Feature/Mech/Parts/Body/MechPartsBody.h"
#include "Feature/Mech/Parts/Arm/MechPartsArm.h"
#include "Feature/Mech/Parts/Leg/MechPartsLeg.h"
// 武器基底クラス
#include "Feature/Mech/Weapon/BaseMechWeapon.h"
// アニメーションクラス
#include "MechAnimation/Animator/MechAnimator.h"
// 回転制御クラス
#include "Feature/Mech/DirController/MechDirController.h"
// コライダークラス
#include "Feature/Mech/Collider/MechCollider.h"
// 汎用ヘッダ
#include "GameCommon/GameCommon.h"

// マネージャ、システムの前方宣言
class DamageObjectManager;
class DamageCollisionSystem;
class GameEffectManager;
class MechAnimationContainer;

class BaseMech: public ILockOnTarget {
public:
	struct InitParam {
		Vector3 position{};
		FriendlyTag tag;
		MechPartsHead::InitParam head;
		MechPartsBody::InitParam body;
		MechPartsArm::InitParam armR;
		MechPartsArm::InitParam armL;
		MechPartsLeg::InitParam leg;
	};
	struct DebugFlag {
		bool isDebugDraw = false;
		bool editPartsTransform = false;
		bool stopUpdate = false;
	};

	struct RefContext {
		DamageObjectManager* damageObjectManager = nullptr;
		GameEffectManager* gameEffectManager = nullptr;
		DamageCollisionSystem* damageCollisionSystem = nullptr;
		MechAnimationContainer* animationContainer = nullptr;
	};

public:
	BaseMech(const InitParam& param, const RefContext& ref);
	virtual ~BaseMech() = default;

	virtual void Update(bool isShowDebugUI, const BaseMech::InitParam& param);
	void Draw();
	virtual void DebugDraw();
	virtual void ShowDebugWindow() {};
	void DebugUpdate(bool isShowDebugUI, const BaseMech::InitParam& param);

	// トランスフォームを取得
	Transform3D* GetTransform();
	Transform3D* GetModelTransform();

	// 標準パーツアクセッサ
	MechPartsHead* GetHead();
	MechPartsBody* GetBody();
	MechPartsArm* GetArmL();
	MechPartsArm* GetArmR();
	MechPartsLeg* GetLeg();

	// 関節トランスフォーム取得
	Transform3D* GetPartsTransform(MechAnimation::TransType type);
	// 中心座標取得
	const Vector3& GetCenterPos()override;
	// 武器
	BaseMechWeapon* GetWeapon(const std::string& name);

	// アニメーター
	MechAnimator* GetAnimator();
	// 回転制御クラス
	MechDirController* GetDirController();
	// コライダー取得
	MechCollider* GetCollider();
	// タグ取得
	FriendlyTag GetTag()const;

	// ターゲット座標セット
	void SetTargetWorldPos(const Vector3& targetWorldPos);
	// ターゲット座標取得
	const Vector3& GetTargetWorldPos()const;

	// パーツタイプを文字列に変換
	const std::string TransTypeToString(MechAnimation::TransType partsType);

	// デバッグフラグ
	DebugFlag& GetDebugFlag();
	const DebugFlag& GetDebugFlag() const;

	// マネージャの参照を取得
	DamageObjectManager* GetDamageObjectManager();
	DamageCollisionSystem* GetDamageCollisionSystem();
	GameEffectManager* GetGameEffectManager();
	MechAnimationContainer* GetAnimationContainer();

protected:
	// 初期化パラメータを受け取る
	void SetInitParam(const BaseMech::InitParam& initParam);

	// パーツを追加
	void AddParts(IMechParts* parts);
	// 武器を追加
	void RegisterWeapon(const std::string& name, std::unique_ptr<BaseMechWeapon> weapon);
	// デバッグフラグのUIを出す
	void ShowDebugFlagUI();

private:
	void CreateParts(const InitParam& param);
	void PartsSetUp();
	void BuildPartsList();
	void BuildPartsTransformArray();

	// デバッグフラグ切り替え
	void SwitchShowPartsTransform();
	void SwitchEditPartsTransform();
	void SwitchStopUpdate();
protected:
	// トランスフォーム
	Transform3D* transform_ = nullptr;
	// モデル描画トランスフォーム
	Transform3D* modelTransform_ = nullptr;
	// 有効タグ
	FriendlyTag tag_;
	// ターゲット座標
	Vector3 targetWorldPos_{};

	// パーツ
	std::unique_ptr<MechPartsHead> head_;
	std::unique_ptr<MechPartsBody> body_;
	std::unique_ptr<MechPartsArm> armL_;
	std::unique_ptr<MechPartsArm> armR_;
	std::unique_ptr<MechPartsLeg> leg_;

	// 更新対象のパーツ
	std::vector<IMechParts*> parts_;

	// 関節トランスフォームのリスト
	std::array<Transform3D*, static_cast<size_t>(MechAnimation::TransType::Count)> partsTrans_{};

	// 武器
	std::unordered_map<std::string, std::unique_ptr<BaseMechWeapon>> weapons_;

	// アニメーション
	std::unique_ptr<MechAnimator> animator_;
	// 回転制御クラス
	std::unique_ptr<MechDirController> dirController_;
	// コライダー
	std::unique_ptr<MechCollider> collider_;

	// デバッグ
	DebugFlag debugFlag_{};

	// 参照
	RefContext ref_{};
};
