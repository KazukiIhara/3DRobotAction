#include "MechAnimationEdit.h"

#include <numbers>
#include <algorithm>

#include "MechAnimation/Container/MechAnimationContainer.h"
#include "GameObject/Boss/Mech/BossMech.h"
#include "3D/Transform3D/Transform3D.h"
#include "ImGuiController/ImGuiController.h"

// 再生側
#include "MechAnimation/Animator/MechAnimator.h"

namespace {
	// Rad <-> Deg 変換係数
	constexpr float kRadToDeg = 180.0f / std::numbers::pi_v<float>;
	constexpr float kDegToRad = std::numbers::pi_v<float> / 180.0f;

	// ラジアン -> 度
	Vector3 ToDeg(const Vector3& r) {
		return { r.x * kRadToDeg, r.y * kRadToDeg, r.z * kRadToDeg };
	}

	// 度 -> ラジアン
	Vector3 ToRad(const Vector3& d) {
		return { d.x * kDegToRad, d.y * kDegToRad, d.z * kDegToRad };
	}

	float ClampMin0(float v) {
		return (v < 0.0f) ? 0.0f : v;
	}
}

MechAnimationEdit::MechAnimationEdit(MechAnimationContainer* container) {
	// コンテナ参照を保持
	container_ = container;
}

void MechAnimationEdit::SetBossMech(BossMech* mech) {
	// 編集対象Mechを差し替え
	mech_ = mech;

	// 再生状態をリセット
	isPlaying_ = false;
	playTimeSec_ = 0.0f;
}

bool MechAnimationEdit::AddAnimationClip(const std::string& name, const MechAnimation::Clip& clip, bool overwrite) {
	if (!container_) {
		return false;
	} // 未設定ガード

	// クリップ追加
	return container_->AddClip(name, clip, overwrite);
}

void MechAnimationEdit::Update() {
	if (!mech_) {
		return;
	} // 未設定ガード

	// 再生更新
	UpdatePlayback();

	// UI更新
	ShowWindow();
}

void MechAnimationEdit::ShowWindow() {
	if (!ImGui::Begin("MechAnimationEdit")) {
		ImGui::End();
		return;
	} // ウィンドウ非表示時

	// 2カラム（左：編集 / 右：Pose一覧）
	if (ImGui::BeginTable("##MechAnimEditTable", 2, ImGuiTableFlags_Resizable)) {

		//=========================
		// Left
		//=========================
		ImGui::TableNextColumn();

		ImGui::SeparatorText("Animation");

		// クリップ名入力
		ImGui::InputText("ClipName", clipName_, sizeof(clipName_));
		// 上書きフラグ
		ImGui::Checkbox("Overwrite", &overwrite_);

		// クリップ追加
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
						// 選択状態更新
						selectedClipIndex_ = i;
						selectedClipName_ = names[i];

						// Pose選択をリセット
						selectedPoseIndex_ = -1;

						// 再生を止める
						isPlaying_ = false;
						playTimeSec_ = 0.0f;
					}
				}
				ImGui::EndListBox();
			}
		}

		//=========================
		// Playback
		//=========================
		ImGui::SeparatorText("Playback");

		// 再生秒数（0未満禁止 / 初期1.0想定）
		playDurationSec_ = ClampMin0(playDurationSec_);
		ImGui::DragFloat("Duration (sec)", &playDurationSec_, 0.05f, 0.0f, 1000.0f, "%.2f");

		// ブレンド秒数（0未満禁止）
		lerpTimeSec_ = ClampMin0(lerpTimeSec_);
		ImGui::DragFloat("Lerp (sec)", &lerpTimeSec_, 0.02f, 0.0f, 1000.0f, "%.2f");

		// 再生操作
		if (ImGui::Button("Play")) {
			if (!selectedClipName_.empty()) {
				isPlaying_ = true;
				playTimeSec_ = 0.0f; // t=0から開始
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop")) {
			isPlaying_ = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset")) {
			playTimeSec_ = 0.0f;
			if (!selectedClipName_.empty()) {
				ApplyAtNormalizedTime(0.0f); // t=0を適用
			}
		}

		// 再生状況
		{
			const float t = (playDurationSec_ <= 0.0f) ? 0.0f : std::clamp(playTimeSec_ / playDurationSec_, 0.0f, 1.0f);
			ImGui::Text("t : %.3f", t);
		}

		//=========================
		// Pose buttons
		//=========================
		// ポーズ操作ボタン（Add / Delete / Apply）
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

		// 関節回転編集（MechAnimation::TransType に統一）
		if (ImGui::CollapsingHeader("Head / Body", ImGuiTreeNodeFlags_DefaultOpen)) {
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::Head), "Head");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::Body), "Body");
		}

		if (ImGui::CollapsingHeader("Arm Left", ImGuiTreeNodeFlags_DefaultOpen)) {
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::UpperArmLeft), "UpperArmLeft");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::LowerArmLeft), "LowerArmLeft");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::HandLeft), "HandLeft");
		}

		if (ImGui::CollapsingHeader("Arm Right", ImGuiTreeNodeFlags_DefaultOpen)) {
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::UpperArmRight), "UpperArmRight");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::LowerArmRight), "LowerArmRight");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::HandRight), "HandRight");
		}

		if (ImGui::CollapsingHeader("Leg", ImGuiTreeNodeFlags_DefaultOpen)) {
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::Waist), "Waist");

			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::UpperLegLeft), "UpperLegLeft");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::LowerLegLeft), "LowerLegLeft");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::FootLeft), "FootLeft");

			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::UpperLegRight), "UpperLegRight");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::LowerLegRight), "LowerLegRight");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::FootRight), "FootRight");
		}

		//=========================
		// Right
		//=========================
		ImGui::TableNextColumn();

		// Pose一覧表示
		ShowPoseList();

		ImGui::EndTable();
	}

	ImGui::End();
}

