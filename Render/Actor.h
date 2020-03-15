#pragma once
#include <memory>
#include <vector>
#include "PxPhysicsAPI.h"
#include <DirectXMath.h>
#include "GraphicsManager.h"

using namespace physx;
using namespace DirectX;

class Geometry
{
public:
	std::vector<XMFLOAT3> positionData;
	std::vector<XMFLOAT2> uv;
	XMFLOAT2 uvOffset = XMFLOAT2(0.0f, 0.0f);
	std::vector<unsigned long> indexData;
	INT vertexOffset = 0;
	const WCHAR* texFileName = nullptr;
};

class Actor
{
public:

	XMMATRIX transformation;
	std::shared_ptr<Geometry> geo;
	PxRigidActor* physicsProxy;
	GraphicsManager::PerActorData actorRenderData;

	Actor()
	{
		this->actorRenderData.color = XMFLOAT4(0.0f, 1.0f, 1.0f, 0.0f);
	}

	virtual void Initialize() {};
	virtual void Tick(double DeltaTime) {};
};