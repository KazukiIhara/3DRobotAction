#pragma once

// C++
#include <vector>
#include <memory>

// MyHedder
#include "Transform3D/Transform3D.h"

namespace magi {
	/// <summary>
	/// トランスフォーム管理クラス
	/// </summary>
	class TransformManager {
	public:
		TransformManager();
		~TransformManager();

		[[nodiscard]] Transform3D* Add(std::unique_ptr<Transform3D> transform);
		void Update();
		void DeleteGarbage();
		void Clear();

	private:
		// トランスフォームコンテナ
		std::vector<std::unique_ptr<Transform3D>> transforms_;
	};
}
