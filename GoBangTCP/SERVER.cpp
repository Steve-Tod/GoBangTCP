// SERVER.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GoBangTCP.h"
#include "GoBangTCPDlg.h"
#include "SERVER.h"
#include "afxdialogex.h"

extern CGoBangTCPDlg *Main;
// CSERVER �Ի���

IMPLEMENT_DYNAMIC(CSERVER, CDialogEx)

CSERVER::CSERVER(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSERVER::IDD, pParent)
{

}

CSERVER::~CSERVER()
{
}

void CSERVER::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSERVER, CDialogEx)
	ON_BN_CLICKED(IDC_LISTEN, &CSERVER::OnBnClickedListen)
END_MESSAGE_MAP()


// CSERVER ��Ϣ�������


void CSERVER::OnBnClickedListen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString Port;
	GetDlgItem(IDC_SERVERPORT)->GetWindowTextW(Port);
	if (Port == L"")
	{
		MessageBox(L"�˿ڲ���Ϊ�գ�");
	}
	else
	{
		USES_CONVERSION;
		char *PortC = T2A(Port);
		int n = atoi(PortC);
		Main->addrM.sin_family = AF_INET;
		Main->addrM.sin_port = htons(n);
		Main->addrM.sin_addr.S_un.S_addr = INADDR_ANY;
		Main->Msock = ::socket(AF_INET, SOCK_STREAM, 0);
		if (::bind(Main->Msock, (sockaddr*)&(Main->addrM), sizeof(Main->addrM)) == SOCKET_ERROR)
		{
			CString temp;
			int n = WSAGetLastError();
			temp.Format(L"�󶨵�ַʧ�ܣ����������%d", n);
			MessageBox(temp);
		}
		else
		{
			::listen(Main->Msock, 1);
			::WSAAsyncSelect(Main->Msock, Main->m_hWnd, WM_SOCKETS, FD_ACCEPT | FD_READ);
			CDialogEx::OnOK();
		}
	}
}
