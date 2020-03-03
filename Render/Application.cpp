#include <stdarg.h>
#include <string>
#include <thread>
#include "util.h"
#include "Application.h"
#include "Viewport.h"
#include "GraphicsManager.h"
#include "World.h"

Application::Application()
{
}

void Application::Initialize()
{
	instance = this;
	CreateMainViewport();

	m_frameLength = 1000.0 / (float)m_maxFPS;
	timePoint_a = std::chrono::system_clock::now();

#ifdef _DEBUG
	if (!m_consoleHandle) {
		AllocConsole();
		m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	}
#endif // !DEBUG


	m_world = std::make_shared<World>();
	m_world->Initialize(this);

	bool result;
	m_graphicsManager = std::make_shared<GraphicsManager>();
	result = m_graphicsManager->Initialize(this, this->m_mainViewport);

	if (!result) {
		BOX(L"Graphics Manager Initialize Fail", L"Error");
		D3DErrorParse(result);
		m_mainViewport->CalledToDestroy();
	}


}

void Application::MainLoop()
{
	while (!m_mainViewport->GetIsCalledToDestroy()) {
		m_frameCount++;
		timePoint_b = std::chrono::system_clock::now();
		std::chrono::duration<double, std::milli> work_time = timePoint_b - timePoint_a;
		if (work_time.count() < m_frameLength) {
			std::chrono::duration<double, std::milli> sleep_time(m_frameLength - work_time.count());
			auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(sleep_time);
			std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
		}
		timePoint_a = std::chrono::system_clock::now();
		std::chrono::duration<double, std::milli> sleep_time = timePoint_a - timePoint_b;
		m_runningTime += (work_time + sleep_time).count() / 1000.0;
		Tick((work_time + sleep_time).count()/1000.0);
	}
}

void Application::Destroy()
{
	instance = nullptr;

	if (m_mainViewport) {
		m_mainViewport->Destroy();
	}

	if (m_graphicsManager) {
		m_graphicsManager->Destroy();
	}

	if (m_world)
	{
		m_world->Destroy();
	}
}

void Application::Tick(double DeltaTime)
{
	m_mainViewport->Execute(DeltaTime);
	m_world->Tick(DeltaTime);
	m_graphicsManager->Render();
	//LOG(L"Frame: %d", this->m_frameCount);
}

POINT Application::GetCursorPositionPixel()
{
	POINT p;
	if (GetCursorPos(&p))
	{
		ScreenToClient(this->m_mainViewport->GetWindowHandle(), &p);
	}
	return p;
}

DirectX::XMFLOAT2 Application::GetCursorPosition()
{
	POINT p = this->GetCursorPositionPixel();
	return DirectX::XMFLOAT2(2 * ((float)p.x / m_screenWidth - 0.5f), - 2 * ((float)p.y / m_screenHeight - 0.5));
}

void Application::CreateMainViewport()
{
	m_mainViewport = std::make_shared<Viewport>();

	m_mainViewport->screenHeight = m_screenHeight;
	m_mainViewport->screenWidth = m_screenWidth;
	m_mainViewport->Initialize();
}

void Application::LogOnConsole(const wchar_t * text, ...)
{
	wchar_t buffer[256];
	va_list args;
	va_start(args, text);
	vswprintf(buffer, 256, text, args);
	//wvsprintf(buffer, text, args);
	va_end(args);

	//if (!m_consoleHandle) {
	//	AllocConsole();
	//	m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	//}

	if (m_consoleHandle)
	{
		WriteConsole(m_consoleHandle, buffer, wcslen(buffer), NULL, NULL);
		WriteConsole(m_consoleHandle, L"\n", 1, NULL, NULL);
	}

}

Application * GetInstance()
{
	return instance; 
}
