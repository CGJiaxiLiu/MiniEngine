#include <Windows.h>  
#include <stdio.h>  
#include <string.h>  
#include <iostream>  

using namespace std;

int main(int argc, char* argv[])
{
	//���ݴ�������ȡQQ��Ϸ��¼���ھ��  
	HWND hq = FindWindow(NULL, "MyWidget");
	if (hq) {
		//�õ�QQ���ڴ�С  
		RECT rect;
		GetWindowRect(hq, &rect);
		int w = rect.right - rect.left, h = rect.bottom - rect.top;
		cout << "��:" << w << " " << "��:" << h << endl;

		//�ƶ�QQ����λ��  
		MoveWindow(hq, 100, 100, w, h, false);

		//�õ����洰��  
		HWND hd = GetDesktopWindow();
		GetWindowRect(hd, &rect);
		w = rect.right - rect.left;
		h = rect.bottom - rect.top;
		cout << "��:" << w << " " << "��:" << h << endl;

	}
	else {
		printf("Cannot Find Window");
	}



	system("pause");

	return 0;
}