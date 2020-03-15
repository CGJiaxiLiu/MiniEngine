#pragma once
#include "Actor.h"

class AnimatedSpriteActor :
	public Actor
{
public:
	virtual void Tick(double DeltaDeltaTime) override;

	float cycle = 1.0f;
	UINT step = 1;
	std::vector<XMFLOAT2> uvOffsetList;

protected:
	float progress = 0.0f;
	UINT index;
};