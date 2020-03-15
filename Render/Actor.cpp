#include "Actor.h"
#include "util.h"
#include <d3d11.h>

Actor::Actor(World * inWorld)
{
	this->m_world = inWorld;
	this->actorRenderData.color = XMFLOAT4(0.0f, 1.0f, 1.0f, 0.0f);
	this->transformation = XMMatrixIdentity();
}

Actor::Actor()
{
	this->actorRenderData.color = XMFLOAT4(0.0f, 1.0f, 1.0f, 0.0f);
	this->transformation = XMMatrixIdentity();
}

XMVECTOR Actor::GetPosition()
{
	XMVECTOR outTrans = XMVECTOR();
	XMVECTOR null_0 = XMVECTOR();
	XMVECTOR null_1 = XMVECTOR();
	XMMatrixDecompose(&null_0, &null_1, &outTrans, this->transformation);
	return outTrans;
}