#include "ParticleDrawer3D.h"


#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "PipelineManagers/ShadowPipelineManager/ShadowPipelineManager.h"
#include "Camera3DManager/Camera3DManager.h"
#include "LightManager/LightManager.h"
#include "ParticleUpdater3D/ParticleUpdater3D.h"

#include "Logger/Logger.h"
#include "MAGIUitility/MAGIUtility.h"

#include <cassert>

using namespace MAGIUtility;
using namespace MAGIMath;

ParticleDrawer3D::ParticleDrawer3D(
	DXGI* dxgi,
	DirectXCommand* directXCommand,
	SRVUAVManager* srvUavManager,
	GraphicsPipelineManager* graphicsPipelineManager,
	Camera3DManager* camera3DManager,
	ParticleUpdater3D* particleUpdater
) {

	dxgi_ = dxgi;
	directXCommand_ = directXCommand;
	srvUavManager_ = srvUavManager;
	graphicsPipelineManager_ = graphicsPipelineManager;
	camera3DManager_ = camera3DManager;
	particleUpdater_ = particleUpdater;

}

void ParticleDrawer3D::Draw(BlendMode mode) {
	const uint32_t inctanceCount = kMaxParticleNum;
	const uint32_t instancingSrvIndex = particleUpdater_->GetInstancingSrvIndex();

	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();

	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::ParticleEffect3D));
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::ParticleEffect3D, mode));

	camera3DManager_->TransferCurrentCamera(0); // b0

	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(instancingSrvIndex));
	commandList->SetGraphicsRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(0)); // t1000


	commandList->DispatchMesh(1, inctanceCount, 1);
}