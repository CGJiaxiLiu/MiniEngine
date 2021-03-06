#include "GraphicsManager.h"
#include "Viewport.h"
#include "D3DManager.h"
#include "util.h"
#include "PolygonModel.h"
#include "ShaderManager.h"
#include "Application.h"
#include "World.h"
#include "Actor.h"
#include "CameraActor.h"

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
		BOX(L"D3D Initialize Fail", L"Error");
		return false;
	}
	else {
		LOG(L"D3D Initialize Success");
	}

	m_model = std::make_shared<PolygonModel>();
	result = m_model->Initialize(m_D3D->GetDevice(), this->app->GetWorld());
	if (!result) {
		BOX(L"Model Initialize Fail", L"Error");
		return false;
	}
	else {
		LOG(L"Polygon Model Initialize Success");
	}

	m_shaderManager = std::make_shared<ShaderManager>();
	result = m_shaderManager->Initialize(this->app, m_D3D->GetDevice(), m_D3D->GetDeviceContext(), m_viewport->GetWindowHandle());
	if (!result) {
		BOX(L"Shader Manager Initialize Fail", L"Error");
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

	m_viewMatrix = this->app->GetWorld()->camera->GetViewMatrix();
	m_projectionMatrix = this->app->GetWorld()->camera->GetProjectionMatrix();

	//m_viewMatrix = XMMatrixIdentity();
	//if (this->app && this->app->GetWorld() && this->app->GetWorld()->player)
	//{
	//	XMVECTOR outTrans = XMVECTOR();
	//	XMVECTOR null_0 = XMVECTOR();
	//	XMVECTOR null_1 = XMVECTOR();
	//	XMMatrixDecompose(&null_0, &null_1, &outTrans, this->app->GetWorld()->player->transformation);
	//	m_viewMatrix = XMMatrixTranslation(-XMVectorGetX(outTrans), -XMVectorGetY(outTrans), 0);
	//	m_viewMatrix = XMMatrixTranspose(m_viewMatrix);
	//}
	//float ratio = 0.02f;
	//m_projectionMatrix = XMMatrixOrthographicLH(ratio * app->GetScreenWidth(), ratio * app->GetScreenHeight(), 0.0f, 100.0f);

	// Transpose the matrices to prepare them for the shader.
	
	//m_viewMatrix = XMMatrixTranspose(m_viewMatrix);
	//m_projectionMatrix = XMMatrixTranspose(m_projectionMatrix);
	auto world = app->GetWorld();

	m_projectionMatrix_inv = XMMatrixInverse(nullptr, m_projectionMatrix);

	for (auto actor : world->GetAllActors())
	{
		if (!actor->GetIsRendered())
		{
			continue;
		}

		XMMATRIX worldMatrix = actor->transformation;
		worldMatrix = XMMatrixTranspose(worldMatrix);

		UINT indexCount = actor->geo->indexData.size();
		UINT startIndexLocation = 0;
		INT startVertexLocation = actor->geo->vertexOffset;

		ShaderManager::VSConstBuffer VSbuffer;
		VSbuffer.world = worldMatrix;
		VSbuffer.view = m_viewMatrix;
		VSbuffer.projection = m_projectionMatrix;
		VSbuffer.color = actor->actorRenderData.color;
		m_shaderManager->SetVSConstBuffer(&VSbuffer);

		ShaderManager::PSConstBuffer PSBuffer;
		PSBuffer.index = max(m_shaderManager->GetTextureIndex(actor->geo->texFileName), 0);
		PSBuffer.uvOffset = actor->geo->uvOffset;
		m_shaderManager->SetPSConstBuffer(&PSBuffer);

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
