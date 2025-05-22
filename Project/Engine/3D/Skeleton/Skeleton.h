#pragma once

// C++
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "Structs/ModelStruct.h"

class Skeleton {
public:
	Skeleton(const Node& rootNode);
	~Skeleton();

	void Update();

private:
	void Initialize(const Node& rootNode);
	// ジョイント作成
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent);
public:
	int32_t root;
	std::map<std::string, int32_t> jointMap;
	std::vector<Joint> joints;
};