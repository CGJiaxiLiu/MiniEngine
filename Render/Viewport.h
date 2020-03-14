#pragma once
#include <windows.h>
#include <vector>
#include <list>

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

	inline bool GetKeyDown(WPARAM index) { return index < keyCount && m_keyDown[index][stagedBufferIndex]; }
	inline bool GetKeyUp(WPARAM index) { return index < keyCount && m_keyUp[index][stagedBufferIndex]; }
	inline bool GetKeyPressed(WPARAM index) { return HIBYTE(GetKeyState(index)); }
	void resetKetAction(int index);
	void lockRead() { readLocked = true; }
	void unlockRead();
	
protected:
	void InitializeWindow(unsigned int inWidth, unsigned int inHeight);
	void ShutdownWindow();
	void Tick(double DeltaTime);

	bool readLocked = false;
	bool needSwapBuffer = false;
	int stagedBufferIndex = 0;
	size_t keyCount = 0x5B;
	bool m_keyDown[0x5B][2] = { false };
	bool m_keyPressedLatframe[0x5B][2] = { false };
	bool m_keyUp[0x5B][2] = { false };

	//std::list<WPARAM>keyDownEventQueue[2];
	//std::list<WPARAM>keyUpEventQueue[2];

	bool m_calledToDestroy = false;
	LPCWSTR m_applicationName = NULL;
	HINSTANCE m_hinstance = NULL;
	HWND m_hwnd = NULL;
	MSG m_msg;
};


