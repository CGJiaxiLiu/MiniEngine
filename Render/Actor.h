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
	std::vector<unsigned long> indexData;
	INT vertexOffset = 0;
	ID3D11ShaderResourceView* m_texture = nullptr;

	~Geometry();
	bool LoadTexture(ID3D11Device* device, WCHAR* filename);
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