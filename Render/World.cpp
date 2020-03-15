#include <Windows.h>
#include "GraphicsManager.h"
#include "World.h"
#include "Actor.h"
#include "Application.h"
#include "Viewport.h"
#include "util.h"

PxDefaultAllocator gAllocator;
PxDefaultErrorCallback gErrorCallback;
PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;
PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
PxMaterial*	gMaterial = NULL;

bool World::Initialize(class Application* inApp)
{
	this->app = inApp;
	ActorList = std::vector<std::shared_ptr<Actor>>();
	GeometryList = std::vector<std::shared_ptr<Geometry>>();

	std::shared_ptr<Geometry> boxGeo = std::make_shared<Geometry>();
	float halfboxSize = 0.5f;
	boxGeo->positionData = std::vector<XMFLOAT3>{
		XMFLOAT3(-halfboxSize, -halfboxSize, 0.0f),
		XMFLOAT3(-halfboxSize, halfboxSize, 0.0f),
		XMFLOAT3(halfboxSize, halfboxSize, 0.0f),
		XMFLOAT3(halfboxSize, -halfboxSize, 0.0f)
	};
	boxGeo->indexData = std::vector<unsigned long>{ 0, 1, 2, 2, 3, 0 };
	boxGeo->uv = std::vector<XMFLOAT2>{ XMFLOAT2(0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };
	boxGeo->texFileName = L"dragon.png";
	this->AddGeometry(boxGeo);

	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true);
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -15.0f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.2f);

	PxShape* boxShape = gPhysics->createShape(PxBoxGeometry(halfboxSize, halfboxSize, halfboxSize), *gMaterial);
	std::shared_ptr<Actor> actor0 = std::make_shared<Actor>();
	XMMATRIX actorTransform = XMMatrixTranslation(0, 0, 0);
	actor0->geo = boxGeo;
	actor0->transformation = actorTransform;
	this->AddActor(actor0);

	if (gPhysics)
	{
		app->LogOnConsole(L"gPhysics Intialized");
	}

	PxRigidDynamic* body = gPhysics->createRigidDynamic(PxTransform(0, 0, 0));
	if (body)
	{
		body->attachShape(*boxShape);
		body->setMaxDepenetrationVelocity(0.1);
		PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		gScene->addActor(*body);
		//body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		//body->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
		body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
		body->setMaxDepenetrationVelocity(PX_MAX_F32);
		actor0->physicsProxy = body;
		body->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z | PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y);
		player = actor0;
	}
	else
	{
		app->LogOnConsole(L"Create Rigid Body Fail");
	}

	std::shared_ptr<Actor> actor1 = std::make_shared<Actor>();
	float x = 3;
	actorTransform = XMMatrixTranslation(x, 0, 0);
	actor1->geo = boxGeo;
	actor1->transformation = actorTransform;
	this->AddActor(actor1);
	PxRigidDynamic* static_body = gPhysics->createRigidDynamic(PxTransform(x, 0, 0));
	static_body->attachShape(*boxShape);
	gScene->addActor(*static_body);
	actor1->physicsProxy = static_body;
	static_body->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z | PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y);
	static_body->setMaxDepenetrationVelocity(PX_MAX_F32);

	std::shared_ptr<Actor> floor = std::make_shared<Actor>();
	this->AddActor(floor);
	float y = -3;
	actorTransform = XMMatrixTranslation(0, y, 0);
	floor->transformation = actorTransform;

	std::shared_ptr<Geometry> floorGeo = std::make_shared<Geometry>();
	float halfFloorWidth = 5.0f;
	float halfFloorHeigh = 0.5f;
	floorGeo->positionData = std::vector<XMFLOAT3>{
		XMFLOAT3(-halfFloorWidth, -halfFloorHeigh, 0.0f),
		XMFLOAT3(-halfFloorWidth, halfFloorHeigh, 0.0f),
		XMFLOAT3(halfFloorWidth, halfFloorHeigh, 0.0f),
		XMFLOAT3(halfFloorWidth, -halfFloorHeigh, 0.0f)
	};
	floorGeo->uv = std::vector<XMFLOAT2>{ XMFLOAT2(0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT2(2 * halfFloorWidth, 0.0f), XMFLOAT2(2 * halfFloorWidth, 1.0f) };
	floorGeo->texFileName = L"brick.jpg";
	floorGeo->indexData = std::vector<unsigned long>{ 0, 1, 2, 2, 3, 0 };
	this->AddGeometry(floorGeo);
	floor->geo = floorGeo;

	PxShape* floorShape = gPhysics->createShape(PxBoxGeometry(halfFloorWidth, halfFloorHeigh, 1), *gMaterial);
	PxRigidStatic* floorPhy = gPhysics->createRigidStatic(PxTransform(0, y, 0));
	gScene->addActor(*floorPhy);
	floorPhy->attachShape(*floorShape);
	floor->physicsProxy = floorPhy;


	boxShape->release();
	floorShape->release();
	return true;
}

