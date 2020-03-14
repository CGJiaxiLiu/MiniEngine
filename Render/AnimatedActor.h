#pragma once
#include "Actor.h"

class TexturedActor : public Actor
{
public:
	TexturedActor();
	~TexturedActor();

	virtual void Tick(double DeltaTime) override;
};

