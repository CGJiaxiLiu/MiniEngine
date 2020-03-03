#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "PxPhysicsAPI.h"

struct Actor;
struct Geometry;
using namespace physx;
using namespace DirectX;

class World
{
public:
	bool Initialize(class Application* inApp);
	void Destroy();
	void Tick(double DeltaTime);

	void AddActor(std::shared_ptr<Actor> inActor);
	void AddGeometry(std::shared_ptr<Geometry> inGeo);
	std::vector<std::shared_ptr<Actor>> GetAllActors() { return ActorList; }
	size_t GetVertexSize();
	size_t GetIndexSize();
	std::vector<std::shared_ptr<Geometry>> GetGeometryData() { return GeometryList; }
	//void StepPhysics(double DeltaTime);

protected:
	class Application* app;
	std::vector<std::shared_ptr<Actor>> ActorList;
	std::vector<std::shared_ptr<Geometry>> GeometryList;
	//XMFLOAT3 GetMouseWorldLocation();
	std::shared_ptr<Actor> mouse = nullptr;
};