void World::Destroy()
{
	PX_RELEASE(gMaterial);
	PX_RELEASE(gScene);
	PX_RELEASE(gDispatcher);
	PX_RELEASE(gPhysics);
	PX_RELEASE(gFoundation);
}

void World::StepPhysics(double DeltaTime)
{

}

void World::Tick(double DeltaTime)
{
	if (gScene)
	{
		gScene->simulate(DeltaTime);
		gScene->fetchResults(true);
	}

	if (app->GetKeyDown(VK_UP))
	{
		LOG(L"Frame: %zd, (UP) KEY DOWN", app->GetFrameCount());
	}
	if (app->GetKeyUp(VK_UP))
	{
		LOG(L"Frame: %zd, (UP) KEY UP", app->GetFrameCount());
	}
	if (app->GetKeyDown(VK_DOWN))
	{
		LOG(L"Frame: %zd, (DOWN) KEY DOWN", app->GetFrameCount());
	}
	if (app->GetKeyUp(VK_DOWN))
	{
		LOG(L"Frame: %zd, (DOWN) KEY UP", app->GetFrameCount());
	}

	for (auto actor : this->ActorList)
	{
		if (actor->physicsProxy /*&& static_cast<PxRigidDynamic*>(actor->physicsProxy)*/)
		{
			actor->actorRenderData.color = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
			auto physics_pos = actor->physicsProxy->getGlobalPose().p;
			XMMATRIX translation = XMMatrixTranslation(physics_pos.x, physics_pos.y, 0);

			float angle;
			PxVec3 axis;
			actor->physicsProxy->getGlobalPose().q.toRadiansAndUnitAxis(angle, axis);
			XMMATRIX rotation = XMMatrixRotationZ(axis.z >= 0 ? angle : -angle);

			actor->transformation = rotation * translation;
			//app->LogOnConsole(L"position: x %f, y %f", physics_pos.x, physics_pos.y);
			//app->LogOnConsole(L"rot angle: %f", angle);
		}
	}
	
	if (player && player->physicsProxy)
	{
		player->actorRenderData.color = XMFLOAT4(0.0f, 1.0f, 1.0f, 0.0f);
		auto pos = app->GetCursorPosition();
		auto pos_ = app->GetGraphicsManager()->ScreenToWorld(pos.x, pos.y);
		PxRigidDynamic* body = static_cast<PxRigidDynamic*>(player->physicsProxy);

		if (body)
		{
			//body->setKinematicTarget(PxTransform(pos_.x, pos_.y, 0));
			//body->setGlobalPose(PxTransform(pos_.x, pos_.y, 0));
			auto currPos = body->getGlobalPose().p;
			float speed = 5.0f;

			if (app->GetKeyPressed(VK_UP))
			{
				currPos.y += speed * DeltaTime;
			}
			if (app->GetKeyPressed(VK_DOWN))
			{
				currPos.y -= speed * DeltaTime;
			}
			if (app->GetKeyPressed(VK_RIGHT))
			{
				currPos.x += speed * DeltaTime;
			}
			if (app->GetKeyPressed(VK_LEFT))
			{
				currPos.x -= speed * DeltaTime;
			}
			body->setGlobalPose(PxTransform(currPos));
			body->setLinearVelocity(PxVec3(0.0f, 0.0f, 0.0f));
		}
	}


	//for (auto actor : this->ActorList)
	//{
	//	D3DXMATRIX actorTransform;
	//	D3DXMatrixRotationZ(&actorTransform, DeltaTime);
	//	//app->LogOnConsole(L"Matrix: %f", actorTransform._12);
	//	actor->transformation = actorTransform * actor->transformation;
	//}

}

void World::AddActor(std::shared_ptr<Actor> inActor)
{
	ActorList.push_back(inActor);
}

void World::AddGeometry(std::shared_ptr<Geometry> inGeo)
{
	GeometryList.push_back(inGeo);
}

size_t World::GetVertexSize()
{
	size_t size = 0;

	for (auto geo : this->GeometryList)
	{
		size += geo->positionData.size();
	}

	return size;
}

size_t World::GetIndexSize()
{
	size_t size = 0;

	for (auto geo : this->GeometryList)
	{
		size += geo->indexData.size();
	}

	return size;
}

//XMFLOAT3 World::GetMouseWorldLocation()
//{
//	POINT p;
//	XMFLOAT3 pos = XMFLOAT3();
//
//	unsigned int width = this->app->GetScreenWidth();
//	unsigned int height = this->app->GetScreenHeight();
//
//	if (GetCursorPos(&p))
//	{
//		if (ScreenToClient(app->m_mainViewport->GetWindowHandle(), &p))
//		{
//			p.y = height - p.y;
//			float x = (p.x - width / 2.0f) / (width / 2.0f);
//			float y = (p.y - height / 2.0f) / (height / 2.0f);
//			pos.x = x / this->app->GetHorizontalRatio();
//			pos.y = y / this->app->GetVerticalRatio();
//			//this->app->LogOnConsole(L"x: %f, y: %f", x, y);
//		}
//	}
//
//	return pos;
//}
