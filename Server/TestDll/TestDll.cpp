// TestDll.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
int main(int argc, char* argv[])
{
	printf("Hello World!\n");
	char strHost[]="127.0.0.1";          //�������ߵ�ַ
	int  nPort=8088;                     //�������߶˿�
	//��������dll
    HMODULE hServerDll=LoadLibrary(".\\..\\..\\bin\\server\\server.dll");
	//����������������--�鿴��һ�ڵ�����TestRun����
    typedef void (_cdecl *TestRunT)(char* strHost,int nPort );
	//Ѱ��dll�е�������
    TestRunT pTestRunT=(TestRunT)GetProcAddress(hServerDll,"TestRun");
	//�жϺ����Ƿ�Ϊ��
	if (pTestRunT!=NULL)
	{
		pTestRunT(strHost,nPort);   //�����������
	}

	return 0;
}

