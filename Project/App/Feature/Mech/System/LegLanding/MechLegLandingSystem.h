#pragma once

#include "MechAnimation/MechAnimation.h"

// Forward
class BaseMech;
class Transform3D;

/// <summary>
/// 接地時に足の沈み込みを抑える簡易システム
/// </summary>
class MechLegLandingSystem {
public:
	explicit MechLegLandingSystem(BaseMech* mech);
	~MechLegLandingSystem() = default;

	void Initialize();
	void Update();

	void SetEnable(bool isEnable);
	void SwitchEnable();

	void SwitchDebugEnable();

	bool IsWorking() const;

private:
	struct LegRef {
		Transform3D* upper = nullptr;
		Transform3D* lower = nullptr;
		Transform3D* foot = nullptr;
	};

private:
	LegRef GetLegRefLeft() const;
	LegRef GetLegRefRight() const;

	void InitLeg(const LegRef& leg, float& outFootOffsetY, float& outUpperHeightY) const;
	void UpdateLeg(const LegRef& leg, float footOffsetY, float upperHeightY);

private:
	BaseMech* mech_ = nullptr;

	bool isEnable_ = true;
	bool isEnableDebug_ = true;

	bool isWorking_ = false;

	float footOffsetYL_ = 0.0f;
	float footOffsetYR_ = 0.0f;

	float upperHeightYL_ = 0.0f;
	float upperHeightYR_ = 0.0f;

	// 腰が落ちた判定の許容
	float upperDropEps_ = 0.01f;

	// 足先が基準より下がった判定の許容
	float footEps_ = 0.0001f;

	// 回転の強さ（m -> rad）
	float gain_ = 8.0f;

	// 1/60秒でのの最大回転量
	float maxStepRad_ = 2.0f;

	// Upper/Lower の配分
	float upperWeight_ = 0.4f;
	float lowerWeight_ = 0.6f;

};