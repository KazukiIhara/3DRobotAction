#pragma once

#include <string>

#include "Feature/Mech/Parts/IMechParts.h"
#include "Math/Utility/MathUtility.h"

// Forward
class BaseMech;

namespace Magi {
	class Transform3D;
}

class MechPartsHead : public IMechParts {
public:
	struct InitParam {
		std::string modelName;
		Vector3 translate{};
	};

public:
	MechPartsHead(const InitParam& param, BaseMech* mech);
	~MechPartsHead() override = default;

	void Update() override;
	void Draw() override;
	void DebugDraw() override;

	void SetInitParam(const InitParam& param);
	void SetInitTranslate(const InitParam& param);

	Magi::Transform3D* GetHeadTransform();

private:
	BaseMech* mech_ = nullptr;

	Magi::Transform3D* headTrans_ = nullptr;
	std::string headModelName_;
};
