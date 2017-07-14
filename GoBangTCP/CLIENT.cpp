// CLIENT.cpp : 实现文件
//
#pragma once

#include "stdafx.h"
#include "GoBangTCP.h"
#include "GoBangTCPDlg.h"
#include "CLIENT.h"
#include "afxdialogex.h"

extern CGoBangTCPDlg *Main;
// CCLIENT 对话框

IMPLEMENT_DYNAMIC(CCLIENT, CDialogEx)

CCLIENT::CCLIENT(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCLIENT::IDD, pParent)
{

}

CCLIENT::~CCLIENT()
{
}

void CCLIENT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCLIENT, CDialogEx)
	ON_BN_CLICKED(IDC_CONNECT, &CCLIENT::OnBnClickedConnect)
END_MESSAGE_MAP()


// CCLIENT 消息处理程序


void CCLIENT::OnBnClickedConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString IPaddress, Port;
	GetDlgItem(IDC_ADDR)->GetWindowTextW(IPaddress);
	GetDlgItem(IDC_PORT)->GetWindowTextW(Port);
	if (IPaddress == L"" || Port == L"")
	{
		MessageBox(L"服务器或端口不能为空！");
	}
	else
	{
		USES_CONVERSION;
		char *IPC = T2A(IPaddress), *PortC = T2A(Port);
		int tPort = atoi(PortC);
		Main->Msock = ::socket(AF_INET, SOCK_STREAM, 0);
		::WSAAsyncSelect(Main->Msock, Main->m_hWnd, WM_SOCKETC, FD_READ);
		Main->addrM.sin_family = AF_INET;
		Main->addrM.sin_port = htons(tPort);
		Main->addrM.sin_addr.S_un.S_addr = inet_addr(IPC);
		::connect(Main->Msock, (sockaddr*)&(Main->addrM), sizeof(Main->addrM));

		fd_set wfd;
		struct timeval timeout;
		FD_ZERO(&wfd);
		timeout.tv_sec = 10;
		timeout.tv_usec=0;
		FD_SET(Main->Msock, &wfd);
		int Res = 1;
		Res = select(1, NULL, &wfd, NULL, &timeout);
		if (Res == 0 || Res == SOCKET_ERROR)
		{
			CString temp;
			int n = WSAGetLastError();
			temp.Format(L"连接失败！请重试！错误%d", n);
			MessageBox(temp);
		}
		else
		{
			MessageBox(L"连接成功！");
			CDialogEx::OnOK();
		}
	}
}
