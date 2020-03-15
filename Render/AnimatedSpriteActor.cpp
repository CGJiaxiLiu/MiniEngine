#include "AnimatedSpriteActor.h"


void AnimatedSpriteActor::Tick(double DeltaDeltaTime)
{
	Actor::Tick(DeltaDeltaTime);

	progress += DeltaDeltaTime / cycle;

	if (progress >= 1)
	{
		progress -= 1.0f;
	}

	index = (UINT)(progress * step);

	this->geo->uvOffset = uvOffsetList[index];
}
