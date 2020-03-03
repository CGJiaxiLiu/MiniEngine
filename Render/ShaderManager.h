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
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMFLOAT4 color;
	};

	struct ParaBufferType
	{
		float intensity;  
		XMFLOAT3 padding;
	};

public:
	ShaderManager();
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND);
	void Destroy();
	bool SetMatrix(XMMATRIX, XMMATRIX, XMMATRIX);
	bool SetConstBuffer(MatrixBufferType* inData);
	bool Render(UINT, UINT, INT);

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void DestroyShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters();
	void Draw(UINT, UINT, INT);

private:
	ID3D11DeviceContext* m_context = 0;
	ID3D11VertexShader* m_vertexShader = 0;
	ID3D11PixelShader* m_pixelShader = 0;
	ID3D11InputLayout* m_layout = 0;
	ID3D11Buffer* m_matrixBuffer = 0;
	ID3D11Buffer* m_ParaBuffer = 0;
};
