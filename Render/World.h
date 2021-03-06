#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "PxPhysicsAPI.h"

class Actor;
class Geometry;
class CameraActor;
using namespace physx;
using namespace DirectX;

class World
{
public:
	bool Initialize(class Application* inApp);
	void Destroy();
	void StepPhysics(double DeltaTime);
	void Tick(double DeltaTime);

	void AddActor(std::shared_ptr<Actor> inActor);
	void AddGeometry(std::shared_ptr<Geometry> inGeo);
	std::vector<std::shared_ptr<Actor>> GetAllActors() { return ActorList; }
	size_t GetVertexSize();
	size_t GetIndexSize();
	std::vector<std::shared_ptr<Geometry>> GetGeometryList() { return GeometryList; }
	std::shared_ptr<Actor> player = 0;
	std::shared_ptr<CameraActor> camera = 0;
	inline class Application* GetApplication() { return this->app; }

protected:
	class Application* app = 0;
	std::vector<std::shared_ptr<Actor>> ActorList;
	std::vector<std::shared_ptr<Geometry>> GeometryList;
	//XMFLOAT3 GetMouseWorldLocation();
};

