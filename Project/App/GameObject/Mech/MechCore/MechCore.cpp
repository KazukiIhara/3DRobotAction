#include "MechCore.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "MechCoreStates/MechCoreBaseState.h"
#include "MechCoreStates/Idle/MechCoreStateIdle.h"
#include "MechCoreStates/Move/MechCoreStateMove.h"
#include "MechCoreStates/QuickBoost/MechCoreStateQuickBoost.h"

using namespace MAGIMath;

MechCore::MechCore(FriendlyTag tag, BulletManager* bulletManager, bool enableHardlockOn) {
	// レンダラーとゲームオブジェクトを作成
	std::shared_ptr<GameObject3D> coreObject = std::make_shared<GameObject3D>("MechCore", Vector3(0.0f, 0.0f, 0.0f));
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

	// 左手武器
	leftHandWeapon_ = std::make_unique<MechWeaponAssultRifle>();

	// 右手武器
	rightHandWeapon_ = std::make_unique<MechWeaponAssultRifle>();

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
			if (auto rightHandWeapon = rightHandWeapon_->GetGameObject().lock()) {
				rightHandWeapon->GetTransform()->SetParent(rightArm->GetTransform(), false);
			}
		}
		// 左腕
		if (auto leftArm = leftArm_->GetGameObject().lock()) {
			leftArm->GetTransform()->SetParent(body->GetTransform(), false);

			// 左手武器
			if (auto leftHandWeapon = leftHandWeapon_->GetGameObject().lock()) {
				leftHandWeapon->GetTransform()->SetParent(leftArm->GetTransform(), false);
			}
		}
		// 足
		if (auto leg = leg_->GetGameObject().lock()) {
			leg->GetTransform()->SetParent(body->GetTransform(), false);
		}
	}

	// コンポーネントを作成

	// 移動
	movementComponent_ = std::make_unique<MechMovementComponent>();
	// ロックオン
	lockOnComponent_ = std::make_unique<MechLockOnComponent>(enableHardlockOn);
	// 攻撃
	attackComponent_ = std::make_unique<MechAttackComponent>(bulletManager);
	// ステータス値管理
	statusComponent_ = std::make_unique<MechStatusComponent>();

	// ステートを作成
	states_[MechCoreState::Idle] = std::make_shared<MechCoreStateIdle>();
	states_[MechCoreState::Move] = std::make_shared<MechCoreStateMove>();
	states_[MechCoreState::QuickBoost] = std::make_shared<MechCoreStateQuickBoost>();

	// 最初のステートを設定
	ChangeState(MechCoreState::Idle);

	//===========================
	// マネージャをセット
	//===========================
	MAGIAssert::Assert(bulletManager, "BulletManager must not be null");

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

	// 各パーツを更新

	// 体
	body_->Update(this);

	// 頭
	head_->Update(this);

	// 腕
	rightArm_->Update(this);
	leftArm_->Update(this);

	// 足
	leg_->Update(this);

	// 武器を更新
	rightHandWeapon_->Update(this);
	leftHandWeapon_->Update(this);

	// 攻撃コンポーネントを更新
	attackComponent_->Update(this);

	// 移動コンポーネントを更新
	movementComponent_->Update(this);

	// 状態パラメータコンポーネントを更新
	statusComponent_->Update(this);

	// コライダーの更新
	UpdateCollider();

	// コライダーのデバッグ描画
	DrawCollider();

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

BaseMechWeapon* MechCore::GetLeftHandWeapon() {
	return leftHandWeapon_.get();
}

BaseMechWeapon* MechCore::GetRightHandWeapon() {
	return rightHandWeapon_.get();
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
