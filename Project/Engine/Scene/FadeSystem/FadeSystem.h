#pragma once

#include "Structs/SpriteStruct.h"

// フェード処理を行うシステム
class FadeSystem {
public:
	enum class State {
		None,
		FadeOut,
		FadeIn,
	};

public:
	FadeSystem();
	~FadeSystem() = default;

	void Update();
	void Draw();

	void StartFadeOut(float durationSec);
	void StartFadeIn(float durationSec);

	bool IsBusy() const;
	bool IsFadeOutFinished() const;

	void SetBlack();
	void SetClear();

private:
	State state_ = State::FadeOut;

	SpriteData data_;
	SpriteMaterialData mat_;

	float durationSec_ = 0.0f;
	float timeSec_ = 0.0f;
	float alpha_ = 0.0f;
};
