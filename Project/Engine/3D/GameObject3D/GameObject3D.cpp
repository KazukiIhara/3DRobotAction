#include "GameObject3D.h"

#include <memory>

#include "framework/MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

GameObject3D::GameObject3D(const std::string& name, const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	name_ = name;
	std::unique_ptr<Transform3D> transform = std::make_unique<Transform3D>(scale, rotate, translate);
	transformComponent_ = MAGISYSTEM::AddTransform3D(std::move(transform));
}

GameObject3D::GameObject3D(const std::string& name, const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	name_ = name;
	std::unique_ptr<Transform3D> transform = std::make_unique<Transform3D>(scale, rotate, translate);
	transformComponent_ = MAGISYSTEM::AddTransform3D(std::move(transform));
}

GameObject3D::GameObject3D(const std::string& name, const Vector3& translate) {
	name_ = name;
	std::unique_ptr<Transform3D> transform = std::make_unique<Transform3D>(translate);
	transformComponent_ = MAGISYSTEM::AddTransform3D(std::move(transform));
}

void GameObject3D::Update() {


}

void GameObject3D::Finalize() {
	{
		if (transformComponent_) {
			transformComponent_->SetIsAlive(false);
		}
	}

	{
		if (!modelRendererComponents_.empty()) {
			for (auto& modelRenderer : modelRendererComponents_) {
				if (auto it = modelRenderer.second.lock()) {
					it->SetIsAlive(false);
				}
			}
		}
	}

	{
		if (!camera3DComponents_.empty()) {
			for (auto& camera3D : camera3DComponents_) {
				if (auto it = camera3D.second.lock()) {
					it->SetIsAlive(false);
				}
			}
		}
	}
}

void GameObject3D::AddModelRenderer(std::shared_ptr<ModelRenderer> modelRenderer) {
	modelRenderer->GetTransform()->SetParent(transformComponent_, false);
	std::weak_ptr<ModelRenderer> ptr = MAGISYSTEM::AddRenderer3D(std::move(modelRenderer));
	if (auto p = ptr.lock()) {
		modelRendererComponents_.insert(std::make_pair(p->GetName(), p));
	}
}

void GameObject3D::AddCamera3D(std::shared_ptr<Camera3D> camera3D) {
	std::weak_ptr<Camera3D> ptr = MAGISYSTEM::AddCamera3D(std::move(camera3D));
	camera3DComponents_.insert(std::make_pair(ptr.lock()->GetName(), ptr));
}

void GameObject3D::SetIsAlive(bool isAlive) {
	isAlive_ = isAlive;
}

void GameObject3D::SetIsActive(bool isActive) {
	isActive_ = isActive;
}

void GameObject3D::SetIsUnique(bool isUnique) {
	isUnique_ = isUnique;
}

const std::string& GameObject3D::GetName()const {
	return name_;
}

bool GameObject3D::GetIsAlive()const {
	return isAlive_;
}

bool GameObject3D::GetIsActive() const {
	return isActive_;
}

bool GameObject3D::GetIsUnique() const {
	return isUnique_;
}

Transform3D* GameObject3D::GetTransform() {
	return transformComponent_;
}

std::weak_ptr<ModelRenderer> GameObject3D::GetModelRenderer(const std::string& rendererName) {
	const auto it = modelRendererComponents_.find(rendererName);
	if (it != modelRendererComponents_.end()) {
		return it->second;
	}

	MAGIAssert::Assert(false, "GameObject3D " + name_ + ": Not found renderer [ " + rendererName + " ]\n");

	return {};
}

std::weak_ptr<Camera3D> GameObject3D::GetCamera3D(const std::string& camera3DName) {
	const auto it = camera3DComponents_.find(camera3DName);
	if (it != camera3DComponents_.end()) {
		return it->second;
	}

	MAGIAssert::Assert(false, "GameObject3D " + name_ + ": Not found camera3D [ " + camera3DName + " ]\n");	

	return {};
}
