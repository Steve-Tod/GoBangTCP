// CHOOSEMODE.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GoBangTCP.h"
#include "CHOOSEMODE.h"
#include "afxdialogex.h"
#include "CLIENT.h"
#include "SERVER.h"

extern BOOL IsConnect;
extern BOOL IsServer;
// CCHOOSEMODE �Ի���

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


// CCHOOSEMODE ��Ϣ�������


BOOL CCHOOSEMODE::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_Choice.InsertString(0, L"������Ϸ");
	m_Choice.InsertString(1, L"˫����Ϸ���ͻ��ˣ�");
	m_Choice.InsertString(1, L"˫����Ϸ����������");
	return TRUE;
}


void CCHOOSEMODE::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString s;
	GetDlgItem(IDC_CHOICE)->GetWindowTextW(s);
	if (s == "˫����Ϸ���ͻ��ˣ�")
	{
		CCLIENT Client;
		if (IDOK == Client.DoModal())//�������OK�����������״̬
		{
			IsConnect = TRUE;
			IsServer = FALSE;
			CDialogEx::OnOK();
		}
	}
	else if (s == "˫����Ϸ����������")
	{
		CSERVER Server;
		if (IDOK == Server.DoModal())//�������OK�����������״̬
		{
			IsConnect = TRUE;
			IsServer = TRUE;
			CDialogEx::OnOK();
		}
	}
	else if (s == "������Ϸ")
	{
		IsConnect = FALSE;
		CDialogEx::OnOK();
	}
}
