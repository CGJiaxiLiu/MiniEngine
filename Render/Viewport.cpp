#include "Viewport.h"
#include "util.h"
#include <stdarg.h>
#include <string>
#include <windows.h>
#include "Application.h"

std::vector<Viewport*> Viewport::ViewportPtrVec = std::vector<Viewport*>();

Viewport::Viewport()
{
}

void Viewport::Initialize()
{
	ViewportPtrVec = std::vector<Viewport*>();

	// Initialize window
	InitializeWindow(screenWidth, screenHeight);
	ViewportPtrVec.push_back(this);
}

void Viewport::Execute(double DeltaTime)
{
	// Initialize the message structure.
	ZeroMemory(&m_msg, sizeof(MSG));

	// Handle the windows messages.
	if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	}

	// If windows signals to end the application then exit out.
	if (m_msg.message == WM_QUIT)
	{
		this->m_calledToDestroy = true;
	}

	if (this->m_calledToDestroy) {
		MessageBox(m_hwnd, L"Close", L"OnEscape", MB_OK);
		return;
	}

	//this->Tick(DeltaTime);
}

void Viewport::Tick(double DeltaTime)
{
}

void Viewport::Destroy()
{
	ShutdownWindow();
}

LRESULT Viewport::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	for (auto viewport : ViewportPtrVec) {
		if (hwnd == viewport->GetWindowHandle()) {
			if (umsg == WM_DESTROY || umsg == WM_CLOSE) {
				PostQuitMessage(0);
				return 0;
			}
			else if (umsg == WM_KEYDOWN) {
				if (wparam == VK_ESCAPE) {
					viewport->CalledToDestroy();
					return 0;
				}
			}
		}
	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void Viewport::InitializeWindow(unsigned int inWidth, unsigned int inHeight)
{
	WNDCLASSEX wc;
	int posX, posY;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Render";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = this->MessageHandler;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Place the window in the middle of the screen.
	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);
}

void Viewport::ShutdownWindow()
{
	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;
}
