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
		std::string head;
		std::string body;
		std::string upperArm;
		std::string lowerArm;
		std::string hand;
		std::string upperLeg;
		std::string lowerLeg;
		std::string foot;
	};
public:
	BossMech(const BossMech::InitParam& initParam);
	~BossMech() = default;

	void Update();

	void Draw();

private:

};