#pragma once
#include <d3d11.h>
#include <d3dx11async.h>
#include <DirectXMath.h>
#include <fstream>

using namespace std;
using namespace DirectX;

class ShaderManager
{
public:
	struct VSConstBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMFLOAT4 color;
	};

	struct PSConstBuffer
	{
		int index;
		XMFLOAT3 padding;
	};

public:
	ShaderManager();
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND);
	void Destroy();
	bool SetVSConstBuffer(VSConstBuffer* inData);
	bool SetPSConstBuffer(PSConstBuffer* inData);
	bool Render(UINT, UINT, INT);

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void DestroyShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);
	void Draw(UINT, UINT, INT);

private:
	ID3D11DeviceContext* m_context = 0;
	ID3D11VertexShader* m_vertexShader = 0;
	ID3D11PixelShader* m_pixelShader = 0;
	ID3D11InputLayout* m_layout = 0;
	ID3D11Buffer* m_VSConstBuffer = 0;
	ID3D11Buffer* m_PSConstBuffer = 0;
};
