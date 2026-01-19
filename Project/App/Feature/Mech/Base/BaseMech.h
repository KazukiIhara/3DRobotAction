#pragma once

#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Math/Utility/MathUtility.h"

#include "3D/Transform3D/Transform3D.h"

#include "Feature/Mech/Parts/Head/MechPartsHead.h"
#include "Feature/Mech/Parts/Body/MechPartsBody.h"
#include "Feature/Mech/Parts/Arm/MechPartsArm.h"
#include "Feature/Mech/Parts/Leg/MechPartsLeg.h"

#include "Feature/Mech/Weapon/BaseMechWeapon.h"

#include "MechAnimation/Animator/MechAnimator.h"
#include "Feature/Mech/Collider/MechCollider.h"

#include "GameCommon/GameCommon.h"

// Forward
class DamageObjectManager;
class GameEffectManager;
class MechAnimationContainer;

class BaseMech {
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
		MechAnimationContainer* animationContainer = nullptr;
	};

public:
	BaseMech(const InitParam& param, const RefContext& ref);
	virtual ~BaseMech() = default;

	virtual void Update(bool isShowDebugUI, const BaseMech::InitParam& param);
	void Draw();
	void DebugDraw();

	virtual void ShowDebugWindow() {};

	void DebugUpdate(bool isShowDebugUI, const BaseMech::InitParam& param);

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
	const Vector3& GetCenterPos();

	// 武器
	BaseMechWeapon* GetWeapon(const std::string& name);

	// アニメーター
	MechAnimator* GetAnimator();

	// パーツタイプを文字列に変換
	const std::string TransTypeToString(MechAnimation::TransType partsType);

	DebugFlag& GetDebugFlag();
	const DebugFlag& GetDebugFlag() const;

	DamageObjectManager* GetDamageObjectManager();
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
	Transform3D* transform_ = nullptr;

	Transform3D* drawTransform_ = nullptr;

	FriendlyTag tag_;

	// 標準パーツ
	std::unique_ptr<MechPartsHead> head_;
	std::unique_ptr<MechPartsBody> body_;
	std::unique_ptr<MechPartsArm> armL_;
	std::unique_ptr<MechPartsArm> armR_;
	std::unique_ptr<MechPartsLeg> leg_;

	// 更新対象
	std::vector<IMechParts*> parts_;

	// 関節トランスフォーム
	std::array<Transform3D*, static_cast<size_t>(MechAnimation::TransType::Count)> partsTrans_{};

	// 武器
	std::unordered_map<std::string, std::unique_ptr<BaseMechWeapon>> weapons_;

	// アニメ
	std::unique_ptr<MechAnimator> animator_;

	// コライダー
	std::unique_ptr<MechCollider> collider_;

	// デバッグ
	DebugFlag debugFlag_{};

	// 参照
	RefContext ref_{};
};
