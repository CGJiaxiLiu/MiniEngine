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

	inline bool GetKeyDown(WPARAM index) { return index < keyCount ? m_keyDown[index] : false; }
	inline bool GetKeyUp(WPARAM index) { return index < keyCount ? m_keyUp[index] : false; }
	inline bool GetKeyPressed(WPARAM index) { return index < keyCount ? m_keyPressed[index] : false; }
	void resetKetAction();


protected:
	void InitializeWindow(unsigned int inWidth, unsigned int inHeight);
	void ShutdownWindow();
	void Tick(double DeltaTime);

	size_t keyCount = 0x5B;
	bool m_keyDown[0x5B] = { false };
	bool m_keyPressed[0x5B] = { false };
	bool m_keyUp[0x5B] = { false };

	bool m_calledToDestroy = false;
	LPCWSTR m_applicationName = NULL;
	HINSTANCE m_hinstance = NULL;
	HWND m_hwnd = NULL;
	MSG m_msg;
};


