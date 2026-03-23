#pragma once

#include <string>

#include "Feature/Mech/Parts/IMechParts.h"
#include "Math/Utility/MathUtility.h"

// Forward
class BaseMech;

namespace Magi {
	class Transform3D;
}

class MechPartsArm : public IMechParts {
public:
	struct InitParam {
		std::string upperModelName;
		std::string lowerModelName;
		std::string handModelName;

		Vector3 upperTranslate{};
		Vector3 lowerTranslate{};
		Vector3 handTranslate{};
	};

public:
	MechPartsArm(const InitParam& param, BaseMech* mech);
	~MechPartsArm() override = default;

	void Update() override;
	void Draw() override;
	void DebugDraw() override;

	void SetInitParam(const InitParam& param);
	void SetInitTranslate(const InitParam& param);

	Magi::Transform3D* GetUpperTransform();
	Magi::Transform3D* GetLowerTransform();
	Magi::Transform3D* GetHandTransform();

private:
	BaseMech* mech_ = nullptr;

	Magi::Transform3D* upperTrans_ = nullptr;
	Magi::Transform3D* lowerTrans_ = nullptr;
	Magi::Transform3D* handTrans_ = nullptr;

	std::string upperModelName_;
	std::string lowerModelName_;
	std::string handModelName_;
};
