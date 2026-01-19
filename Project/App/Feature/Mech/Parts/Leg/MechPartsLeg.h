#pragma once

#include <string>

#include "Feature/Mech/Parts/IMechParts.h"
#include "Math/Utility/MathUtility.h"

// Forward
class BaseMech;
class Transform3D;

class MechPartsLeg : public IMechParts {
public:
	struct InitParam {
		std::string waistModelName;

		std::string upperModelNameLeft;
		std::string lowerModelNameLeft;
		std::string footModelNameLeft;

		std::string upperModelNameRight;
		std::string lowerModelNameRight;
		std::string footModelNameRight;

		Vector3 waistTranslate{};

		Vector3 upperTranslateLeft{};
		Vector3 lowerTranslateLeft{};
		Vector3 footTranslateLeft{};

		Vector3 upperTranslateRight{};
		Vector3 lowerTranslateRight{};
		Vector3 footTranslateRight{};
	};

public:
	MechPartsLeg(const InitParam& param, BaseMech* mech);
	~MechPartsLeg() override = default;

	void Update() override;
	void Draw() override;
	void DebugDraw() override;

	void SetInitParam(const InitParam& param);
	void SetInitTranslate(const InitParam& param);

	Transform3D* GetWaistTransform();

	Transform3D* GetUpperTransformLeft();
	Transform3D* GetLowerTransformLeft();
	Transform3D* GetFootTransformLeft();

	Transform3D* GetUpperTransformRight();
	Transform3D* GetLowerTransformRight();
	Transform3D* GetFootTransformRight();

private:
	BaseMech* mech_ = nullptr;

	Transform3D* waistTrans_ = nullptr;

	Transform3D* upperTransLeft_ = nullptr;
	Transform3D* lowerTransLeft_ = nullptr;
	Transform3D* footTransLeft_ = nullptr;

	Transform3D* upperTransRight_ = nullptr;
	Transform3D* lowerTransRight_ = nullptr;
	Transform3D* footTransRight_ = nullptr;

	std::string waistModelName_;

	std::string upperModelNameLeft_;
	std::string lowerModelNameLeft_;
	std::string footModelNameLeft_;

	std::string upperModelNameRight_;
	std::string lowerModelNameRight_;
	std::string footModelNameRight_;
};
