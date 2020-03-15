#include "CameraActor.h"
#include "World.h"
#include "Application.h"
#include "util.h"

CameraActor::CameraActor(std::shared_ptr<Actor> PlayerActor)
{
	Actor::Actor(PlayerActor->GetWorld());
	this->playerActor = PlayerActor;
	this->bHasPhysicsProxy = false;
	this->bIsRendered = false;
	freeAreaRatio = 0.6f;
}

void CameraActor::Tick(double DeltaTime)
{
	switch (this->m_moveRule)
	{

	case SamePosition:
	{
		this->transformation = XMMatrixTranslationFromVector(this->playerActor->GetPosition());
		break;
	}
	default:
	{
		auto playerPos = this->playerActor->GetPosition();
		auto playerX = XMVectorGetX(playerPos);
		auto playerY = XMVectorGetY(playerPos);
		auto myPos = this->GetPosition();
		auto myX = XMVectorGetX(myPos);
		auto myY = XMVectorGetY(myPos);
		float deltaX = playerX - myX;
		float deltaY = playerY - myY;

		//LOG(L"deltaX: %f", deltaX);
		//LOG(L"deltaY: %f", deltaY);

		bool needUpdate = false;
		if (abs(deltaX / this->GetSceneWidth()) * 2.0f > freeAreaRatio)
		{
			myX = playerX - this->GetSceneWidth() / 2.0f * freeAreaRatio * (playerX > myX ? 1 : -1);
			needUpdate = true;
		}

		if (abs(deltaY / this->GetSceneHeight()) * 2.0f > freeAreaRatio)
		{
			myY = playerY - this->GetSceneHeight() / 2.0f * freeAreaRatio * (playerY > myY ? 1 : -1);
			needUpdate = true;
		}

		//LOG(L"deltaX: %f", myX);
		//LOG(L"deltaY: %f", myY);
		//LOG(L"NeedUpdate %d", needUpdate);

		if (needUpdate)
		{
			this->transformation = XMMatrixTranslation(myX, myY, 0.0f);
		}
	}
	}
}

void CameraActor::SetPlayerActor(std::shared_ptr<Actor> inActor)
{
	if (inActor != this->playerActor)
	{
		this->playerActor = inActor;
	}
}

XMMATRIX CameraActor::GetViewMatrix()
{
	auto viewMatrix = XMMatrixIdentity();

	XMVECTOR outTrans = XMVECTOR();
	XMVECTOR null_0 = XMVECTOR();
	XMVECTOR null_1 = XMVECTOR();
	XMMatrixDecompose(&null_0, &null_1, &outTrans, this->transformation);
	viewMatrix = XMMatrixTranslation(-XMVectorGetX(outTrans), -XMVectorGetY(outTrans), 0);
	viewMatrix = XMMatrixTranspose(viewMatrix);

	return viewMatrix;
}

XMMATRIX CameraActor::GetProjectionMatrix()
{
	return XMMatrixTranspose(XMMatrixOrthographicLH(this->GetSceneWidth(), this->GetSceneHeight(), 0.0f, 100.0f));
}

inline float CameraActor::GetSceneWidth()
{
	//return m_ScreenSizeRatio * this->m_world->GetApplication()->GetScreenWidth();
	return m_ScreenSizeRatio * GetAppInstance()->GetScreenWidth();
}

inline float CameraActor::GetSceneHeight()
{
	return m_ScreenSizeRatio * GetAppInstance()->GetScreenHeight();
}
