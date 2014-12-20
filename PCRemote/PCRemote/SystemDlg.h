#pragma once
#include "afxcmn.h"
#include "include/IOCPServer.h"

// CSystemDlg �Ի���

class CSystemDlg : public CDialog
{
	DECLARE_DYNAMIC(CSystemDlg)

public:
	CSystemDlg(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext *pContext = NULL);   // ��׼���캯��
	virtual ~CSystemDlg();

// �Ի�������
	enum { IDD = IDD_SYSTEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_process;
	//CListCtrl m_list_windows;
private:
	HICON m_hIcon;
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	 BOOL m_bHow;     //�������ִ��ڹ���ͽ��̹���
public:
	void AdjustList(void);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
private:
	void GetProcessList(void);
public:
	virtual BOOL OnInitDialog();
	void ShowProcessList(void);
	afx_msg void OnKillprocess();
	afx_msg void OnRefreshpslist();
	afx_msg void OnNMRClickListProcess(NMHDR *pNMHDR, LRESULT *pResult);
	void OnReceiveComplete(void);
	void ShowWindowsList(void);
	afx_msg void OnWindowClost();
	afx_msg void OnWindowHide();
	afx_msg void OnWindowMax();
	afx_msg void OnWindowMin();
	afx_msg void OnWindowReflush();
	void GetWindowsList(void);
	afx_msg void OnWindowReturn();
};
