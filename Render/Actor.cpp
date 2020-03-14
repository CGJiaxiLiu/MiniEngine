#include "Actor.h"
#include "util.h"
#include <d3d11.h>
#include <d3dx11tex.h>

Geometry::~Geometry()
{
	DX_RELEASE(this->m_texture);
}

bool Geometry::LoadTexture(ID3D11Device* device, WCHAR * filename)
{
	HRESULT result;

	// Load the texture in.
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
		
	if (FAILED(result))
	{
		BOX(D3DErrorParse(result), L"Geomety Error");
		return false;
	}

	return true;
}
