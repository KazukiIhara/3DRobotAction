#include "Transform3D.h"

Transform3D::Transform3D(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Initialize(scale, rotate, translate);
}

Transform3D::Transform3D(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	Initialize(scale, rotate, translate);
}

Transform3D::Transform3D(const Vector3& translate) {
	Initialize(translate);
}

void Transform3D::Initialize(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	scale_ = scale;
	inputRadians_ = rotate;
	rotate_ = MAGIMath::EulerToQuaternionYXZ(inputRadians_);
	translate_ = translate;

	// ワールド行列作成
	worldMatrix_ = MAGIMath::MakeAffineMatrix(scale_, rotate_, translate_);
	// ワールド座標生成
	worldPosition_ = MAGIMath::ExtractionWorldPos(worldMatrix_);
}

void Transform3D::Initialize(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	scale_ = scale;
	rotate_ = rotate;
	inputRadians_ = MAGIMath::QuaternionToEuler(rotate_);
	translate_ = translate;

	// ワールド行列作成
	worldMatrix_ = MAGIMath::MakeAffineMatrix(scale_, rotate_, translate_);
	// ワールド座標生成
	worldPosition_ = MAGIMath::ExtractionWorldPos(worldMatrix_);
}

void Transform3D::Initialize(const Vector3& translate) {
	scale_ = { 1.0f,1.0f,1.0f };
	inputRadians_ = { 0.0f,0.0f,0.0f };
	rotate_ = MAGIMath::EulerToQuaternionYXZ(inputRadians_);
	translate_ = translate;

	// ワールド行列作成
	worldMatrix_ = MAGIMath::MakeAffineMatrix(scale_, rotate_, translate_);
	// ワールド座標生成
	worldPosition_ = MAGIMath::ExtractionWorldPos(worldMatrix_);
}

void Transform3D::Update() {
	if (isChanged_) {
		// 直接Q回転に変更があった場合はこっち優先
		if (preRotate_ != rotate_) {
			inputRadians_ = MAGIMath::QuaternionToEuler(rotate_);
		} else if (preInputRadians_ != inputRadians_) { 	// ユーザー入力用の回転に変更があった場合
			// 変換してクオータニオンの回転に適用
			rotate_ = MAGIMath::EulerToQuaternionYXZ(inputRadians_);
		}

		// ワールド行列作成
		worldMatrix_ = MAGIMath::MakeAffineMatrix(scale_, rotate_, translate_);

		// 現在フレームの値を保存
		preRotate_ = rotate_;
		preInputRadians_ = inputRadians_;

		// 次フレーム用のフラグを立てる
		isChanged_ = false;

		// 親がいる場合
		if (parent_) {
			worldMatrix_ = worldMatrix_ * parent_->GetWorldMatrix();
		}

		// ワールド座標生成
		worldPosition_ = MAGIMath::ExtractionWorldPos(worldMatrix_);

		// もし子がいる場合
		if (!children_.empty()) {
			for (auto& child : children_) {
				if (child) {
					child->SetIsChange(true);
					child->Update();
				}
			}
		}

	} else {
		// もし子がいる場合
		if (!children_.empty()) {
			for (auto& child : children_) {
				if (child) {
					child->Update();
				}
			}
		}
	}
}

void Transform3D::Finalize() {
	// 親を切り離す
	RemoveParent();

	// もし子がいる場合
	if (!children_.empty()) {
		const auto childrenCopy = children_;
		for (auto* child : childrenCopy) {
			if (child) {
				child->RemoveParent(true);
				child->SetIsChange(true);
			}
		}
		children_.clear();
	}

}

Transform3D* Transform3D::GetParent() const {
	return parent_;
}

const bool& Transform3D::GetIsChanged() const {
	return isChanged_;
}

const bool& Transform3D::GetisAlive() const {
	return isAlive_;
}

const Vector3& Transform3D::GetScale()const {
	return scale_;
}

const Vector3& Transform3D::GetRotate() const {
	return inputRadians_;
}

const Vector3& Transform3D::GetTranslate()const {
	return translate_;
}

