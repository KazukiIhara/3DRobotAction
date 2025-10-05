#include "EnemyAI.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Mech/MechCore/MechCore.h"

#include "EnemyAIState/BaseEnemyAIState.h"

#include "GameObject/AttackObjectManager/AttackObjectManager.h"

// 
// 敵AIのステート
// 
#include "EnemyAIState/Root/EnemyAIStateRoot.h"
#include "EnemyAIState/Search/EnemyAIStateSearch.h"
#include "EnemyAIState/Avoid/EnemyAIStateAvoid.h"

EnemyAI::EnemyAI(std::weak_ptr<MechCore> mechCore, std::weak_ptr<MechCore> playerMech, AttackObjectManager* bulletManager) {
	// 自機のポインタを受け取る
	if (auto m = mechCore.lock()) {
		mechCore_ = m.get();
	}

	// プレイヤーの機体のポインタを受け取る
	playerMech_ = playerMech;

	// コマンド初期化
	command_ = InputCommand{};

	// ステートを作成
	states_[EnemyAIState::Root] = std::make_shared<EnemyAIStateRoot>();
	states_[EnemyAIState::Search] = std::make_shared<EnemyAIStateSearch>();
	states_[EnemyAIState::Avoid] = std::make_shared<EnemyAIStateAvoid>();

	// 最初のステートを設定
	ChangeState(EnemyAIState::Search);

	attackObjectManager_ = bulletManager;
}

InputCommand EnemyAI::Update() {
	// コマンドリセット
	command_ = InputCommand{};

	// 回避用コライダーの更新
	const Matrix4x4 lMat = MakeTranslateMatrix(avoidColliderTranslate_);
	const Matrix4x4 wMat = lMat * mechCore_->GetMechBody()->GetGameObject().lock()->GetTransform()->GetWorldMatrix();
	const Vector3 wPos = ExtractionWorldPos(wMat);

	const float size = avoidCollider_.localMinMax;
	const float sizeY = avoidCollider_.localMinMaxY;
	avoidCollider_.min = { wPos.x - size, wPos.y - sizeY,wPos.z - size };
	avoidCollider_.max = { wPos.x + size, wPos.y + sizeY,wPos.z + size };

	// コライダーをデバッグ描画
#if defined(DEBUG) || defined(DEVELOP)
	MAGISYSTEM::DrawLineAABB(avoidCollider_.min, avoidCollider_.max, Color::Green);
#endif

	// ステートごとの更新
	if (auto cs = currentState_.second.lock()) {
		cs->Update(this, mechCore_);
	}

	// 入力された移動方向をカメラに対しての向きに直す
	CulDirectionWithCamera(mechCore_);

	// コマンドを返す
	return command_;
}

void EnemyAI::ChangeState(EnemyAIState nextState) {
	// 旧ステートの終了処理
	if (auto cs = currentState_.second.lock()) {
		cs->Exit(this, mechCore_);
	}

	// 変更後ステートの開始処理
	currentState_ = std::make_pair(nextState, GetState(nextState));
	if (auto cs = currentState_.second.lock()) {
		cs->Enter(this, mechCore_);
	}
}

MechCore* EnemyAI::GetPlayerMech() {
	auto ptr = playerMech_.lock();
	return ptr.get();
}

RootDir EnemyAI::GetRootDir() const {
	return rootDir_;
}

AvoidColliderAABB EnemyAI::GetAvoidCollider() const {
	return avoidCollider_;
}

void EnemyAI::MoveDir(const Vector2& dir) {
	command_.moveDirection = dir;
}

void EnemyAI::CameraRotDir(const Vector2& camRDir) {
	command_.cameraRotDirection = camRDir;
}

void EnemyAI::Jump() {
	command_.jump = true;
}

void EnemyAI::QuickBoost() {
	command_.quickBoost = true;
}

void EnemyAI::AssultBoost() {
	command_.assultBoost = true;
}

void EnemyAI::LeftHandWeapon() {
	command_.leftHandWeapon = true;
}

void EnemyAI::RightHandWeapon() {
	command_.rightHandWeapon = true;
}

void EnemyAI::SetRootDir(RootDir dir) {
	rootDir_ = dir;
}

AttackObjectManager* EnemyAI::GetBulletManager() {
	return attackObjectManager_;
}

std::weak_ptr<BaseEnemyAIState> EnemyAI::GetState(EnemyAIState state) {
	auto it = states_.find(state);
	if (it != states_.end()) {
		return it->second;
	}

	MAGIAssert::Assert(false, "Not find EnemyAIState!");
	return {};
}

void EnemyAI::CulDirectionWithCamera(MechCore* mechCore) {

	Vector2 inputMoveDir{};
	Vector3 forward{};
	Vector3 right{};
	Vector2 moveDir{};

	// 入力された移動方向をカメラに対しての向きに直す
	inputMoveDir = command_.moveDirection;
	// 機体のゲームオブジェクトを取得
	if (auto mechObj = mechCore->GetGameObject().lock()) {
		// 機体のカメラを取得
		if (auto mainCam = mechObj->GetCamera3D("MainCamera").lock()) {

			forward = mainCam->GetTarget() - mainCam->GetEye();
			forward.y = 0.0f;
			forward = Normalize(forward);
			right = Normalize(Cross({ 0.0f,1.0f,0.0f }, forward));

			const Vector3 tempDir = Normalize(right * inputMoveDir.x + forward * inputMoveDir.y);

			moveDir = { tempDir.x, tempDir.z };

		}

	}

	// 移動方向をセット
	command_.moveDirection = Normalize(moveDir);
}
