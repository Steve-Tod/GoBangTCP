// CHOOSEMODE.cpp : 实现文件
//

#include "stdafx.h"
#include "GoBangTCP.h"
#include "CHOOSEMODE.h"
#include "afxdialogex.h"
#include "CLIENT.h"
#include "SERVER.h"

extern BOOL IsConnect;
extern BOOL IsServer;
// CCHOOSEMODE 对话框

IMPLEMENT_DYNAMIC(CCHOOSEMODE, CDialogEx)

CCHOOSEMODE::CCHOOSEMODE(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCHOOSEMODE::IDD, pParent)
{

}

CCHOOSEMODE::~CCHOOSEMODE()
{
}

void CCHOOSEMODE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHOICE, m_Choice);
}


BEGIN_MESSAGE_MAP(CCHOOSEMODE, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCHOOSEMODE::OnBnClickedOk)
END_MESSAGE_MAP()


// CCHOOSEMODE 消息处理程序


BOOL CCHOOSEMODE::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_Choice.InsertString(0, L"本地游戏");
	m_Choice.InsertString(1, L"双人游戏（客户端）");
	m_Choice.InsertString(1, L"双人游戏（服务器）");
	return TRUE;
}


void CCHOOSEMODE::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s;
	GetDlgItem(IDC_CHOICE)->GetWindowTextW(s);
	if (s == "双人游戏（客户端）")
	{
		CCLIENT Client;
		if (IDOK == Client.DoModal())//如果返回OK，则进入连接状态
		{
			IsConnect = TRUE;
			IsServer = FALSE;
			CDialogEx::OnOK();
		}
	}
	else if (s == "双人游戏（服务器）")
	{
		CSERVER Server;
		if (IDOK == Server.DoModal())//如果返回OK，则进入连接状态
		{
			IsConnect = TRUE;
			IsServer = TRUE;
			CDialogEx::OnOK();
		}
	}
	else if (s == "本地游戏")
	{
		IsConnect = FALSE;
		CDialogEx::OnOK();
	}
}
