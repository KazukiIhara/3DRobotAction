#pragma once

// C++
#include <memory>

// 前方宣言
class AttackCollider;

/// <summary>
/// 攻撃オブジェクトの共通基底
/// </summary>
class BaseAttackObject {
public:
	BaseAttackObject();
	virtual ~BaseAttackObject();

	bool GetIsAlive() const;
	AttackCollider* GetAttackCollider();

protected:
	void BeginLife(float lifeTime, std::weak_ptr<AttackCollider> collider);

	bool CheckHitAndFinalize();
	bool TickLifeAndFinalize(float dt);

	std::shared_ptr<AttackCollider> LockCollider() const;

	void Finalize();

private:
	virtual void OnFinalize() = 0;

protected:
	bool isAlive_ = false;

private:
	float lifeTime_ = 0.0f;
	std::weak_ptr<AttackCollider> collider_;
};
