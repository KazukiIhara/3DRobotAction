#include "Skeleton.h"

using namespace Magi;
using namespace MAGIMath;

Skeleton::Skeleton(const Node& rootNode) {
	Initialize(rootNode);
}

Skeleton::~Skeleton() {}

void Skeleton::Update() {
	for (Joint& joint : joints_) {
		joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
		if (joint.parent) {
			joint.skeletonSpaceMatrix = joint.localMatrix * joints_[*joint.parent].skeletonSpaceMatrix;
		} else {
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
}

int32_t Magi::Skeleton::GetRoot() const {
	return root_;
}

std::map<std::string, int32_t> Magi::Skeleton::GetJointMap() const {
	return jointMap_;
}

std::vector<Joint> Magi::Skeleton::GetJoints() const {
	return joints_;
}

void Skeleton::Initialize(const Node& rootNode) {
	// ジョイントを作成
	root_ = CreateJoint(rootNode, {});
	// ジョイントマップを埋める
	for (const Joint& joint : joints_) {
		jointMap_.emplace(joint.name, joint.index);
	}

	Update();
}

int32_t Skeleton::CreateJoint(const Node& node, const std::optional<int32_t>& parent) {
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = MakeIdentityMatrix4x4();
	joint.transform = node.transform;
	joint.index = int32_t(joints_.size());
	joint.parent = parent;
	joints_.push_back(joint);
	for (const Node& child : node.children) {
		int32_t childIndex = CreateJoint(child, joint.index);
		joints_[joint.index].children.push_back(childIndex);
	}
	return joint.index;
}