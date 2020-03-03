#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include "Application.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	auto app = std::make_shared<Application>();

	app->SetMaxFPS(144);

	app->Initialize();
	app->MainLoop();
	app->Destroy();

	return 0;
}