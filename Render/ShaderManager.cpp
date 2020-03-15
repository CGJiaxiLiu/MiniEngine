#include "ShaderManager.h"
#include "util.h"
#include "World.h"
#include "Application.h"
#include "Actor.h"

ShaderManager::ShaderManager()
{
}

bool ShaderManager::Initialize(class Application* inApp, ID3D11Device * device, ID3D11DeviceContext* context, HWND hwnd)
{
	bool result;
	this->m_context = context;
	this->app = inApp;
	this->texIndexMap = std::unordered_map<const WCHAR*, UINT>();
	this->texNameList = std::vector<const WCHAR*>();

	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"color.vs", L"color.ps");
	if (!result)
	{
		return false;
	}

	result = LoadResource(device, this->app->GetWorld());
	if (!result)
	{
		return false;
	}

	return true;
}

void ShaderManager::Destroy()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	DestroyShader();
}

bool ShaderManager::SetVSConstBuffer(VSConstBuffer* inData)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VSConstBuffer* dataPtr;
	unsigned int bufferNumber;

	// Lock the constant buffer so it can be written to.
	result = this->m_context->Map(m_VSConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (VSConstBuffer*)mappedResource.pData;
	memcpy(dataPtr, inData, sizeof(VSConstBuffer));

	// Unlock the constant buffer.
	this->m_context->Unmap(m_VSConstBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	this->m_context->VSSetConstantBuffers(bufferNumber, 1, &m_VSConstBuffer);

	return true;
}

bool ShaderManager::SetPSConstBuffer(PSConstBuffer * inData)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	PSConstBuffer* dataPtr;
	unsigned int bufferNumber;

	result = this->m_context->Map(m_PSConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (PSConstBuffer*)mappedResource.pData;
	memcpy(dataPtr, inData, sizeof(PSConstBuffer));

	this->m_context->Unmap(m_PSConstBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	this->m_context->PSSetConstantBuffers(bufferNumber, 1, &m_PSConstBuffer);

	return true;
}

bool ShaderManager::Render(UINT indexCount, UINT startIndex, INT startVertex)
{
	// Now render the prepared buffers with the shader.
	Draw(indexCount, startIndex, startVertex);

	return true;
}

int ShaderManager::GetTextureIndex(const WCHAR * name)
{
	if (texIndexMap.count(name) == 1)
	{
		return texIndexMap[name];
	}
	else
	{
		return -1;
	}
}

bool ShaderManager::InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC VSBufferDesc;
	D3D11_BUFFER_DESC PSBufferDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was  nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		BOX(D3DErrorParse(result), L"Shader Manager Error 0");
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		BOX(D3DErrorParse(result), L"Shader Manager Error 1");
		return false;
	}

	// Now setup the layout of the data that goes into the shader.
// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);

	if (FAILED(result))
	{
		BOX(D3DErrorParse(result), L"Shader Manager Error 2");
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	VSBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VSBufferDesc.ByteWidth = sizeof(VSConstBuffer);
	VSBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	VSBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VSBufferDesc.MiscFlags = 0;
	VSBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&VSBufferDesc, NULL, &m_VSConstBuffer);
	if (FAILED(result))
	{
		BOX(D3DErrorParse(result), L"Shader Manager Error 3");
		return false;
	}

	PSBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	PSBufferDesc.ByteWidth = sizeof(PSConstBuffer);
	PSBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	PSBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	PSBufferDesc.MiscFlags = 0;
	PSBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&PSBufferDesc, NULL, &m_PSConstBuffer);
	if (FAILED(result))
	{
		BOX(D3DErrorParse(result), L"Shader Manager Error 4");
		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}


	LOG(L"Shader Compile Success");
	return true;
}

