#pragma once
#include "Actor.h"

class CameraActor :	public Actor
{

public:
	enum MovingMethod
	{
		SamePosition,
		KeepBoundry,
		Physics,
	};

	CameraActor(std::shared_ptr<Actor> PlayerActor);
	virtual void Tick(double DeltaTime) override;
	void SetPlayerActor(std::shared_ptr<Actor> inActor);
	inline std::shared_ptr<Actor> GetPlayerActor() { return this->playerActor; }
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	
	inline float GetScreenSizeRatio() { return this->m_ScreenSizeRatio; }
	inline void SetScreenSizeRatio(float in) { this->m_ScreenSizeRatio = in; }
	inline float GetFreeAreaRatio() { return this->freeAreaRatio; }
	inline void SetFreeAreaRatio(float in) { this->freeAreaRatio = in; }
	inline float GetSceneWidth();
	inline float GetSceneHeight();

protected:
	float m_ScreenSizeRatio = 0.02f;
	float freeAreaRatio = 0.6;
	std::shared_ptr<Actor> playerActor = 0;
	MovingMethod m_moveRule = MovingMethod::KeepBoundry;
};

