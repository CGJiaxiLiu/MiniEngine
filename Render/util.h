#pragma once
#include <d3dcommon.h>
#include <Dxerr.h>
#include <math.h>

#define LOG(Format, ...) _Log(Format, ##__VA_ARGS__);
#define RUNNING_TIME _RunTime()
#define BOX(a, b) _AriseMessageBox(a, b);
#define D3DErrorParse(result) DXGetErrorDescription(result)
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}

void _Log(const wchar_t* Format, ...);
float _RunTime();
void _AriseMessageBox(const wchar_t* Content, const wchar_t* Title);