#include "BaseCollider3D.h"

#include "3D/GameObject3D/GameObject3D.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

BaseCollider3D::BaseCollider3D(const std::string& colliderName, uint32_t id, Collider3DType type) {
	// 名前をセット
	name = colliderName;
	// 識別IDをセット
	id_ = id;
	// コライダーのタイプを登録
	type_ = type;
	// トランスフォームを初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
}

void BaseCollider3D::Update() {
	// ワールド行列の更新
	worldTransform_->translate_ = offset_;
	worldTransform_->Update();

	// 行列からワールド座標を計算
	worldPosition = ExtractionWorldPos(worldTransform_->worldMatrix_);

	// アクティブ状態かどうかによって色を変える処理
	if (isActive_) {
		color_ = Color::Green;
	} else {
		color_ = Color::Blue;
	}

	// 何かに触れていれば赤にする
	if (isSellingOther_) {
		color_ = Color::Red;
	}
}

uint32_t BaseCollider3D::GetID() const {
	return id_;
}

GameObject3D* BaseCollider3D::GetOwner() const {
	return owner_;
}

Collider3DType& BaseCollider3D::GetType() { 
	return type_.value();
}

CollisionCategory& BaseCollider3D::GetCategory() {
	return collisionCategory_;
}

Vector3& BaseCollider3D::GetOffset() {
	return offset_;
}

bool& BaseCollider3D::GetIsActive() {
	return isActive_;
}

bool& BaseCollider3D::GetIsSellingOther() {
	return isSellingOther_;
}

void BaseCollider3D::SetOwner(GameObject3D* owner) {
	// オーナーを登録
	owner_ = owner;
	// 親のゲームオブジェクトのトランスフォームを親子付け
	worldTransform_->parent_ = owner_->GetWorldTransform();
}

