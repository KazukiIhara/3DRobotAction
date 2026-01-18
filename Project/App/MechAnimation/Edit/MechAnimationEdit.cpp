#include "MechAnimationEdit.h"

#include <numbers>

#include "MechAnimation/Container/MechAnimationContainer.h"
#include "GameObject/Boss/Mech/BossMech.h"
#include "3D/Transform3D/Transform3D.h"
#include "ImGuiController/ImGuiController.h"

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

MechAnimationEdit::MechAnimationEdit(MechAnimationContainer* container, BossMech* mech) {
	mech_ = mech;
	container_ = container;
}

void MechAnimationEdit::SetBossMech(BossMech* mech) {
	mech_ = mech;
}

bool MechAnimationEdit::AddAnimationClip(const std::string& name, const MechAnimation::Clip& clip, bool overwrite) {
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

	// 2カラム（左：編集 / 右：Pose一覧）
	if (ImGui::BeginTable("##MechAnimEditTable", 2, ImGuiTableFlags_Resizable)) {

		//=========================
		// Left
		//=========================
		ImGui::TableNextColumn();

		ImGui::SeparatorText("Animation");

		ImGui::InputText("ClipName", clipName_, sizeof(clipName_));
		ImGui::Checkbox("Overwrite", &overwrite_);

		if (ImGui::Button("Add Clip")) {
			if (container_) {
				MechAnimation::Clip clip{};
				AddAnimationClip(std::string(clipName_), clip, overwrite_);
			}
		}

		// クリップ一覧
		if (container_) {
			const auto names = container_->GetClipNames();

			ImGui::SeparatorText("Clip List");

			if (ImGui::BeginListBox("##ClipList", ImVec2(-FLT_MIN, 140.0f))) {
				for (int i = 0; i < static_cast<int>(names.size()); ++i) {
					const bool isSelected = (i == selectedClipIndex_);
					if (ImGui::Selectable(names[i].c_str(), isSelected)) {
						selectedClipIndex_ = i;
						selectedClipName_ = names[i];
						selectedPoseIndex_ = -1; // クリップ切替でリセット
					}
				}
				ImGui::EndListBox();
			}
		}

		// ポーズを追加、削除、適用
		if (ImGui::Button("Add Pose")) {
			AddPoseToSelectedClip();
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete Pose")) {
			RemoveSelectedPose();
		}
		ImGui::SameLine();
		if (ImGui::Button("Apply Pose")) {
			ApplyCurrentToSelectedPose();
		}


		ImGui::SeparatorText("PoseEdit");

		// 関節回転編集
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

		//=========================
		// Right
		//=========================
		ImGui::TableNextColumn();
		ShowPoseList();

		ImGui::EndTable();
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

MechAnimation::Pose MechAnimationEdit::CaptureCurrentPose() const {
	MechAnimation::Pose pose{};

	// Identity
	const Quaternion identity{ 0.0f,0.0f,0.0f,1.0f };

	for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {
		const auto type = static_cast<BossMech::TransType>(i);
		if (mech_) {
			Transform3D* trans = mech_->GetPartsTransform(type);
			if (trans) {
				pose.rotations[i] = trans->GetQuaternion(); // 現在回転を取得
			} else {
				pose.rotations[i] = identity; // 未取得はidentity
			}
		}
	}

	return pose;
}

void MechAnimationEdit::AddPoseToSelectedClip() {
	if (!container_) {
		return;
	}
	if (selectedClipName_.empty()) {
		return;
	}

	MechAnimation::Clip* clip = container_->GetClipMutable(selectedClipName_);
	if (!clip) {
		return;
	}
	// 末尾に追加
	clip->frames.push_back(CaptureCurrentPose());
}

void MechAnimationEdit::RemoveSelectedPose() {
	if (!container_) {
		return;
	}
	if (selectedClipName_.empty()) {
		return;
	}
	if (selectedPoseIndex_ < 0) {
		return;
	}

	MechAnimation::Clip* clip = container_->GetClipMutable(selectedClipName_);
	if (!clip) {
		return;
	}

	const int count = static_cast<int>(clip->frames.size());
	if (selectedPoseIndex_ >= count) {
		selectedPoseIndex_ = -1;
		return;
	}

	// Pose削除
	clip->frames.erase(clip->frames.begin() + selectedPoseIndex_);

	// 選択位置調整
	if (clip->frames.empty()) {
		selectedPoseIndex_ = -1;
	} else {
		if (selectedPoseIndex_ >= static_cast<int>(clip->frames.size())) {
			selectedPoseIndex_ = static_cast<int>(clip->frames.size()) - 1;
		}
	}
}

void MechAnimationEdit::ApplyCurrentToSelectedPose() {
	if (!container_) {
		return;
	}
	if (selectedClipName_.empty()) {
		return;
	}
	if (selectedPoseIndex_ < 0) {
		return;
	}

	MechAnimation::Clip* clip = container_->GetClipMutable(selectedClipName_);
	if (!clip) {
		return;
	}

	const int count = static_cast<int>(clip->frames.size());
	if (selectedPoseIndex_ >= count) {
		return;
	}

	clip->frames[static_cast<size_t>(selectedPoseIndex_)] = CaptureCurrentPose(); // 上書き
}


void MechAnimationEdit::ApplyPoseToMech(const MechAnimation::Pose& pose) {
	if (!mech_) {
		return;
	}

	for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {
		const auto type = static_cast<BossMech::TransType>(i);
		Transform3D* trans = mech_->GetPartsTransform(type);
		if (!trans) {
			continue;
		}

		trans->SetQuaternion(pose.rotations[i]); // Pose適用
	}
}

void MechAnimationEdit::ShowPoseList() {
	ImGui::SeparatorText("Pose List");

	if (!container_ || selectedClipName_.empty()) {
		ImGui::Text("Selected: None");
		return;
	}

	const MechAnimation::Clip* clip = container_->GetClip(selectedClipName_);
	if (!clip) {
		ImGui::Text("Selected: %s", selectedClipName_.c_str());
		ImGui::Text("Clip not found");
		return;
	}

	ImGui::Text("Selected: %s", selectedClipName_.c_str());
	ImGui::Text("Pose Count: %d", static_cast<int>(clip->frames.size()));

	if (ImGui::BeginListBox("##PoseList", ImVec2(-FLT_MIN, -FLT_MIN))) {
		for (int i = 0; i < static_cast<int>(clip->frames.size()); ++i) {
			const bool isSelected = (i == selectedPoseIndex_);

			char label[64];
			std::snprintf(label, sizeof(label), "Pose %d", i);

			if (ImGui::Selectable(label, isSelected)) {
				// Pose選択
				selectedPoseIndex_ = i;

				// 選択したPoseをMechへ適用
				ApplyPoseToMech(clip->frames[static_cast<size_t>(i)]);
			}
		}
		ImGui::EndListBox();
	}
}
