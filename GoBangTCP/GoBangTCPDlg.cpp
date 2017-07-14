
// GoBangTCPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GoBangTCP.h"
#include "GoBangTCPDlg.h"
#include "afxdialogex.h"
#include "ChessBoard.h"
#include "GameControl.h"
#include "CHOOSEMODE.h"
#include "SockMessage.h"
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma warning(disable: 4996)

int ClientX, ClientY;//��ȡ�ͻ�����С
CChessBoard *Board;
GameControl Control;
CString GameDoc;
BOOL IsOver = FALSE;
BOOL IsNew = TRUE;//�Ƿ�����Ϊ�գ���δ��ʼ
BOOL RegretAvailable = FALSE;
BOOL IsConnect = FALSE;
BOOL IsConnected = FALSE;//���ڷ��������Ƿ����ӳɹ�
BOOL IsServer;
BOOL IsGiveUp = FALSE;
CGoBangTCPDlg *Main;
CSockMessage Msg;
// CGoBangTCPDlg �Ի���



CGoBangTCPDlg::CGoBangTCPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGoBangTCPDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGoBangTCPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAMESHOW, m_GameDoc);
}

BEGIN_MESSAGE_MAP(CGoBangTCPDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BUTTONNEWGAME, OnClickBUTTONNEW)
	ON_BN_CLICKED(IDC_BUTTONREGRET, OnClickBUTTONREGRET)
	ON_BN_CLICKED(IDC_BUTTONEXIT, OnClickBUTTONEXIT)
	ON_MESSAGE(WM_SOCKETS, OnSocketS)
	ON_MESSAGE(WM_SOCKETC, OnSocketC)
END_MESSAGE_MAP()


// CGoBangTCPDlg ��Ϣ�������

