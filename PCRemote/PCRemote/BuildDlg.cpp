// BuildDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PCRemote.h"
#include "BuildDlg.h"
#include "afxdialogex.h"


// CBuildDlg �Ի���

IMPLEMENT_DYNAMIC(CBuildDlg, CDialogEx)

struct Connect_Address
{
	DWORD dwstact;
	char  strIP[MAX_PATH];
	int   nPort;
}g_myAddress={0x1234567,"",0};

CBuildDlg::CBuildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBuildDlg::IDD, pParent)
	, m_strIP(_T(""))
	, m_strPort(_T(""))
{

}

CBuildDlg::~CBuildDlg()
{
}

void CBuildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strIP);
	DDX_Text(pDX, IDC_EDIT2, m_strPort);
}


BEGIN_MESSAGE_MAP(CBuildDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBuildDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBuildDlg ��Ϣ�������

int memfind(const char *mem, const char *str, int sizem, int sizes)   
{   
	int   da,i,j;   
	if (sizes == 0) da = strlen(str);   
	else da = sizes;   
	for (i = 0; i < sizem; i++)   
	{   
		for (j = 0; j < da; j ++)   
			if (mem[i+j] != str[j])	break;   
		if (j == da) return i;   
	}   
	return -1;   
}

void CBuildDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFile file;
	char strTemp[MAX_PATH];
	ZeroMemory(strTemp,MAX_PATH);
	CString strCurrentPath;
	CString strFile;
	CString strSeverFile;
	BYTE *lpBuffer=NULL;
	DWORD dwFileSize;
	UpdateData(TRUE);
	//////////������Ϣ//////////////////////
	strcpy(g_myAddress.strIP,m_strIP);
	g_myAddress.nPort=atoi(m_strPort);
	try
	{
		//�˴��õ�δ����ǰ���ļ���
		GetModuleFileName(NULL,strTemp,MAX_PATH);     //�õ��ļ���  
		strCurrentPath=strTemp;
		int nPos=strCurrentPath.ReverseFind('\\');     
		strCurrentPath=strCurrentPath.Left(nPos);
		strFile=strCurrentPath+"\\server\\Loder.exe";   //�õ���ǰδ�����ļ���
		//���ļ�
		file.Open(strFile,CFile::modeRead|CFile::typeBinary);
		dwFileSize=file.GetLength();
		lpBuffer=new BYTE[dwFileSize];
		ZeroMemory(lpBuffer,dwFileSize);
		//��ȡ�ļ�����
		file.Read(lpBuffer,dwFileSize);
		file.Close();
		//д������IP�Ͷ˿� ��Ҫ��Ѱ��0x1234567�����ʶȻ��д�����λ��
		int nOffset=memfind((char*)lpBuffer,(char*)&g_myAddress.dwstact,dwFileSize,sizeof(DWORD));
		memcpy(lpBuffer+nOffset,&g_myAddress,sizeof(Connect_Address));
		//���浽�ļ�
		strSeverFile=strCurrentPath+"\\server.exe";
		file.Open(strSeverFile,CFile::typeBinary|CFile::modeCreate|CFile::modeWrite);
		file.Write(lpBuffer,dwFileSize);
		file.Close();
		delete[] lpBuffer;
		MessageBox("���ɳɹ�");

	}
	catch (CMemoryException* e)
	{
		MessageBox("�ڴ治��");
	}
	catch (CFileException* e)
	{
		MessageBox("�ļ���������");
	}
	catch (CException* e)
	{
		MessageBox("δ֪����");
	}
	CDialogEx::OnOK();

}
