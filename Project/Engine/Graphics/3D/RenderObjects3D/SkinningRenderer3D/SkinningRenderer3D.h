#pragma once

#include "3D/RenderObjects3D/StaticRenderer3D/StaticRenderer3D.h"
#include "3D/Shapes3D/SkinningModel/SkinningModel.h"

/// <summary>
/// スキニングつき3Dオブジェクト(インスタンスの生成後初期化メソッドを呼んでください)
/// </summary>
class SkinningRenderer3D:public StaticRenderer3D {
public:
	SkinningRenderer3D(const std::string& objectName, const std::string& modelName);
	~SkinningRenderer3D()override;
	void AssignShape()override;
	void Update()override;

	void PlayAnimation(const std::string& animationName);
	void ResetAnimation();
	void SwitchAnimation(const std::string& animationName);

private:
	// アニメーション用変数
	float animationTime_ = 0.0f;
	// 現在再生中のアニメーション名
	std::string currentAnimationName_ = "";
	// 現在再生中のアニメーション
	AnimationData currentAnimation_{};
};