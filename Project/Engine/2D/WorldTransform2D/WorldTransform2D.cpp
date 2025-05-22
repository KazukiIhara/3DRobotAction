#include "WorldTransform2D.h"

using namespace MAGIMath;

void WorldTransform2D::Initialize() {
	// 値を初期化
	size = { 256.0f,256.0f };
	rotateZ = 0.0f;
	position = { 0.0f,0.0f };
	Vector3 scale = { size.x,size.y,0.0f };
	Vector3 rotate = { 0.0f,0.0f,rotateZ };
	Vector3 translate = { position.x,position.y,0.0f };
	// 行列を初期化
	worldMatrix = MakeAffineMatrix(scale, rotate, translate);
}

void WorldTransform2D::Update() {
	// 4x4行列作成用に一時変数に挿入
	Vector3 scale = { size.x,size.y,0.0f };
	Vector3 rotate = { 0.0f,0.0f,rotateZ };
	Vector3 translate = { position.x,position.y,depth };
	// トランスフォームから行列を作る
	worldMatrix = MakeAffineMatrix(scale, rotate, translate);
}
