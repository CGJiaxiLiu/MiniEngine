#pragma once
#include <memory>
#include <vector>
#include "PxPhysicsAPI.h"
#include <DirectXMath.h>
#include "GraphicsManager.h"

using namespace physx;
using namespace DirectX;

struct Geometry
{
	std::vector<XMFLOAT3> positionData;
	std::vector<unsigned long> indexData;
	INT vertexOffset = 0;
};

struct Actor
{
	XMMATRIX transformation;
	std::shared_ptr<Geometry> geo;
	PxRigidActor* physicsProxy;
	GraphicsManager::PerActorData actorRenderData;

	Actor()
	{
		this->actorRenderData.color = XMFLOAT4(0.0f, 1.0f, 1.0f, 0.0f);
	}
};