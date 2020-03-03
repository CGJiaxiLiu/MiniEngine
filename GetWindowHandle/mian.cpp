#include <Windows.h>  
#include <stdio.h>  
#include <string.h>  
#include <iostream>  

using namespace std;

int main(int argc, char* argv[])
{
	//根据窗口名获取QQ游戏登录窗口句柄  
	HWND hq = FindWindow(NULL, "MyWidget");
	if (hq) {
		//得到QQ窗口大小  
		RECT rect;
		GetWindowRect(hq, &rect);
		int w = rect.right - rect.left, h = rect.bottom - rect.top;
		cout << "宽:" << w << " " << "高:" << h << endl;

		//移动QQ窗口位置  
		MoveWindow(hq, 100, 100, w, h, false);

		//得到桌面窗口  
		HWND hd = GetDesktopWindow();
		GetWindowRect(hd, &rect);
		w = rect.right - rect.left;
		h = rect.bottom - rect.top;
		cout << "宽:" << w << " " << "高:" << h << endl;

	}
	else {
		printf("Cannot Find Window");
	}



	system("pause");

	return 0;
}