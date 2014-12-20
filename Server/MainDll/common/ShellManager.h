// ShellManager.h: interface for the CShellManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHELLMANAGER_H__8E71A611_99CA_468E_9F7A_1E9B106C44AB__INCLUDED_)
#define AFX_SHELLMANAGER_H__8E71A611_99CA_468E_9F7A_1E9B106C44AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Manager.h"

class CShellManager : public CManager  
{
public:
	CShellManager(CClientSocket *pClient);
	virtual ~CShellManager();

private:
    HANDLE m_hReadPipeHandle;   //lang4.2   �ǲ��Ǻ���Ϥ����  ��һ�ڶ���ı��������ﶼ�а�   ���� ��ʵ�Ҿ��Ƿ���������
    HANDLE m_hWritePipeHandle;   //��Щ���ǹܵ��ľ����
	HANDLE m_hReadPipeShell;
    HANDLE m_hWritePipeShell;
	
    HANDLE m_hProcessHandle;    //��Щ�½��̺��̵߳ľ���˰�    ת����Ĺ��캯��������
	HANDLE m_hThreadHandle;
    HANDLE m_hThreadRead;
	HANDLE m_hThreadMonitor;

protected:
	void OnReceive(LPBYTE lpBuffer, UINT nSize);

	 static DWORD WINAPI ReadPipeThread(LPVOID lparam); 
	 static DWORD WINAPI MonitorThread(LPVOID lparam);
};

#endif // !defined(AFX_SHELLMANAGER_H__8E71A611_99CA_468E_9F7A_1E9B106C44AB__INCLUDED_)
