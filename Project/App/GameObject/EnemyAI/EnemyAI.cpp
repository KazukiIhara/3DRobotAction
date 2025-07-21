#include "EnemyAI.h"

#include "MAGI.h"

#include "GameObject/Mech/MechCore/MechCore.h"

EnemyAI::EnemyAI(std::weak_ptr<MechCore> playerMech) {
	// プレイヤーの機体のポインタを受け取る
	playerMech_ = playerMech;


}

InputCommand EnemyAI::Update(MechCore* mechCore) {

	// 出力するコマンド
	InputCommand command{};

	// 
	// ずっと攻撃
	// 


	// 
	// ひとまずプレイヤーの周りを旋回させてみる
	// 
	Vector2 move{};
	Vector3 forward{};
	Vector3 right{};
	Vector2 moveDir{};

	move.x = 1.0f;
	move.y = 1.0f;

	// 機体のゲームオブジェクトを取得
	if (auto mechObj = mechCore->GetGameObject().lock()) {
		// 機体のカメラを取得
		if (auto mainCam = mechObj->GetCamera3D("MainCamera").lock()) {

			forward = mainCam->GetTarget() - mainCam->GetEye();
			forward.y = 0.0f;
			forward = Normalize(forward);
			right = Normalize(Cross({ 0.0f,1.0f,0.0f }, forward));

			const Vector3 tempDir = Normalize(right * move.x + forward * move.y);

			moveDir = { tempDir.x, tempDir.z };

		}

	}

	// 移動方向をセット
	command.moveDirection = Normalize(moveDir);


	// コマンドを返す
	return command;
}