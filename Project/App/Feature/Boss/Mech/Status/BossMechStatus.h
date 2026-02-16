#pragma once

// C++
#include <cstdint>

// ボス機体
class BossMech;

/// <summary>
/// ボス機体ステータス
/// </summary>
class BossMechStatus {
public:
	struct Param {
		int32_t hp;
	};
public:
	BossMechStatus(BossMech* mech);
	~BossMechStatus() = default;

	void Update();
	int32_t GetHP() const;
	int32_t GetMaxHP()const;

private:
	void Damage(int32_t damage);
private:
	int32_t maxHp_;
	Param param_;
	BossMech* mech_ = nullptr;
};