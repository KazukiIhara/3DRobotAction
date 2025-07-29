#include "ParticleUpdater3D.h"

ParticleUpdater3D::ParticleUpdater3D() {

}

void ParticleUpdater3D::AddParticle(const ParticleEffectEmitData& emitData) {

}

void ParticleUpdater3D::Update() {

}

uint32_t ParticleUpdater3D::GetInstanceDrawCount(BlendMode mode) const {
	return instanceCount_[static_cast<uint32_t>(mode)];
}

uint32_t ParticleUpdater3D::GetInstancingDrawSrvIndex(BlendMode mode) const {
	return instancingDrawSrvIndex_[static_cast<uint32_t>(mode)];
}
