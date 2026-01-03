#include "BaseAttackObject.h"

#include "GameObject/AttackCollider/AttackCollider.h"

BaseAttackObject::BaseAttackObject() {
}

BaseAttackObject::~BaseAttackObject() {
}

bool BaseAttackObject::GetIsAlive() const {
	return isAlive_;
}

AttackCollider* BaseAttackObject::GetAttackCollider() {
	return collider_.lock().get();
}

void BaseAttackObject::BeginLife(float lifeTime, std::weak_ptr<AttackCollider> collider) {
	isAlive_ = true;
	lifeTime_ = lifeTime;
	collider_ = collider;
}

std::shared_ptr<AttackCollider> BaseAttackObject::LockCollider() const {
	return collider_.lock();
}

bool BaseAttackObject::CheckHitAndFinalize() {
	if (!isAlive_) {
		return true;
	}

	if (auto collider = collider_.lock()) {
		if (collider->GetHitInfo().isHit_) {
			Finalize();
			return true;
		}
	}

	return false;
}

bool BaseAttackObject::TickLifeAndFinalize(float dt) {
	if (!isAlive_) {
		return true;
	}

	lifeTime_ -= dt;
	if (lifeTime_ <= 0.0f) {
		Finalize();
		return true;
	}

	return false;
}

void BaseAttackObject::Finalize() {
	if (!isAlive_) {
		return;
	}

	isAlive_ = false;

	if (auto collider = collider_.lock()) {
		collider->SetIsAlive(false);
	}

	OnFinalize();
}