void MechAnimationEdit::UpdatePlayback() {
	if (!isPlaying_) {
		return;
	}
	if (!mech_) {
		return;
	}
	if (selectedClipName_.empty()) {
		return;
	}

	// 秒数0なら即終了（t=1適用）
	if (playDurationSec_ <= 0.0f) {
		ApplyAtNormalizedTime(1.0f);
		isPlaying_ = false;
		return;
	}

	// 経過時間加算
	const float dt = ImGui::GetIO().DeltaTime;
	playTimeSec_ += dt;

	// 正規化時間
	const float t = std::clamp(playTimeSec_ / playDurationSec_, 0.0f, 1.0f);

	// t適用
	ApplyAtNormalizedTime(t);

	// 終了
	if (t >= 1.0f) {
		isPlaying_ = false;
	}
}

void MechAnimationEdit::ApplyAtNormalizedTime(float t) {
	if (!mech_) {
		return;
	}
	MechAnimator* animator = mech_->GetAnimator();
	if (!animator) {
		return;
	}

	// クリップ再生（Animator側でt=0時の姿勢保存＆ブレンド開始）
	animator->ApplyAnimation(selectedClipName_, t, lerpTimeSec_);
}

void MechAnimationEdit::DrawRotate(BossMech* mech, int typeValue, const char* label) {
	// 型変換（MechAnimation::TransType）
	const auto type = static_cast<MechAnimation::TransType>(typeValue);

	// 対応トランスフォーム取得
	Transform3D* trans = mech->GetPartsTransform(type);
	if (!trans) {
		return;
	}

	// 現在回転（Deg）
	const Vector3 rotDeg = ToDeg(trans->GetRotate());
	float v[3] = { rotDeg.x, rotDeg.y, rotDeg.z };

	// 回転UI
	if (ImGui::DragFloat3(label, v, 0.5f, -180.0f, 180.0f, "%.1f")) {
		// 回転反映（Rad）
		trans->SetRotate(ToRad({ v[0], v[1], v[2] }));
	}
}

MechAnimation::Pose MechAnimationEdit::CaptureCurrentPose() const {
	MechAnimation::Pose pose{};

	// Identity
	const Quaternion identity{ 0.0f,0.0f,0.0f,1.0f };

	for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {
		// 対応トランスフォーム取得
		const auto type = static_cast<MechAnimation::TransType>(i);

		if (!mech_) {
			pose.rotations[i] = identity;
			continue;
		}

		Transform3D* trans = mech_->GetPartsTransform(type);
		if (trans) {
			// 現在回転を取得
			pose.rotations[i] = trans->GetQuaternion();
		} else {
			// 未取得はidentity
			pose.rotations[i] = identity;
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

	// 対象クリップ取得
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

	// 対象クリップ取得
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

	// 対象クリップ取得
	MechAnimation::Clip* clip = container_->GetClipMutable(selectedClipName_);
	if (!clip) {
		return;
	}

	const int count = static_cast<int>(clip->frames.size());
	if (selectedPoseIndex_ >= count) {
		return;
	}

	// 選択Poseに上書き
	clip->frames[static_cast<size_t>(selectedPoseIndex_)] = CaptureCurrentPose();
}

void MechAnimationEdit::ApplyPoseToMech(const MechAnimation::Pose& pose) {
	if (!mech_) {
		return;
	}

	for (size_t i = 0; i < MechAnimation::kJointCount; ++i) {
		// 対応トランスフォーム取得
		const auto type = static_cast<MechAnimation::TransType>(i);

		Transform3D* trans = mech_->GetPartsTransform(type);
		if (!trans) {
			continue;
		}

		// Pose適用
		trans->SetQuaternion(pose.rotations[i]);
	}
}

void MechAnimationEdit::ShowPoseList() {
	ImGui::SeparatorText("Pose List");

	if (!container_ || selectedClipName_.empty()) {
		ImGui::Text("Selected: None");
		return;
	}

	// 対象クリップ取得（const）
	const MechAnimation::Clip* clip = container_->GetClip(selectedClipName_);
	if (!clip) {
		ImGui::Text("Selected: %s", selectedClipName_.c_str());
		ImGui::Text("Clip not found");
		return;
	}

	ImGui::Text("Selected: %s", selectedClipName_.c_str());
	ImGui::Text("Pose Count: %d", static_cast<int>(clip->frames.size()));

	// Pose一覧
	if (ImGui::BeginListBox("##PoseList", ImVec2(-FLT_MIN, -FLT_MIN))) {
		for (int i = 0; i < static_cast<int>(clip->frames.size()); ++i) {
			const bool isSelected = (i == selectedPoseIndex_);

			char label[64];
			std::snprintf(label, sizeof(label), "Pose %d", i);

			if (ImGui::Selectable(label, isSelected)) {
				// Pose選択
				selectedPoseIndex_ = i;

				// 選択PoseをMechへ適用
				ApplyPoseToMech(clip->frames[static_cast<size_t>(i)]);
			}
		}
		ImGui::EndListBox();
	}
}
