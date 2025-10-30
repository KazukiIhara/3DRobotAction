#include "MechCore.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "MechCoreStates/MechCoreBaseState.h"
#include "MechCoreStates/Idle/MechCoreStateIdle.h"
#include "MechCoreStates/Move/MechCoreStateMove.h"
#include "MechCoreStates/QuickBoost/MechCoreStateQuickBoost.h"
#include "MechCoreStates/AssultBoost/MechCoreStateAssultBoost.h"

using namespace MAGIMath;
using namespace MAGIUtility;

MechCore::MechCore(const Vector3& position, FriendlyTag tag, AttackObjectManager* attackObjectManager, bool enableHardlockOn) {

	// 
	// 今後の実装ココから
	// 

	// 機体の名前を引数で取得


	// 機体のデータ(パーツ名と武器名)保存しているコンテナから各パーツ名と武器名を取得


	// パーツと武器を保存しているコンテナから各パラメータを取得


	// 以下機体作成時にパラメータを受け取って生成するように実装


	// 
	// 今後の実装ここまで
	// 

	// ゲームオブジェクトを作成
	std::shared_ptr<GameObject3D> coreObject = std::make_shared<GameObject3D>("MechCore", position);
	coreObject->SetIsUnique(true);
	// ゲームオブジェクトマネージャに追加
	core_ = MAGISYSTEM::AddGameObject3D(std::move(coreObject));

	// タグをセット
	tag_ = tag;

	// パーツを作成

	// 頭
	head_ = std::make_unique<MechHead>();

	// 体
	body_ = std::make_unique<MechBody>();

	// 腕
	rightArm_ = std::make_unique<MechArmRight>();
	leftArm_ = std::make_unique<MechArmLeft>();

	// 足
	leg_ = std::make_unique<MechLeg>();


	// 一旦値をそのまま入力
	MechHandWeapon::Param param;
	param.speed = 70.0f;
	param.fireOffsetLocalPos = { 0.0f,0.24f,1.3f };
	param.coolTime = 0.2f;
	param.damage = 200;

	// 右手武器
	rightHandWeapon_ = std::make_unique<MechHandWeapon>(param, attackObjectManager);

	// 左手武器をロケランにする
	param.name = "RocketLauncher";
	param.modelName = "RocketLauncher";
	param.type = MechHandWeapon::Type::RocketLauncher;

	// 左手武器
	leftHandWeapon_ = std::make_unique<MechHandWeapon>(param, attackObjectManager);


	// 左肩武器
	leftShoulerWeapon_ = std::make_unique<MechShoulderWeaponDualMissileLauncher>(WitchShoulder::Left);

	// 右肩武器
	rightShoulerWeapon_ = std::make_unique<MechShoulderWeaponDualMissileLauncher>(WitchShoulder::Right);


	// パーツを親子付け

	// 体
	if (auto body = body_->GetGameObject().lock()) {
		body->GetTransform()->SetParent(core_.lock()->GetTransform(), false);
		// コライダーを作成
		collider_ = std::make_unique<MechCollider>(tag_, body->GetTransform()->GetWorldPosition(), kColliderMin_, kColliderMax_);

		// 頭
		if (auto head = head_->GetGameObject().lock()) {
			head->GetTransform()->SetParent(body->GetTransform(), false);
		}
		// 右腕
		if (auto rightArm = rightArm_->GetGameObject().lock()) {
			rightArm->GetTransform()->SetParent(body->GetTransform(), false);

			// 右手武器
			rightHandWeapon_->GetTransform()->SetParent(rightArm_->GetHandTransform(), false);
		}
		// 左腕
		if (auto leftArm = leftArm_->GetGameObject().lock()) {
			leftArm->GetTransform()->SetParent(body->GetTransform(), false);

			// 左手武器
			leftHandWeapon_->GetTransform()->SetParent(leftArm_->GetHandTransform(), false);

		}
		// 足
		if (auto leg = leg_->GetGameObject().lock()) {
			leg->GetTransform()->SetParent(body->GetTransform(), false);
		}

		// 右肩武器
		if (auto rightShoulderWeapon = rightShoulerWeapon_->GetGameObject().lock()) {
			rightShoulderWeapon->GetTransform()->SetParent(body->GetTransform(), false);
		}

		// 左肩武器
		if (auto leftShoulderWeapon = leftShoulerWeapon_->GetGameObject().lock()) {
			leftShoulderWeapon->GetTransform()->SetParent(body->GetTransform(), false);
		}
	}

	// コンポーネントを作成

	// 移動
	movementComponent_ = std::make_unique<MechMovementComponent>();
	// ロックオン
	lockOnComponent_ = std::make_unique<MechLockOnComponent>(enableHardlockOn);
	// 攻撃
	attackComponent_ = std::make_unique<MechAttackComponent>(attackObjectManager);
	// ステータス値管理
	statusComponent_ = std::make_unique<MechStatusComponent>();

	// ステートを作成
	states_[MechCoreState::Idle] = std::make_shared<MechCoreStateIdle>();
	states_[MechCoreState::Move] = std::make_shared<MechCoreStateMove>();
	states_[MechCoreState::QuickBoost] = std::make_shared<MechCoreStateQuickBoost>();
	states_[MechCoreState::AssultBoost] = std::make_shared<MechCoreStateAssultBoost>();

	// 最初のステートを設定
	ChangeState(MechCoreState::Idle);

	//===========================
	// エフェクト初期化
	//===========================

	// クイックブースト
	quickBoostparticle_ = std::make_unique<QuickBoostParticle>(this);

	// 弾衝突時エフェクト
	bulletHitEffect_ = std::make_unique<BulletHitEffect>(this);

}

