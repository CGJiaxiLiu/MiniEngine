 #pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
class World;

using namespace DirectX;

class PolygonModel
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		float padding;
		XMFLOAT2 uv;
		XMFLOAT2 padding_2;
	};

public:
	PolygonModel();
	bool Initialize(ID3D11Device* device, std::shared_ptr<World> world);
	void Destroy();
	void Update(ID3D11DeviceContext*);
	int GetIndexCount() { return m_indexCount; }

private:
	bool InitializeBuffer(ID3D11Device* device, std::shared_ptr<World> world);
	void DestroyBuffer();
	void RenderBuffer(ID3D11DeviceContext*);

	ID3D11Buffer *m_vertexBuffer = 0, *m_indexBuffer = 0;
	int m_vertexCount, m_indexCount;
};
