#include "SkinningRenderer3D.h"

#include "Framework/MAGI.h"


SkinningRenderer3D::SkinningRenderer3D(const std::string& objectName, const std::string& modelName)
	:StaticRenderer3D(objectName, modelName) {

}

SkinningRenderer3D::~SkinningRenderer3D() {

}

void SkinningRenderer3D::AssignShape() {
	// モデル取得
	model_ = std::make_unique<SkinningModel>(modelData_);
	model_->Initialize();
	assert(model_ && "Warning: Not found model");
}

void SkinningRenderer3D::Update() {
	// アニメーション再生中なら
	if (currentAnimationName_ != "") {
		animationTime_ += MAGISYSTEM::GetDeltaTime();
		animationTime_ = std::fmod(animationTime_, currentAnimation_.duration);

		if (auto skinmodel = static_cast<SkinningModel*>(model_.get())) {
			skinmodel->ApplyAnimation(currentAnimation_, animationTime_);
		}
	}
	// 基底クラスの更新
	StaticRenderer3D::Update();
}

void SkinningRenderer3D::PlayAnimation(const std::string& animationName) {
	// 名前をセット
	currentAnimationName_ = animationName;
	// アニメーションを持ってくる
	currentAnimation_ = MAGISYSTEM::FindAnimation(currentAnimationName_);
	// タイマーリセット
	animationTime_ = 0.0f;
}

void SkinningRenderer3D::ResetAnimation() {
	// 名前を削除
	currentAnimationName_ = "";
	// アニメーションをリセット
	if (auto skinmodel = static_cast<SkinningModel*>(model_.get())) {
		skinmodel->ApplyAnimation(currentAnimation_, 0.0f);
	}
	currentAnimation_ = AnimationData{};
	// タイマーリセット
	animationTime_ = 0.0f;
}

void SkinningRenderer3D::SwitchAnimation(const std::string& animationName) {
	animationName;
	// TODO::アニメーションの切り替え部分を補間する
}