void MechCore::Update() {
	// 状態を整理 (コマンドはMechCoreの更新前に外部からセットする)
	// ロックオン用のビュー情報も外部からセットしておく

	// ロックオンコンポーネントを更新
	lockOnComponent_->Update(this);

	// 接地状態かどうかチェック
	movementComponent_->CheckOnGround(this);

	// ステートごとの更新
	if (auto cs = currentState_.second.lock()) {
		cs->Update(this);
	}

	//=================== 
	// コンポーネント
	//=================== 

	// 攻撃コンポーネントを更新
	attackComponent_->Update(this);

	// 移動コンポーネントを更新
	movementComponent_->Update(this);

	// 状態パラメータコンポーネントを更新
	statusComponent_->Update(this);


	//=================== 
	// パーツ
	//=================== 

	// 体
	body_->Update(this);
	// 頭
	head_->Update(this);
	// 腕
	rightArm_->Update(this);
	leftArm_->Update(this);
	// 足
	leg_->Update(this);

	//=================== 
	// 武器
	//=================== 

	rightHandWeapon_->Update();
	leftHandWeapon_->Update();
	leftShoulerWeapon_->Update(this);
	rightShoulerWeapon_->Update(this);


	//=================== 
	// エフェクト
	//=================== 

	// 弾衝突時エフェクト
	bulletHitEffect_->Update();

	// コライダーの更新
	UpdateCollider();

	// 
	// プレイヤー機体のみの処理
	// 
	if (tag_ == FriendlyTag::PlayerSide) {
		PlayerMechEffect();
	}

}

void MechCore::ChangeState(MechCoreState nextState) {
	// 旧ステートの終了処理
	if (auto cs = currentState_.second.lock()) {
		cs->Exit(this);
	}

	// 変更後ステートの開始処理
	currentState_ = std::make_pair(nextState, GetState(nextState));
	if (auto cs = currentState_.second.lock()) {
		cs->Enter(this);
	}
}

void MechCore::Draw() {
	// 弾衝突時エフェクト
	bulletHitEffect_->Draw();

	// 左手武器描画
	leftHandWeapon_->Draw();

	// 右手武器描画
	rightHandWeapon_->Draw();

	// コライダーのデバッグ描画
	DrawCollider();
}

std::weak_ptr<GameObject3D> MechCore::GetGameObject() const {
	return core_;
}

const MechCoreState& MechCore::GetCurrentState() const {
	return currentState_.first;
}

const InputCommand& MechCore::GetInputCommand() const {
	return inputCommand_;
}

const LockOnView& MechCore::GetLockOnView() const {
	return lockOnView_;
}

const FriendlyTag& MechCore::GetFriendlyTag() const {
	return tag_;
}

