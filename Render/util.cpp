#include "util.h"
#include "Application.h"
#include <stdarg.h>
#include <windows.h>
#include "Viewport.h"

//extern Application * instance;

void _Log(const wchar_t * Format, ...)
{
	auto instance = GetInstance();
	if (instance) {
		//va_list args;
		//va_start(args, Format);
		//instance->LogOnConsole(Format, args);
		//va_end(args);

		wchar_t buffer[256];
		va_list args;
		va_start(args, Format);
		vswprintf(buffer, 256, Format, args);
		instance->LogOnConsole(buffer);
		va_end(args);
	}
}

float _RunTime()
{
	auto instance = GetInstance();
	if (instance) {
		return instance->GetRunningTime();
	}
	else {
		return 0.0f;
	}
}

void _AriseMessageBox(const wchar_t * Content, const wchar_t * Title)
{
	auto instance = GetInstance();
	if (instance) {
		if (instance->m_mainViewport) {
			MessageBox(instance->m_mainViewport->GetWindowHandle(), Content, Title, MB_OK);
		}
	}
}