const Quaternion& Transform3D::GetQuaternion()const {
	return rotate_;
}

const Matrix4x4& Transform3D::GetWorldMatrix() const {
	return worldMatrix_;
}

const Vector3& Transform3D::GetWorldPosition() const {
	return worldPosition_;
}

void Transform3D::SetParent(Transform3D* parent, bool keepWorld) {
	// 同じだった場合は早期リターン
	if (parent_ == parent) {
		return;
	}

	// 現在のワールド行列を保持
	Matrix4x4 currentWorld = worldMatrix_;

	// すでに親がいた場合は旧親から自身を削除する
	if (parent_) {
		auto& oldChildren = parent_->children_;
		oldChildren.erase(
			std::remove(oldChildren.begin(), oldChildren.end(), this),
			oldChildren.end()
		);
	}

	// 親をセット
	parent_ = parent;

	// 親に自身を子としてセット
	if (parent_) {
		parent_->children_.push_back(this);
	}

	// ワールド座標をキープする場合
	if (keepWorld && parent_) {
		// ワールド行列を親の逆行列でローカルへ変換
		Matrix4x4 parentInvWorld = MAGIMath::Inverse(parent_->GetWorldMatrix());
		Matrix4x4 localMatrix = currentWorld * parentInvWorld;

		// 分解してローカルの値に設定
		MAGIMath::DecomposeAffineMatrix(localMatrix, scale_, rotate_, translate_);
		inputRadians_ = MAGIMath::QuaternionToEuler(rotate_);
	}

	isChanged_ = true;

	if (isChanged_) {
		// もし子がいる場合
		if (!children_.empty()) {
			for (auto& child : children_) {
				if (child) {
					child->SetIsChange(true);
				}
			}
		}
	}
}

void Transform3D::RemoveParent(bool keepWorld) {
	// すでに親が無ければ何もしない
	if (!parent_) {
		return;
	}

	// 現在のワールド行列を保持
	Matrix4x4 currentWorld = worldMatrix_;

	// 親の children_ から自分を削除
	{
		auto& siblings = parent_->children_;
		siblings.erase(
			std::remove(siblings.begin(), siblings.end(), this),
			siblings.end()
		);
	}

	// 親ポインタを解除
	parent_ = nullptr;

	// ワールドを維持したい場合は
	if (keepWorld) {
		MAGIMath::DecomposeAffineMatrix(currentWorld, scale_, rotate_, translate_);
		inputRadians_ = MAGIMath::QuaternionToEuler(rotate_);
	}

	// 変化フラグを立て、子へ伝搬
	isChanged_ = true;

	if (!children_.empty()) {
		for (auto& child : children_) {
			if (child) {
				child->SetIsChange(true);
			}
		}
	}
}

void Transform3D::SetIsChange(bool isChange) {
	isChanged_ = isChange;
}

void Transform3D::SetIsAlive(bool isAlive) {
	isAlive_ = isAlive;
}

void Transform3D::SetScale(const Vector3& scale) {
	scale_ = scale;
	isChanged_ = true;
}

void Transform3D::SetRotate(const Vector3& rotate) {
	inputRadians_ = rotate;
	isChanged_ = true;
}

void Transform3D::SetTranslate(const Vector3& translate) {
	translate_ = translate;
	isChanged_ = true;
}

void Transform3D::SetTranslateX(float x) {
	translate_.x = x;
	isChanged_ = true;
}

void Transform3D::SetTranslateY(float y) {
	translate_.y = y;
	isChanged_ = true;
}

void Transform3D::SetTranslateZ(float z) {
	translate_.z = z;
	isChanged_ = true;
}

void Transform3D::SetQuaternion(const Quaternion& rotate) {
	rotate_ = rotate;
	isChanged_ = true;
}

void Transform3D::AddScale(const Vector3& scale) {
	scale_ += scale;
	isChanged_ = true;
}

void Transform3D::AddRotate(const Vector3& rotate) {
	inputRadians_ += rotate;
	isChanged_ = true;
}

void Transform3D::AddTranslate(const Vector3& translate) {
	translate_ += translate;
	isChanged_ = true;
}
