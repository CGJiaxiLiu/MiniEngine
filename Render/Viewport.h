#pragma once
#include <windows.h>
#include <vector>

class Viewport
{
public:
	Viewport();
	void Initialize();
	void Execute(double DeltaTime);
	void Destroy();

	HWND GetWindowHandle() { return m_hwnd; }
	void CalledToDestroy() { m_calledToDestroy = true; }
	bool GetIsCalledToDestroy() { return m_calledToDestroy; }

	static LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	static std::vector<Viewport*> ViewportPtrVec;

	unsigned int screenWidth = 800;
	unsigned int screenHeight = 600;

protected:
	void InitializeWindow(unsigned int inWidth, unsigned int inHeight);
	void ShutdownWindow();
	void Tick(double DeltaTime);

	bool m_calledToDestroy = false;
	LPCWSTR m_applicationName = NULL;
	HINSTANCE m_hinstance = NULL;
	HWND m_hwnd = NULL;
	MSG m_msg;
};