BOOL CGoBangTCPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	Main = this;
	//ȷ����������
	CRect rect;
	GetClientRect(&rect);
	ClientX = rect.Width();
	ClientY = rect.Height();
	int SideLength = (ClientY - 86) / 14;
	int X0 = 46, Y0 = 46;
	Board = new CChessBoard(X0, Y0, SideLength);
	//���ư�ť
	CRect BoardRect = Board->GetBoardRect();
	int LeftX = BoardRect.right;
	int Left = (2 * LeftX + 1 * ClientX) / 3;
	int Right = (1 * LeftX + 2 * ClientX) / 3;
	int Up1 = BoardRect.top;
	int Up2 = Up1 + SideLength * 2;
	int Up3 = Up2 + SideLength * 2;
	CRect Buttons[3] ={CRect(Left, Up1, Right, Up1 + SideLength), CRect(Left, Up2, Right, Up2 + SideLength), CRect(Left, Up3, Right, Up3 + SideLength)};
	CButton *NewGame = new CButton;
	CButton *Regret = new CButton;
	CButton *Exit = new CButton;
	NewGame->Create(L"����Ϸ", WS_CHILD | WS_VISIBLE, Buttons[0], this, IDC_BUTTONNEWGAME);
	Regret->Create(L"����", WS_CHILD | WS_VISIBLE, Buttons[1], this, IDC_BUTTONREGRET);
	Exit->Create(L"�˳�", WS_CHILD | WS_VISIBLE, Buttons[2], this, IDC_BUTTONEXIT);
	//������
	GetDlgItem(IDC_GAMESHOW)->MoveWindow(LeftX + 20, Up3 + SideLength * 1.5, ClientX - LeftX - 40, BoardRect.bottom - Up3 - SideLength * 1.5);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGoBangTCPDlg::OnPaint()
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
		//���Ʊ���
		CPaintDC BackGround(this);
		CRect rect;
		GetClientRect(&rect);
		ClientX = rect.Width();
		ClientY = rect.Height();
		CDC dcMem;
		dcMem.CreateCompatibleDC(&BackGround);
		CBitmap bmpBackG;
		bmpBackG.LoadBitmap(IDB_BITMAP1);
		BITMAP bitmap;
		bmpBackG.GetBitmap(&bitmap);
		CBitmap* pbmpOld = dcMem.SelectObject(&bmpBackG);

		BackGround.StretchBlt(0, 0, ClientX, ClientY, &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
		dcMem.SelectObject(pbmpOld);
		dcMem.DeleteDC();

		//��������
		Board->DrawBoard(this);
		//CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGoBangTCPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGoBangTCPDlg::OnClickBUTTONNEW()
{
	BOOL flag = TRUE;
	if (!IsOver && !IsNew)
	{
		if (MessageBox(L"��δ�������Ƿ�ʼ����Ϸ��", L"��ʾ", MB_OKCANCEL) == IDCANCEL)
		{
			flag = FALSE;
		}
	}
	if (flag)
	{
	//�������Ϸ����
		if (IsOver && !IsGiveUp)//�Ѿ��ĵ�����
		{
			time_t tt = time(NULL);
			tm* t= localtime(&tt);
			CString T;
			T.Format(L"ʱ�䣺%d��%02d��%02d�� %02d:%02d:%02d\r\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
			T += GameDoc;
			USES_CONVERSION;
			char* doc = T2A(T);
			std::ofstream out("Document.txt", std::ios::app);
			out << doc << std::endl;
			out.close();
		}
		closesocket(Msock);
		closesocket(Fsock);
		CCHOOSEMODE Choose;
		int Res = Choose.DoModal();
		if (Res == IDOK)
		{
			Board->NewGame();
			Control.ClearRecord();
			GameDoc.Empty();
			if (IsConnect && !IsNew && IsConnected && !IsOver)
			{
				if (!Msg.SendMsg(Msock, Fsock, -2, IsServer) && !IsGiveUp)
					MessageBox(L"����ʧ�ܣ�");
			}
			IsNew = FALSE;
			IsOver = FALSE;
			if (!IsConnect)
			{
				GameDoc += "������Ϸ��ʼ���������֣�\r\n";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				IsGiveUp = FALSE;
			}
			else if (IsServer)
			{
				GameDoc += "������Ϸ������������ʼ��������������...\r\n";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				IsConnected = FALSE;
			}
			else
			{
				GameDoc += "������Ϸ���ͻ��ˣ���ʼ��\r\n���壨�����������֣�\r\n";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				IsConnected = TRUE;
				IsGiveUp = FALSE;
			}
			Invalidate();
		}
	}
}

void CGoBangTCPDlg::OnClickBUTTONREGRET()
{
	if (IsConnect)
	{
		if (Control.turn % 2 != IsServer)
		{
			MessageBox(L"���ڲ�����Ļغϣ�");
		}
		else
		{
			if (IsOver || Control.turn <= 2)
			{
				MessageBox(L"���ڲ��ܻ��壡", L"��ʾ");
			}
			else if (!RegretAvailable)
			{
				MessageBox(L"�����������һ�Σ�", L"��ʾ");
			}
			else
			{
				Control.Regret(Board);
				GameDoc += "���壡\r\n";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				if (!Msg.SendMsg(Msock, Fsock, -1, IsServer))
					MessageBox(L"����ʧ�ܣ�");
				Invalidate();
				RegretAvailable = FALSE;
			}
		}
	}
	else
	{
		if (IsOver || Control.turn <= 2)
		{
			MessageBox(L"���ڲ��ܻ��壡", L"��ʾ");
		}
		else if (!RegretAvailable)
		{
			MessageBox(L"�����������һ�Σ�", L"��ʾ");
		}
		else
		{
			Control.Regret(Board);
			GameDoc += "���壡\r\n";
			GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);	
			Invalidate();
			RegretAvailable = FALSE;
		}	
	}
}

void CGoBangTCPDlg::OnClickBUTTONEXIT()
{
	BOOL flag = 1;
	if (!IsOver && !IsNew)
	{
		if (MessageBox(L"��δ�������Ƿ��˳���Ϸ��", L"��ʾ", MB_OKCANCEL) == IDCANCEL)
		{
			flag = 0;
		}
	}
	if (flag)
	{
		if (IsConnect && IsConnected  && !IsOver)
		{
			if (!Msg.SendMsg(Msock, Fsock, -2, IsServer) && !IsGiveUp)
				MessageBox(L"����ʧ�ܣ�");
		}
		if (IsOver && !IsGiveUp)
		{
			time_t tt = time(NULL);
			tm* t= localtime(&tt);
			CString T;
			T.Format(L"ʱ�䣺%d��%02d��%02d�� %02d:%02d:%02d\r\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
			T += GameDoc;
			USES_CONVERSION;
			char* doc = T2A(T);
			std::ofstream out("Document.txt", std::ios::app);
			out << doc << std::endl;
			out.close();
		}
		CDialog::OnCancel();
	}
}

void CGoBangTCPDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!IsOver && !IsNew)
	{
		if (IsConnect)
		{
			if (IsServer)
			{
				if (Control.turn % 2 == 1)
				{
					int Result = Control.TakeStep(point, Board);
					if (Result)
					{
						Board->DrawChess(this);
						GameDoc += Control.GetRecord();
						GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
						m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
						if (!Msg.SendMsg(Msock, Fsock, Control.LastChess(), IsServer))
							MessageBox(L"����ʧ�ܣ�");
						RegretAvailable = TRUE;
						switch (Result)
						{
						case 2:
							MessageBox(L"�׷�ʤ��", L"��Ϸ����");
							IsOver = TRUE;
							GameDoc += L"�׷�ʤ\r\n";
							GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
							m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
							break;
						case 3:
							MessageBox(L"ƽ�֣�", L"��Ϸ����"); 
							IsOver = TRUE;
							GameDoc += L"ƽ��\r\n";
							GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
							m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
							break;
						case 4:
							MessageBox(L"�ڷ�ʤ��", L"��Ϸ����");
							IsOver = TRUE;
							GameDoc += L"�ڷ�ʤ\r\n";
							GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
							m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
							break;
						default:
							break;
						}
					}
				}
			}
			else
			{
				if (Control.turn % 2 == 0)
				{
					int Result = Control.TakeStep(point, Board);
					if (Result)
					{
						Board->DrawChess(this);
						GameDoc += Control.GetRecord();
						GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
						m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
						if (!Msg.SendMsg(Msock, Fsock, Control.LastChess(), IsServer))
							MessageBox(L"����ʧ�ܣ�");
						RegretAvailable = TRUE;
						switch (Result)
						{
						case 2:
							MessageBox(L"�׷�ʤ��", L"��Ϸ����");
							IsOver = TRUE;
							GameDoc += L"�׷�ʤ\r\n";
							GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
							m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
							break;
						case 3:
							MessageBox(L"ƽ�֣�", L"��Ϸ����"); 
							IsOver = TRUE;
							GameDoc += L"ƽ��\r\n";
							GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
							m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
							break;
						case 4:
							MessageBox(L"�ڷ�ʤ��", L"��Ϸ����");
							IsOver = TRUE;
							GameDoc += L"�ڷ�ʤ\r\n";
							GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
							m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
							break;
						default:
							break;
						}
					}
				}
			}
		}
		else
		{
			int Result = Control.TakeStep(point, Board);
			if (Result)
			{
				Board->DrawChess(this);
				GameDoc += Control.GetRecord();
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
				RegretAvailable = TRUE;
				switch (Result)
				{
				case 2:
					MessageBox(L"�׷�ʤ��", L"��Ϸ����");
					IsOver = TRUE;
					GameDoc += L"�׷�ʤ\r\n";
					GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
					m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
					break;
				case 3:
					MessageBox(L"ƽ�֣�", L"��Ϸ����"); 
					IsOver = TRUE;
					GameDoc += L"ƽ��\r\n";
					GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
					m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
					break;
				case 4:
					MessageBox(L"�ڷ�ʤ��", L"��Ϸ����");
					IsOver = TRUE;
					GameDoc += L"�ڷ�ʤ\r\n";
					GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
					m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
					break;
				default:
					break;
				}
			}
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CGoBangTCPDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	lpMMI->ptMinTrackSize.x = 1200; // �趨��С���ٿ��

    lpMMI->ptMinTrackSize.y = 800; // �趨��С���ٸ߶�

    lpMMI->ptMaxTrackSize.x = 1200; // �趨�����ٿ��

    lpMMI->ptMaxTrackSize.y = 800; // �趨�����ٸ߶�
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

LRESULT CGoBangTCPDlg::OnSocketS(WPARAM wParam, LPARAM lParam)//����������
{
	char cs[100] = {0};
	switch (lParam)
	{
	case FD_ACCEPT:
		{
			int length = sizeof(addrF);
			Fsock = ::accept(Msock, (sockaddr*)&addrF, &length);
			IsConnected = TRUE;
			GameDoc += "���ӳɹ���\r\n���壨�����������֣�\r\n";
			GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
			IsGiveUp = FALSE;
			break;
		}
	case FD_READ:
		{
			::recv(Fsock, cs, 100, 0);
			//��Ϣ������
			int num = Msg.RecvMsg(cs);
			if (num >= 0)
			{
				int Result = Control.TakeStep(num, Board);
				Board->DrawChess(this);
				GameDoc += Control.GetRecord();
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
				RegretAvailable = TRUE;
				switch (Result)
				{
				case 2:
					MessageBox(L"�׷�ʤ��", L"��Ϸ����");
					IsOver = TRUE;
					GameDoc += L"�׷�ʤ\r\n";
					GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
					break;
				case 3:
					MessageBox(L"ƽ�֣�", L"��Ϸ����"); 
					IsOver = TRUE;
					GameDoc += L"ƽ��\r\n";
					GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
					break;
				case 4:
					MessageBox(L"�ڷ�ʤ��", L"��Ϸ����");
					IsOver = TRUE;
					GameDoc += L"�ڷ�ʤ\r\n";
					GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
					break;
				default:
					break;
				}
			}
			else
			{
				switch(num)
				{
				case -1:
					Control.Regret(Board);
					GameDoc += "���壡\r\n";
					GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
					Invalidate();
					RegretAvailable = FALSE;
					break;
				case -2:
					GameDoc += "�Է������˾֣�";
					IsGiveUp = TRUE;
					GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
					IsOver = TRUE;
					break;
				}
			}
		}
	//default:
		//break;
	}
	return 0;
}
LRESULT CGoBangTCPDlg::OnSocketC(WPARAM wParam, LPARAM lParam)//�ͻ��˴���
{
	char cs[100] = {0};
	if (lParam == FD_READ)
	{
		recv(Msock, cs, 100, NULL);
		int num = Msg.RecvMsg(cs);
		if (num >= 0)
		{
			int Result = Control.TakeStep(num, Board);
			Board->DrawChess(this);
			GameDoc += Control.GetRecord();
			GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
			m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
			RegretAvailable = TRUE;
			switch (Result)
			{
			case 2:
				MessageBox(L"�׷�ʤ��", L"��Ϸ����");
				IsOver = TRUE;
				GameDoc += L"�׷�ʤ\r\n";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				break;
			case 3:
				MessageBox(L"ƽ�֣�", L"��Ϸ����"); 
				IsOver = TRUE;
				GameDoc += L"ƽ��\r\n";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				break;
			case 4:
				MessageBox(L"�ڷ�ʤ��", L"��Ϸ����");
				IsOver = TRUE;
				GameDoc += L"�ڷ�ʤ\r\n";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				break;
			default:
				break;
			}
		}
		else
		{
			switch(num)
			{
			case -1:
				Control.Regret(Board);
				GameDoc += "���壡\r\n";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				Invalidate();
				RegretAvailable = FALSE;
				break;
			case -2:
				GameDoc += "�Է������˾֣�";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				MessageBox(L"�Է������˾֣�");
				IsGiveUp = TRUE;
				IsOver = TRUE;
				break;
			}
		}
	}
	return 0;
}

