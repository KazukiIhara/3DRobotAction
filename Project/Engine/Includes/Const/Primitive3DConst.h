#pragma once

// C++
#include <cstdint>

/// <summary>
/// 3Dプリミティブ形状で汎用的に使える定数
/// </summary>
namespace PrimitiveCommonConst {
	static const uint32_t NumMaxInstance = 65536;										// 全プリミティブ共通の最大インスタンス数
	static const uint32_t MaxThreadGroupCount = 4194303;								// DispatchMesh制限（全体上限）
}

/// <summary>
/// 3D球体で使う定数
/// </summary>
namespace Sphere3DConst {
	static const uint32_t MaxLongitude = 32;											// 経度分割数上限
	static const uint32_t MaxLatitude = 32;												// 緯度分割数上限
	static const uint32_t MaxTilesPerSphere = MaxLatitude * MaxLatitude;				// MeshShaderのX方向thread数
	static const uint32_t MaxDispatchCountY = PrimitiveCommonConst::MaxThreadGroupCount / MaxTilesPerSphere;
}

/// <summary>
/// 3Dリングで使う定数
/// </summary>
namespace Ring3DConst {
	static const uint32_t MaxDivide = 128; // 分割数の上限
	static const uint32_t MaxTilesPerRing = MaxDivide; // 1インスタンスあたりの分割に対応したスレッド数（X方向）
	static const uint32_t MaxDispatchCountY = PrimitiveCommonConst::MaxThreadGroupCount / MaxTilesPerRing;
}

/// <summary>
/// 三角形の頂点順
/// </summary>
namespace Triangle3DVertices {
	static const uint32_t LeftBottom = 0;
	static const uint32_t Top = 1;
	static const uint32_t RightBottom = 2;
}

/// <summary>
/// 板ポリの頂点順
/// </summary>
namespace Plane3DVertices {
	static const uint32_t LeftTop = 0;
	static const uint32_t RightTop = 1;
	static const uint32_t LeftBottom = 2;
	static const uint32_t RightBottom = 3;
}