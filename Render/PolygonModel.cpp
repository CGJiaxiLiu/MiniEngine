#include "PolygonModel.h"
#include <vector>
#include "World.h"
#include "Actor.h"
#include "util.h"

PolygonModel::PolygonModel()
{
}

bool PolygonModel::Initialize(ID3D11Device * device, std::shared_ptr<World> world)
{
	bool result;

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffer(device, world);
	if (!result)
	{
		return false;
	}

	return true;
}

void PolygonModel::Destroy()
{
	// Release the vertex and index buffers.
	DestroyBuffer();

	return;
}

void PolygonModel::Update(ID3D11DeviceContext * deviceContext )
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffer(deviceContext);

	return;
}

bool PolygonModel::InitializeBuffer(ID3D11Device * device, std::shared_ptr<World> world)
{
	std::vector<VertexType> vectexArr = std::vector<VertexType>();
	std::vector<unsigned long> indexArr = std::vector<unsigned long>();
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = world->GetVertexSize();
	m_indexCount = world->GetIndexSize();

	vectexArr.reserve(m_vertexCount);
	indexArr.reserve(m_indexCount);

	INT vertexOffset = 0;
	for (auto geo : world->GetGeometryData())
	{
		for (auto vec : geo->positionData)
		{
			VertexType temp = VertexType();
			temp.position = vec;
			temp.intensity = 1.0f;
			vectexArr.push_back(temp);
		}

		for (auto index : geo->indexData)
		{
			indexArr.push_back(index);
		}
		geo->vertexOffset = vertexOffset;
		vertexOffset += geo->positionData.size();
	}

	LOG(L"vertex count: %d", vectexArr.size());
	LOG(L"index count: %d", indexArr.size());

	//m_vertexCount = 6;
	//m_indexCount = 6;
	//vectexArr.resize(m_vertexCount);
	//indexArr.resize(m_indexCount);
	//vectexArr[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.5f);
	//vectexArr[0].intensity = 1.0f;
	//vectexArr[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.5f);
	//vectexArr[1].intensity = 1.0f;
	//vectexArr[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.5f);
	//vectexArr[2].intensity = 1.0f;
	//vectexArr[3].position = D3DXVECTOR3(-0.5f, -0.50f, 0.0f);
	//vectexArr[3].intensity = -1.0f;
	//vectexArr[4].position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	//vectexArr[4].intensity = -1.0f;
	//vectexArr[5].position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	//vectexArr[5].intensity = -1.0f;
	//indexArr[0] = 0;  // Bottom left.
	//indexArr[1] = 1;  // Top middle.
	//indexArr[2] = 2;  // Bottom right.
	//indexArr[3] = 3;  // Bottom left.
	//indexArr[4] = 4;  // Top middle.
	//indexArr[5] = 5;  // Bottom right.

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vectexArr.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indexArr.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void PolygonModel::DestroyBuffer()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void PolygonModel::RenderBuffer(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