bool ShaderManager::LoadResource(ID3D11Device * device, std::shared_ptr<class World> world)
{
	int texCount = 0;

	for (auto geo : world->GetGeometryList())
	{
		const WCHAR* imgName = geo->texFileName;

		if (!imgName || texIndexMap.count(imgName) == 1)
		{
			continue;
		}

		texIndexMap[imgName] = texCount;
		texNameList.push_back(imgName);
		LOG(L"Find Tex %d", texCount);

		texCount++;

	}
	
	if (texCount == 0)
	{
		return true;
	}

	D3D11_TEXTURE2D_DESC sTexDesc;
	sTexDesc.Width = app->GetAtlasSize();
	sTexDesc.Height = app->GetAtlasSize();
	sTexDesc.MipLevels = 1;
	sTexDesc.ArraySize = texCount;
	sTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sTexDesc.SampleDesc.Count = 1;
	sTexDesc.SampleDesc.Quality = 0;
	sTexDesc.Usage = D3D11_USAGE_IMMUTABLE;
	sTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sTexDesc.CPUAccessFlags = 0;
	sTexDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA *sSubData = new D3D11_SUBRESOURCE_DATA[texCount];
	//std::vector<std::vector<unsigned char>> dataList = std::vector<std::vector<unsigned char>>();

	for (int i = 0; i < texCount; i++) {

		UINT width, height;

		auto buffer_ = app->LoadImageData(texNameList[i], width, height);
		//dataList.push_back(buffer_);

		if (width != app->GetAtlasSize() || height != app->GetAtlasSize())
		{
			BOX(L"Texture Size Incorret", L"Shader Manager Load Resource ERROR 0");
			return false;
		}
		LOG(L"Load Tex %d", i);

		sSubData[i].pSysMem = buffer_;
		sSubData[i].SysMemPitch = (UINT)(app->GetAtlasSize() * 4);
		sSubData[i].SysMemSlicePitch = (UINT)(app->GetAtlasSize() * app->GetAtlasSize() * 4);
	}

	ID3D11Texture2D* pTexture;

	LOG(L"Before Texture2d Create %d", texCount);

	auto result = device->CreateTexture2D(&sTexDesc, sSubData, &pTexture);
	delete[] sSubData;

	if (FAILED(result))
	{
		BOX(D3DErrorParse(result), L"Shader Manager Load Resource ERROR 2");
		return false;
	}

	LOG(L"Create Texture2d Success");

	D3D11_SHADER_RESOURCE_VIEW_DESC  SRVDesc;
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2DARRAY;
	SRVDesc.Texture2DArray.ArraySize = texCount;
	SRVDesc.Texture2DArray.FirstArraySlice = 0;
	SRVDesc.Texture2DArray.MipLevels = 1;
	SRVDesc.Texture2DArray.MostDetailedMip = 0;

	result = device->CreateShaderResourceView(pTexture, &SRVDesc, &m_SRV);

	if (FAILED(result))
	{
		BOX(D3DErrorParse(result), L"Shader Manager Load Resource ERROR 2");
		return false;
	}

	this->m_context->PSSetShaderResources(0, 1, &m_SRV);

	return true;
}

void ShaderManager::DestroyShader()
{
	// Release the matrix constant buffer.
	if (m_VSConstBuffer)
	{
		m_VSConstBuffer->Release();
		m_VSConstBuffer = 0;
	}

	if (m_PSConstBuffer)
	{
		m_PSConstBuffer->Release();
		m_PSConstBuffer = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}
}

void ShaderManager::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
}

void ShaderManager::Draw(UINT indexCount, UINT startIndex, INT startVertex)
{
	// Set the vertex input layout.
	this->m_context->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	this->m_context->VSSetShader(m_vertexShader, NULL, 0);
	this->m_context->PSSetShader(m_pixelShader, NULL, 0);
	this->m_context->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	this->m_context->DrawIndexed(indexCount, startIndex, startVertex);
	return;
}

//bool ShaderManager::SetShaderParameters()
//{
	//HRESULT result;
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//VSConstBuffer* dataPtr;
	//PSConstBuffer* dataPtr2;
	//unsigned int bufferNumber;

	//// Transpose the matrices to prepare them for the shader.
	//D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	//D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	//D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	//// Lock the constant buffer so it can be written to.
	//result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	//// Get a pointer to the data in the constant buffer.
	//dataPtr = (MatrixBufferType*)mappedResource.pData;

	//// Copy the matrices into the constant buffer.
	//dataPtr->world = worldMatrix;
	//dataPtr->view = viewMatrix;
	//dataPtr->projection = projectionMatrix;

	//// Unlock the constant buffer.
	//deviceContext->Unmap(m_matrixBuffer, 0);

	//// Set the position of the constant buffer in the vertex shader.
	//bufferNumber = 0;

	//// Finanly set the constant buffer in the vertex shader with the updated values.
	//deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	//result = deviceContext->Map(m_ParaBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	//dataPtr2 = (ParaBufferType*)mappedResource.pData;
	//dataPtr2->intensity = sin(RUNNING_TIME);
	//this->m_context->Unmap(m_ParaBuffer, 0);

	//// Set the position of the light constant buffer in the pixel shader.
	//bufferNumber = 0;

	//// Finally set the light constant buffer in the pixel shader with the updated values.
	//this->m_context->PSSetConstantBuffers(bufferNumber, 1, &m_ParaBuffer);

//	return true;
//}
