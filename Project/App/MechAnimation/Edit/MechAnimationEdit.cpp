#include "MechAnimationEdit.h"

#include "ImGuiController/ImGuiController.h"

#include "GameObject/Boss/Mech/BossMech.h"
#include "3D/Transform3D/Transform3D.h"

namespace {
	constexpr float kRadToDeg = 180.0f / std::numbers::pi_v<float>;
	constexpr float kDegToRad = std::numbers::pi_v<float> / 180.0f;

	Vector3 ToDeg(const Vector3& r) {
		return { r.x * kRadToDeg, r.y * kRadToDeg, r.z * kRadToDeg };
	}

	Vector3 ToRad(const Vector3& d) {
		return { d.x * kDegToRad, d.y * kDegToRad, d.z * kDegToRad };
	}
}

MechAnimationEdit::MechAnimationEdit(BossMech* mech) {
	mech_ = mech;
}

void MechAnimationEdit::SetBossMech(BossMech* mech) {
	mech_ = mech;
}

void MechAnimationEdit::SetAnimationContainer(MechAnimationContainer* container) {
	container_ = container; // コンテナ設定
}

bool MechAnimationEdit::AddAnimationClip(const std::string& name, const MechAnimationContainer::Clip& clip, bool overwrite) {
	if (!container_) {
		return false;
	} // 未設定ガード
	return container_->AddClip(name, clip, overwrite); // 追加
}

void MechAnimationEdit::Update() {
	if (!mech_) {
		return;
	}
	ShowWindow();
}

void MechAnimationEdit::ShowWindow() {
	if (!ImGui::Begin("MechAnimationEdit")) {
		ImGui::End();
		return;
	}

	if (ImGui::CollapsingHeader("Head / Body", ImGuiTreeNodeFlags_DefaultOpen)) {
		DrawRotate(mech_, static_cast<int>(BossMech::TransType::Head), "Head");
		DrawRotate(mech_, static_cast<int>(BossMech::TransType::Body), "Body");
	}

	if (ImGui::CollapsingHeader("Arm Left", ImGuiTreeNodeFlags_DefaultOpen)) {
		DrawRotate(mech_, static_cast<int>(BossMech::TransType::UpperArmLeft), "UpperArmLeft");
		DrawRotate(mech_, static_cast<int>(BossMech::TransType::LowerArmLeft), "LowerArmLeft");
		DrawRotate(mech_, static_cast<int>(BossMech::TransType::HandLeft), "HandLeft");
	}

	if (ImGui::CollapsingHeader("Arm Right", ImGuiTreeNodeFlags_DefaultOpen)) {
		DrawRotate(mech_, static_cast<int>(BossMech::TransType::UpperArmRight), "UpperArmRight");
		DrawRotate(mech_, static_cast<int>(BossMech::TransType::LowerArmRight), "LowerArmRight");
		DrawRotate(mech_, static_cast<int>(BossMech::TransType::HandRight), "HandRight");
	}

	if (ImGui::CollapsingHeader("Leg", ImGuiTreeNodeFlags_DefaultOpen)) {
		DrawRotate(mech_, static_cast<int>(BossMech::TransType::Waist), "Waist");

		DrawRotate(mech_, static_cast<int>(BossMech::TransType::UpperLegLeft), "UpperLegLeft");
		DrawRotate(mech_, static_cast<int>(BossMech::TransType::LowerLegLeft), "LowerLegLeft");
		DrawRotate(mech_, static_cast<int>(BossMech::TransType::FootLeft), "FootLeft");

		DrawRotate(mech_, static_cast<int>(BossMech::TransType::UpperLegRight), "UpperLegRight");
		DrawRotate(mech_, static_cast<int>(BossMech::TransType::LowerLegRight), "LowerLegRight");
		DrawRotate(mech_, static_cast<int>(BossMech::TransType::FootRight), "FootRight");
	}

	ImGui::End();
}

void MechAnimationEdit::DrawRotate(BossMech* mech, int typeValue, const char* label) {
	const auto type = static_cast<BossMech::TransType>(typeValue);
	Transform3D* trans = mech->GetPartsTransform(type);
	if (!trans) {
		return;
	}

	const Vector3 rotDeg = ToDeg(trans->GetRotate());
	float v[3] = { rotDeg.x, rotDeg.y, rotDeg.z };

	if (ImGui::DragFloat3(label, v, 0.5f, -180.0f, 180.0f, "%.1f")) {
		trans->SetRotate(ToRad({ v[0], v[1], v[2] })); // 回転反映
	}
}
