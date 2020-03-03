#pragma once
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>

class Application;
using namespace DirectX;

class GraphicsManager
{
public:
	struct PerActorData
	{
		XMFLOAT4 color;
	};

	GraphicsManager();
	bool Initialize(Application* inApp, std::shared_ptr<class Viewport> inViewport);
	void Render();
	void Destroy();
	XMFLOAT3 ScreenToWorld(float x, float y);

protected:
	Application* app = 0;
	std::shared_ptr<class D3DManager> m_D3D = 0;
	std::shared_ptr<class Viewport> m_viewport = 0;
	std::shared_ptr<class PolygonModel> m_model = 0;
	std::shared_ptr<class ShaderManager> m_shaderManager = 0;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_projectionMatrix_inv;
	//D3DXMATRIX m_worldMatrix;
};

