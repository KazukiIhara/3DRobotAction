#pragma once

// C++
#include <string>
#include <memory>

// 機体パーツ
#include "../Mech/Parts/Head/BossMechHead.h"
#include "../Mech/Parts/Body/BossMechBody.h"
#include "../Mech/Parts/Arm/Right/BossMechRightArm.h"
#include "../Mech/Parts/Arm/Left/BossMechLeftArm.h"
#include "../Mech/Parts/Leg/Right/BossMechRightLeg.h"
#include "../Mech/Parts/Leg/Left/BossMechLeftLeg.h"

/// <summary>
/// ボス機体クラス
/// </summary>
class BossMech {
public:
	struct InitParam {
		// 各パーツのデータ
		BossMechHead::InitParam head;
		BossMechBody::InitParam body;
		BossMechBaseArm::InitParam arm;
		BossMechBaseLeg::InitParam leg;
	};
public:
	BossMech(const BossMech::InitParam& initParam);
	~BossMech() = default;

	void Update();
	void Draw();

private:
	// 各パーツ
	std::unique_ptr<BossMechHead> head_;
	std::unique_ptr<BossMechBody> body_;
	std::unique_ptr<BossMechRightArm> rightArm_;
	std::unique_ptr<BossMechLeftArm> leftArm_;
	std::unique_ptr<BossMechRightLeg> rightLeg_;
	std::unique_ptr<BossMechLeftLeg> leftLeg_;

};