MechBody* MechCore::GetMechBody() {
	return body_.get();
}

MechArmLeft* MechCore::GetMechArmLeft() {
	return leftArm_.get();
}

MechArmRight* MechCore::GetMechArmRight() {
	return rightArm_.get();
}

MechHandWeapon* MechCore::GetLeftHandWeapon() {
	return leftHandWeapon_.get();
}

MechHandWeapon* MechCore::GetRightHandWeapon() {
	return rightHandWeapon_.get();
}

BaseMechShoulderWeapon* MechCore::GetLeftShoulderWeapon() {
	return leftShoulerWeapon_.get();
}

BaseMechShoulderWeapon* MechCore::GetRightShoulderWeapon() {
	return rightShoulerWeapon_.get();
}

MechMovementComponent* MechCore::GetMovementComponent() {
	return movementComponent_.get();
}

MechLockOnComponent* MechCore::GetLockOnComponent() {
	return lockOnComponent_.get();
}

MechAttackComponent* MechCore::GetAttackComponent() {
	return attackComponent_.get();
}

MechStatusComponent* MechCore::GetStatusComponent() {
	return statusComponent_.get();
}

QuickBoostParticle* MechCore::GetQuickBoostParticle() {
	return quickBoostparticle_.get();
}

BulletHitEffect* MechCore::GetBulletHitEffect() {
	return bulletHitEffect_.get();
}

MechCollider* MechCore::GetCollider() {
	return collider_.get();
}

void MechCore::SetInputCommand(const InputCommand& command) {
	inputCommand_ = command;
}

void MechCore::SetLockOnView(const LockOnView& lockOnView) {
	lockOnView_ = lockOnView;
}

std::weak_ptr<MechCoreBaseState> MechCore::GetState(MechCoreState state) {
	auto it = states_.find(state);
	if (it != states_.end()) {
		return it->second;
	}

	MAGIAssert::Assert(false, "Not find MechCoreState!");
	return {};
}

void MechCore::UpdateCollider() {
	// ひとまずBodyに追従させる
	if (auto bodyObj = body_->GetGameObject().lock()) {
		// 体のワールド座標を取得
		const Vector3 bodyWorldPos = bodyObj->GetTransform()->GetWorldPosition();
		collider_->SetWorldPos(bodyWorldPos);
	}
	// コライダー更新
	collider_->Update();
}

void MechCore::DrawCollider() {
	collider_->Draw();
}

void MechCore::PlayerMechEffect() {
	QuickBoostRadialBlur();
}

void MechCore::QuickBoostRadialBlur() {
	// クイックブースト中でなければ早期リターン
	if (currentState_.first != MechCoreState::QuickBoost) return;

	// クイックブーストタイマー取得
	const float time = movementComponent_->GetQuickBoostTimer();
	const float kMaxTime = movementComponent_->GetQuickBoostMaxTime();

	// ブラーの最大値
	const float kMaxBlurWidth = 0.01f;

	// ブラーの値を補完計算
	const float blurWitdh = (1.0f - time / kMaxTime) * kMaxBlurWidth;

	// ブラーの座標計算
	Vector3 blurWorldPos;
	Vector2 blurScreenPos;
	if (auto enemy = lockOnComponent_->GetLockOnTarget().lock()) {
		// 敵の胴体のワールド座標
		blurWorldPos = enemy->GetMechBody()->GetGameObject().lock()->GetTransform()->GetWorldPosition();
		// 機体胴体のスクリーン座標を取得
		blurScreenPos = TransformWorldToScreen(blurWorldPos);
	} else {
		blurScreenPos = { 0.5f,0.5f };
	}

	// ウィンドウサイズから0.0f~1.0fの値にクランプ
	const Vector2 bodyScreenPosClamped = { std::clamp(blurScreenPos.x / WindowApp::kClientWidth,0.0f,1.0f), std::clamp(blurScreenPos.y / WindowApp::kClientHeight,0.0f,1.0f) };

	// 機体のスクリーン0.0f~1.0f座標に補完計算したブラーの値で
	MAGISYSTEM::ApplyPostEffectRadialBlur(bodyScreenPosClamped, blurWitdh);
}
