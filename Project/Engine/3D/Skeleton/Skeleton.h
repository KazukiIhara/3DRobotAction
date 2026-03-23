#pragma once

// C++
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "Structs/ModelStruct.h"

namespace Magi {
	class Skeleton {
	public:
		Skeleton(const Node& rootNode);
		~Skeleton();

		void Update();

		int32_t GetRoot()const;
		std::map<std::string, int32_t> GetJointMap()const;
		std::vector<Joint> GetJoints()const;

	private:
		void Initialize(const Node& rootNode);
		// ジョイント作成
		int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent);
	private:
		int32_t root_;
		std::map<std::string, int32_t> jointMap_;
		std::vector<Joint> joints_;
	};
}