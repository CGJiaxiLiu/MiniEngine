#pragma once
#include <memory>
#include <vector>
#include "PxPhysicsAPI.h"
#include <DirectXMath.h>
#include "GraphicsManager.h"

using namespace physx;
using namespace DirectX;
class World;

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

	Actor();
	Actor(World* inWorld);
	virtual void Initialize() {};
	virtual void Tick(double DeltaTime) {};
	inline void SetWorld(World* inWorld) { m_world = inWorld; }
	inline World* GetWorld() { return m_world; }
	inline bool GetHasPhysics() { return this->bHasPhysicsProxy; }
	inline bool GetIsRendered() { return this->bIsRendered; }
	XMVECTOR GetPosition();

protected:
	World* m_world = 0;
	bool bHasPhysicsProxy = true;
	bool bIsRendered = true;
};