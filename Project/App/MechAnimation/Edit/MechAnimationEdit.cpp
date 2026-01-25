#include "MechAnimationEdit.h"

#include <numbers>
#include <algorithm>

#include "MechAnimation/Container/MechAnimationContainer.h"
#include "3D/Transform3D/Transform3D.h"
#include "ImGuiController/ImGuiController.h"

#include "Feature/Mech/Base/BaseMech.h"

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

void MechAnimationEdit::SetBaseMech(BaseMech* mech) {
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

void MechAnimationEdit::DeleteSelectedClip() {
	if (!container_) {
		return;
	}
	if (selectedClipName_.empty()) {
		return;
	}

	// 再生停止
	isPlaying_ = false;
	playTimeSec_ = 0.0f;

	if (mech_) {
		MechAnimator* animator = mech_->GetAnimator();
		if (animator) {
			animator->StopAnimation();
		}
	}

	// 削除
	container_->RemoveClip(selectedClipName_);

	// 選択状態リセット
	selectedClipName_.clear();
	selectedClipIndex_ = -1;
	selectedPoseIndex_ = -1;
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

		// クリップ削除
		if (ImGui::Button("Delete Clip")) {
			DeleteSelectedClip();
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

		// イージング設定
		{
			static const char* easingNames[] = {
				"Linear",
				"EaseInSine","EaseOutSine","EaseInOutSine",
				"EaseInQuad","EaseOutQuad","EaseInOutQuad",
				"EaseInCubic","EaseOutCubic","EaseInOutCubic",
				"EaseInQuart","EaseOutQuart","EaseInOutQuart",
				"EaseInQuint","EaseOutQuint","EaseInOutQuint",
				"EaseInExpo","EaseOutExpo","EaseInOutExpo",
				"EaseInCirc","EaseOutCirc","EaseInOutCirc",
				"EaseInBack","EaseOutBack","EaseInOutBack",
				"EaseInElastic","EaseOutElastic","EaseInOutElastic",
				"EaseInBounce","EaseOutBounce","EaseInOutBounce",
			};

			int easingIndex = static_cast<int>(easingType_);
			if (ImGui::Combo("Easing", &easingIndex, easingNames, IM_ARRAYSIZE(easingNames))) {
				easingType_ = static_cast<EasingType>(easingIndex);
			}
		}

		// 再生操作
		if (ImGui::Button("Play")) {
			if (!selectedClipName_.empty() && mech_) {
				MechAnimator* animator = mech_->GetAnimator();
				if (animator) {
					isPlaying_ = true;
					playTimeSec_ = 0.0f;
					// Animatorで再生
					animator->PlayAnimation(selectedClipName_, playDurationSec_, lerpTimeSec_, easingType_);
				}
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("Stop")) {
			isPlaying_ = false;

			if (mech_) {
				MechAnimator* animator = mech_->GetAnimator();
				if (animator) {
					animator->StopAnimation();
				}
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("Reset")) {
			playTimeSec_ = 0.0f;

			if (!selectedClipName_.empty() && mech_) {
				MechAnimator* animator = mech_->GetAnimator();
				if (animator) {
					animator->StopAnimation();
					animator->ApplyAnimation(selectedClipName_, 0.0f);
				}
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

		// Waistの
		if (ImGui::CollapsingHeader("WaistTrans", ImGuiTreeNodeFlags_DefaultOpen)) {
			DrawTranslate(mech_, static_cast<int>(MechAnimation::TransType::Waist), "WaistTranslate");
		}
		// 関節回転編集
		if (ImGui::CollapsingHeader("Head", ImGuiTreeNodeFlags_DefaultOpen)) {
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::Head), "Head");
		}

		if (ImGui::CollapsingHeader(" Body", ImGuiTreeNodeFlags_DefaultOpen)) {
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::Body), "Body");
		}

		if (ImGui::CollapsingHeader("Arm", ImGuiTreeNodeFlags_DefaultOpen)) {
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::UpperArmRight), "UpperArmRight");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::UpperArmLeft), "UpperArmLeft");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::LowerArmRight), "LowerArmRight");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::LowerArmLeft), "LowerArmLeft");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::HandRight), "HandRight");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::HandLeft), "HandLeft");
		}

		if (ImGui::CollapsingHeader("Leg", ImGuiTreeNodeFlags_DefaultOpen)) {
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::Waist), "Waist");

			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::UpperLegRight), "UpperLegRight");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::UpperLegLeft), "UpperLegLeft");

			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::LowerLegRight), "LowerLegRight");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::LowerLegLeft), "LowerLegLeft");

			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::FootRight), "FootRight");
			DrawRotate(mech_, static_cast<int>(MechAnimation::TransType::FootLeft), "FootLeft");
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

	MechAnimator* animator = mech_->GetAnimator();
	if (!animator) {
		isPlaying_ = false;
		return;
	}

	// 時間表示用
	const float dt = ImGui::GetIO().DeltaTime;
	playTimeSec_ += dt;

	// 再生更新
	animator->Update();

	// 終了判定
	if (!animator->IsPlaying()) {
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

	// ここは「単にその時点を適用」
	animator->ApplyAnimation(selectedClipName_, t);
}

void MechAnimationEdit::DrawRotate(BaseMech* mech, int typeValue, const char* label) {
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

void MechAnimationEdit::DrawTranslate(BaseMech* mech, int typeValue, const char* label) {
	// 型変換（MechAnimation::TransType）
	const auto type = static_cast<MechAnimation::TransType>(typeValue);

	// 対応トランスフォーム取得
	Transform3D* trans = mech->GetPartsTransform(type);
	if (!trans) {
		return;
	}

	// 現在位置
	const Vector3 t = trans->GetTranslate();
	float v[3] = { t.x, t.y, t.z };

	// 位置UI
	if (ImGui::DragFloat3(label, v, 0.01f, -1000.0f, 1000.0f, "%.3f")) {
		// 位置反映
		trans->SetTranslate(Vector3{ v[0], v[1], v[2] });
	}
}


MechAnimation::Pose MechAnimationEdit::CaptureCurrentPose() const {
	MechAnimation::Pose pose{};

	// Identity
	const Quaternion identity{ 0.0f,0.0f,0.0f,1.0f };

	// Waist translate初期化
	pose.waistTranslate = Vector3{ 0.0f, 0.0f, 0.0f };

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
			// 未取得はIdentity
			pose.rotations[i] = identity;
		}
	}

	// Waistの位置を保存
	if (mech_) {
		Transform3D* waist = mech_->GetPartsTransform(MechAnimation::TransType::Waist);
		if (waist) {
			pose.waistTranslate = waist->GetTranslate(); // 現在位置
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

		// Pose適用（回転）
		trans->SetQuaternion(pose.rotations[i]);
	}

	// Waistの位置反映
	{
		Transform3D* waist = mech_->GetPartsTransform(MechAnimation::TransType::Waist);
		if (waist) {
			waist->SetTranslate(pose.waistTranslate); // 位置反映
		}
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
