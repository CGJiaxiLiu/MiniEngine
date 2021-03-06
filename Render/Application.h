#pragma once
#include <windows.h>
#include <memory>
#include <chrono>
#include <DirectXMath.h>
#include <vector>

class Application
{
public:
	Application();
	void Initialize();
	void MainLoop();
	void Destroy();
	void Tick(double DeltaTime);

	void SetMaxFPS(unsigned int maxFPS) { m_maxFPS = maxFPS; }
	void SetViewportSize(unsigned int inWidth, unsigned int inHeight) { m_screenWidth = inWidth; m_screenHeight = inHeight; }
	void LogOnConsole(const wchar_t* text, ...);
	float GetRunningTime() { return m_runningTime; }
	bool m_shouldQuit = false;
	std::shared_ptr<class Viewport> m_mainViewport = 0;
	std::shared_ptr<class World> GetWorld() { return m_world; }
	unsigned int GetScreenWidth() { return m_screenWidth; }
	unsigned int GetScreenHeight() { return m_screenHeight; }
	float GetHorizontalRatio() { return screenRatio / m_screenWidth; }
	float GetVerticalRatio() { return screenRatio / m_screenHeight; }
	std::shared_ptr<class GraphicsManager> GetGraphicsManager() { return m_graphicsManager; }
	POINT GetCursorPositionPixel();
	DirectX::XMFLOAT2 GetCursorPosition();
	inline size_t GetFrameCount() { return this->m_frameCount; }
	bool GetKeyDown(WPARAM index);
	bool GetKeyUp(WPARAM index);
	bool GetKeyPressed(WPARAM index);

	unsigned char * LoadImageData(const WCHAR* filename, UINT& width, UINT& height);
	inline UINT GetAtlasSize() { return this->m_atlasSize; }

protected:
	void CreateMainViewport();

	float m_runningTime = 0;
	unsigned int m_maxFPS = 100;
	double m_frameLength = 0.0;
	unsigned int m_screenWidth = 800;
	unsigned int m_screenHeight = 600;
	size_t m_frameCount = 0;
	float screenRatio = 100;
	unsigned int m_atlasSize = 1024;

	std::chrono::system_clock::time_point timePoint_a;
	std::chrono::system_clock::time_point timePoint_b;

	HANDLE m_consoleHandle = 0;
	std::shared_ptr<class GraphicsManager> m_graphicsManager = 0;
	std::shared_ptr<class World> m_world = 0;

	ULONG_PTR m_gdiplusToken;
};

static Application* instance = 0;
Application* GetAppInstance();
