#include "GameObject3D.h"
#include "MAGIUitility/MAGIUtility.h"
#include <cassert>

using namespace MAGIUtility;

GameObject3D::GameObject3D(const std::string& objectName, const EulerTransform3D& transform)
	:WorldEntity() {
	// 名前をセット
	name = objectName;
	// ワールドトランスフォーム作成
	CreateWorldTransform(transform);
}

void GameObject3D::Initialize() {
	// オブジェクトごとの初期化処理

}

void GameObject3D::Update() {
	// オブジェクトごとの更新処理

}

void GameObject3D::Draw() {

}

void GameObject3D::UpdateWorldTransformHierarchy() {
	// まず自分のトランスフォームを更新
	if (worldTransform_) {
		worldTransform_->Update();
	}
	// 子を再帰的に更新
	for (auto it = children_.begin(); it != children_.end(); ) {
		GameObject3D* child = *it;
		child->UpdateWorldTransformHierarchy();
		if (child->parent_ != this) {
			it = children_.erase(it);
		} else {
			++it;
		}
	}
}

void GameObject3D::UpdateHierarchy() {
	// 自分自身の処理
	Update();

	// 子リストをイテレート
	for (auto it = children_.begin(); it != children_.end(); ) {
		auto child = *it;
		// 再帰的に UpdateHierarchy
		child->UpdateHierarchy();

		// 子が「DetachParent()」した場合、parent_ == nullptr になる
		if (child->GetParent() != this) {
			// リストから削除
			it = children_.erase(it);
		} else {
			++it;
		}
	}
}

void GameObject3D::DeleteComponents() {
	// 付随する全レンダラーの削除フラグを立てる
	for (auto& renderer3D : renderers3D_) {
		if (renderer3D.second) {
			renderer3D.second->isAlive = false;
			renderer3D.second = nullptr;
		}
	}

	// 付随する全コライダーの削除フラグを立てる
	for (auto& collider3D : colliders3D_) {
		if (collider3D.second) {
			collider3D.second->isAlive = false;
			collider3D.second = nullptr;
		}
	}
}

void GameObject3D::SetParent(GameObject3D* parent) {
	if (parent_ == parent) {
		return; // すでに同じ親なら何もしない
	}
	// もし現在の親がいるなら、いったんデタッチ
	if (parent_) {
		DetachParent();
	}

	// Nullチェック
	assert(parent != nullptr && "SetParent to nullptr is not allowed.");

	parent_ = parent;
	// 親の子リストに登録
	parent_->children_.push_back(this);

	// 親のワールドトランスフォームを参照する場合
	if (worldTransform_) {
		worldTransform_->parent_ = parent->GetWorldTransform();
	}
}

void GameObject3D::AddChild(GameObject3D* child) {
	// Nullチェック
	assert(child != nullptr && "AddChild is nullptr.");
	// 子に自身をセット
	child->SetParent(this);
}

void GameObject3D::DetachParent() {
	if (parent_) {
		parent_ = nullptr;
		if (worldTransform_) {
			worldTransform_->parent_ = nullptr;
		}
	}
}

void GameObject3D::DetachChild(GameObject3D* child) {
	if (!child) return;
	// もし引数の子の親が自分でなければ何もしない
	if (child->parent_ != this) return;

	// 子のほうの parent_ を nullptr にする(一方的に切る)
	child->parent_ = nullptr;
	if (child->worldTransform_) {
		child->worldTransform_->parent_ = nullptr;
	}
	// 自分の子リストからも削除(安全な方法で remove)
	children_.remove(child);
}

GameObject3D* GameObject3D::GetParent() {
	return parent_;
}

std::list<GameObject3D*>* GameObject3D::GetChildren() {
	return &children_;
}

GameObject3D* GameObject3D::GetChild(const std::string& childName) {
	for (auto& child : children_) {
		if (child->name == childName) {
			return child;
		}
	}
	return nullptr; // 見つからなかった場合
}

