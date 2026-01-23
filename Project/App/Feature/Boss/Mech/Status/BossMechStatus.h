#pragma once

class BossMech;

/// <summary>
/// ボス機体ステータス
/// </summary>
class BossMechStatus {
public:
	BossMechStatus();
	~BossMechStatus() = default;

	void Damage(float damage);

	float GetHP() const;

private:
	float hp_ = 0.0f;

};