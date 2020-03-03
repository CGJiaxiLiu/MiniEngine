#pragma once
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "Dxerr.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <memory>

class D3DManager
{
public:
	D3DManager();
	bool Initialize(std::shared_ptr<class Viewport> inViewport);
	void Destroy();

	void BeginScene();
	void EndScene();
	void CopyResource();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

protected:
	std::shared_ptr<class Viewport> m_viewport;

	IDXGISwapChain* m_swapChain = 0;
	ID3D11Device* m_device = 0;
	ID3D11DeviceContext* m_deviceContext = 0;
	ID3D11RenderTargetView* m_renderTargetView = 0;
	ID3D11Texture2D* m_depthStencilBuffer = 0;
	ID3D11DepthStencilState* m_depthStencilState = 0;
	ID3D11DepthStencilView* m_depthStencilView = 0;
	ID3D11RasterizerState* m_rasterState = 0;
	ID3D11Texture2D* m_customBuffer = 0;

};