Vector3& GameObject3D::GetScale() {
	return worldTransform_->scale_;
}

Vector3& GameObject3D::GetRotate() {
	return worldTransform_->rotate_;
}

Vector3& GameObject3D::GetTranslate() {
	return worldTransform_->translate_;
}

WorldTransform* GameObject3D::GetWorldTransform() {
	return worldTransform_.get();
}

void GameObject3D::OnCollisionEnter([[maybe_unused]] BaseCollider3D* other) {

}

void GameObject3D::OnCollisionStay([[maybe_unused]] BaseCollider3D* other) {

}

void GameObject3D::OnCollisionExit([[maybe_unused]] BaseCollider3D* other) {

}

BaseRenderable3D* GameObject3D::GetRenderer3D(const std::string& object3DName) {
	// 描画オブジェクトを検索
	if (!renderers3D_.contains(object3DName)) {
		// エラーを出して止める
		assert(false && "Not Found Renderer3D");
	}
	// 見つかった描画オブジェクトを返す
	return renderers3D_.at(object3DName);
}

BaseCollider3D* GameObject3D::GetCollider3D(const std::string& object3DName) {
	// 作成済みコライダーを検索
	if (!colliders3D_.contains(object3DName)) {
		// エラーメッセージ
		assert(false && "Not Found Collider");
	}
	// コライダーのオフセットの参照を返す
	return colliders3D_.at(object3DName);
}

SphereCollider* GameObject3D::GetSphereCollider3D(const std::string& collider3DName) {
	// コライダーを検索
	auto it = colliders3D_.find(collider3DName);
	if (it == colliders3D_.end()) {
		// エラーログ出力
		assert(false && "Not Found Collider");
		return nullptr;
	}

	// 型チェック（SphereCollider かどうか）
	if (auto sphereCollider3D = dynamic_cast<SphereCollider*>(it->second)) {
		return sphereCollider3D;
	} else {
		assert(false && "This Collider Is Not SphereCollider");
		return nullptr;
	}
}

AABBCollider* GameObject3D::GetAABBCollider3D(const std::string& collider3DName) {
	// コライダーを検索
	auto it = colliders3D_.find(collider3DName);
	if (it == colliders3D_.end()) {
		// エラーログ出力
		assert(false && "Not Found Collider");
		return nullptr;
	}

	// 見つかったコライダーがAABBコライダーかどうかを判定
	if (auto aabbCollider3D = dynamic_cast<AABBCollider*>(colliders3D_.at(collider3DName))) {
		return aabbCollider3D;
	} else {
		assert(false && "This Collider Is Not AABBCollider");
		return nullptr;
	}
}

std::map<std::string, BaseRenderable3D*> GameObject3D::GetRenderers3D() {
	return renderers3D_;
}

std::map<std::string, BaseCollider3D*> GameObject3D::GetColliders() {
	return colliders3D_;
}

void GameObject3D::AddRenderer3D(BaseRenderable3D* renderer3D) {
	// nullチェック
	assert(renderer3D && "Add Renderer3D is nullptr");
	// ワールドトランスフォームの親子付け
	renderer3D->GetWorldTransform()->parent_ = worldTransform_.get();
	// 描画フラグオン
	renderer3D->GetIsShow() = true;
	// 3D描画オブジェクトマップにセット
	renderers3D_.insert(std::pair(renderer3D->name, renderer3D));
}

void GameObject3D::AddCollider(BaseCollider3D* collider3D) {
	// nullチェック
	assert(collider3D && "Add Collider3D is nullptr");
	// オーナーをセット
	collider3D->SetOwner(this);
	// コライダーマップにセット
	colliders3D_.insert(std::pair(collider3D->name, collider3D));
}

void GameObject3D::CreateWorldTransform(const EulerTransform3D& transform) {
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->scale_ = transform.scale;
	worldTransform_->rotate_ = transform.rotate;
	worldTransform_->translate_ = transform.translate;
}