#pragma once

#include <cstdint>

class BossMech;

/// <summary>
/// ボス機体ステータス
/// </summary>
class BossMechStatus {
public:
	struct InitParam {
		int32_t hp;
	};
public:
	BossMechStatus(const BossMechStatus::InitParam& initPram, BossMech* mech);
	~BossMechStatus() = default;

	void Update();
	int32_t GetHP() const;
private:
	void Damage(int32_t damage);
private:
	int32_t hp_ = 0;
	BossMech* mech_ = nullptr;
};