#include "GraphicsManager.h"
#include "Viewport.h"
#include "D3DManager.h"
#include "util.h"
#include "PolygonModel.h"
#include "ShaderManager.h"
#include "Application.h"
#include "World.h"
#include "Actor.h"

GraphicsManager::GraphicsManager()
{
}

bool GraphicsManager::Initialize(Application* inApp, std::shared_ptr<class Viewport> inViewport)
{
	this->app = inApp;
	this->m_viewport = inViewport;

	bool result = false;

	m_D3D = std::make_shared<D3DManager>();

	result = m_D3D->Initialize(inViewport);
	if (!result) {
		LOG(L"D3D Initialize Fail");
		return false;

	}
	else {
		LOG(L"D3D Initialize Success");
	}

	m_model = std::make_shared<PolygonModel>();
	result = m_model->Initialize(m_D3D->GetDevice(), this->app->GetWorld());
	if (!result) {
		return false;
	}
	else {
		LOG(L"Polygon Model Initialize Success");
	}

	m_shaderManager = std::make_shared<ShaderManager>();
	result = m_shaderManager->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), m_viewport->GetWindowHandle());
	if (!result) {
		return false;
	}
	else {
		LOG(L"Shader Manager Initialize Success");
	}

	return true;
}

void GraphicsManager::Render()
{
	m_D3D->BeginScene();
	m_model->Update(m_D3D->GetDeviceContext());

	m_viewMatrix = XMMatrixIdentity();
	float ratio = 0.02f;
	m_projectionMatrix = XMMatrixOrthographicLH(ratio * app->GetScreenWidth(), ratio * app->GetScreenHeight(), 0.0f, 100.0f);

	auto world = app->GetWorld();
	// Transpose the matrices to prepare them for the shader.
	
	//m_viewMatrix = XMMatrixTranspose(m_viewMatrix);
	//m_projectionMatrix = XMMatrixTranspose(m_projectionMatrix);
	m_projectionMatrix_inv = XMMatrixInverse(nullptr, m_projectionMatrix);

	for (auto actor : world->GetAllActors())
	{
		
		XMMATRIX worldMatrix = actor->transformation;
		worldMatrix = XMMatrixTranspose(worldMatrix);
		UINT indexCount = actor->geo->indexData.size();
		UINT startIndexLocation = 0;
		INT startVertexLocation = actor->geo->vertexOffset;
		ShaderManager::MatrixBufferType buffer;
		buffer.world = worldMatrix;
		buffer.view = m_viewMatrix;
		buffer.projection = m_projectionMatrix;
		buffer.color = actor->actorRenderData.color;
		//m_shaderManager->SetMatrix(worldMatrix, m_viewMatrix, m_projectionMatrix);
		m_shaderManager->SetConstBuffer(&buffer);
		m_shaderManager->Render(indexCount, startIndexLocation, startVertexLocation);
	}
	
	m_D3D->CopyResource();
	m_D3D->EndScene();
}

void GraphicsManager::Destroy()
{
	m_D3D->Destroy();
	m_model->Destroy();
	m_shaderManager->Destroy();
}

XMFLOAT3 GraphicsManager::ScreenToWorld(float x, float y)
{
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, XMVector3Transform(XMVectorSet(x, y, 0.0f, 0.0f), m_projectionMatrix_inv));
	return pos;
}

//D3DXVECTOR3 GraphicsManager::ScreenToWorld(float x, float y)
//{
//	XMFLOAT3 ScreenPos = XMFLOAT3(x, y, 0);
//	return XMVector3Transform(ScreenPos , m_projectionMatrix_inv);
//}
