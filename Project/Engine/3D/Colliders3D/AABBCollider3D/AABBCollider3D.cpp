#include "AABBCollider3D.h"

#include "Framework/MAGI.h"

AABBCollider::AABBCollider(const std::string& name, uint32_t id, Collider3DType type)
	:BaseCollider3D(name, id, type) {

}

void AABBCollider::TransferShape() {
	// AABBの最小点と最大点を取得
	Vector3 min = GetMin();
	Vector3 max = GetMax();

	// AABBの8つの頂点を定義
	Vector3 vertices[8] = {
		{ min.x, min.y, min.z },  // 0: 左下手前
		{ max.x, min.y, min.z },  // 1: 右下手前
		{ max.x, max.y, min.z },  // 2: 右上手前
		{ min.x, max.y, min.z },  // 3: 左上手前
		{ min.x, min.y, max.z },  // 4: 左下奥
		{ max.x, min.y, max.z },  // 5: 右下奥
		{ max.x, max.y, max.z },  // 6: 右上奥
		{ min.x, max.y, max.z }   // 7: 左上奥
	};

	// AABBの12本のエッジを定義（各エッジの頂点ペア）
	int edges[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0},  // 前面の四辺
		{4, 5}, {5, 6}, {6, 7}, {7, 4},  // 背面の四辺
		{0, 4}, {1, 5}, {2, 6}, {3, 7}   // 前後を結ぶ四辺
	};

	// エッジを描画
	for (int i = 0; i < 12; ++i) {
		MAGISYSTEM::DrawLine3D(vertices[edges[i][0]], vertices[edges[i][1]], color_);
	}
}

Vector3& AABBCollider::GetOffsetMin() {
	return offsetMin_;
}

Vector3& AABBCollider::GetOffsetMax() {
	return offsetMax_;
}

Vector3 AABBCollider::GetMin() const {
	return worldPosition + offsetMin_;
}

Vector3 AABBCollider::GetMax() const {
	return worldPosition + offsetMax_;
}
