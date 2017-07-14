
// GoBangTCPDlg.cpp : 实现文件
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

int ClientX, ClientY;//获取客户区大小
CChessBoard *Board;
GameControl Control;
CString GameDoc;
BOOL IsOver = FALSE;
BOOL IsNew = TRUE;//是否棋盘为空，尚未开始
BOOL RegretAvailable = FALSE;
BOOL IsConnect = FALSE;
BOOL IsConnected = FALSE;//对于服务器看是否连接成功
BOOL IsServer;
BOOL IsGiveUp = FALSE;
CGoBangTCPDlg *Main;
CSockMessage Msg;
// CGoBangTCPDlg 对话框



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


// CGoBangTCPDlg 消息处理程序

BOOL CGoBangTCPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	Main = this;
	//确定棋盘数组
	CRect rect;
	GetClientRect(&rect);
	ClientX = rect.Width();
	ClientY = rect.Height();
	int SideLength = (ClientY - 86) / 14;
	int X0 = 46, Y0 = 46;
	Board = new CChessBoard(X0, Y0, SideLength);
	//绘制按钮
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
	NewGame->Create(L"新游戏", WS_CHILD | WS_VISIBLE, Buttons[0], this, IDC_BUTTONNEWGAME);
	Regret->Create(L"悔棋", WS_CHILD | WS_VISIBLE, Buttons[1], this, IDC_BUTTONREGRET);
	Exit->Create(L"退出", WS_CHILD | WS_VISIBLE, Buttons[2], this, IDC_BUTTONEXIT);
	//滚动屏
	GetDlgItem(IDC_GAMESHOW)->MoveWindow(LeftX + 20, Up3 + SideLength * 1.5, ClientX - LeftX - 40, BoardRect.bottom - Up3 - SideLength * 1.5);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGoBangTCPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//绘制背景
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

		//绘制棋盘
		Board->DrawBoard(this);
		//CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGoBangTCPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGoBangTCPDlg::OnClickBUTTONNEW()
{
	BOOL flag = TRUE;
	if (!IsOver && !IsNew)
	{
		if (MessageBox(L"尚未结束，是否开始新游戏？", L"提示", MB_OKCANCEL) == IDCANCEL)
		{
			flag = FALSE;
		}
	}
	if (flag)
	{
	//添加新游戏代码
		if (IsOver && !IsGiveUp)//把旧文档存入
		{
			time_t tt = time(NULL);
			tm* t= localtime(&tt);
			CString T;
			T.Format(L"时间：%d年%02d月%02d日 %02d:%02d:%02d\r\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
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
					MessageBox(L"发送失败！");
			}
			IsNew = FALSE;
			IsOver = FALSE;
			if (!IsConnect)
			{
				GameDoc += "本地游戏开始！黑棋先手！\r\n";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				IsGiveUp = FALSE;
			}
			else if (IsServer)
			{
				GameDoc += "联网游戏（服务器）开始，服务器监听中...\r\n";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				IsConnected = FALSE;
			}
			else
			{
				GameDoc += "联网游戏（客户端）开始！\r\n黑棋（服务器）先手！\r\n";
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
			MessageBox(L"现在不是你的回合！");
		}
		else
		{
			if (IsOver || Control.turn <= 2)
			{
				MessageBox(L"现在不能悔棋！", L"提示");
			}
			else if (!RegretAvailable)
			{
				MessageBox(L"最多连续悔棋一次！", L"提示");
			}
			else
			{
				Control.Regret(Board);
				GameDoc += "悔棋！\r\n";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				if (!Msg.SendMsg(Msock, Fsock, -1, IsServer))
					MessageBox(L"发送失败！");
				Invalidate();
				RegretAvailable = FALSE;
			}
		}
	}
	else
	{
		if (IsOver || Control.turn <= 2)
		{
			MessageBox(L"现在不能悔棋！", L"提示");
		}
		else if (!RegretAvailable)
		{
			MessageBox(L"最多连续悔棋一次！", L"提示");
		}
		else
		{
			Control.Regret(Board);
			GameDoc += "悔棋！\r\n";
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
		if (MessageBox(L"尚未结束，是否退出游戏？", L"提示", MB_OKCANCEL) == IDCANCEL)
		{
			flag = 0;
		}
	}
	if (flag)
	{
		if (IsConnect && IsConnected  && !IsOver)
		{
			if (!Msg.SendMsg(Msock, Fsock, -2, IsServer) && !IsGiveUp)
				MessageBox(L"发送失败！");
		}
		if (IsOver && !IsGiveUp)
		{
			time_t tt = time(NULL);
			tm* t= localtime(&tt);
			CString T;
			T.Format(L"时间：%d年%02d月%02d日 %02d:%02d:%02d\r\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
							MessageBox(L"发送失败！");
						RegretAvailable = TRUE;
						switch (Result)
						{
						case 2:
							MessageBox(L"白方胜！", L"游戏结束");
							IsOver = TRUE;
							GameDoc += L"白方胜\r\n";
							GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
							m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
							break;
						case 3:
							MessageBox(L"平局！", L"游戏结束"); 
							IsOver = TRUE;
							GameDoc += L"平局\r\n";
							GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
							m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
							break;
						case 4:
							MessageBox(L"黑方胜！", L"游戏结束");
							IsOver = TRUE;
							GameDoc += L"黑方胜\r\n";
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
							MessageBox(L"发送失败！");
						RegretAvailable = TRUE;
						switch (Result)
						{
						case 2:
							MessageBox(L"白方胜！", L"游戏结束");
							IsOver = TRUE;
							GameDoc += L"白方胜\r\n";
							GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
							m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
							break;
						case 3:
							MessageBox(L"平局！", L"游戏结束"); 
							IsOver = TRUE;
							GameDoc += L"平局\r\n";
							GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
							m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
							break;
						case 4:
							MessageBox(L"黑方胜！", L"游戏结束");
							IsOver = TRUE;
							GameDoc += L"黑方胜\r\n";
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
					MessageBox(L"白方胜！", L"游戏结束");
					IsOver = TRUE;
					GameDoc += L"白方胜\r\n";
					GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
					m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
					break;
				case 3:
					MessageBox(L"平局！", L"游戏结束"); 
					IsOver = TRUE;
					GameDoc += L"平局\r\n";
					GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
					m_GameDoc.LineScroll(m_GameDoc.GetLineCount());
					break;
				case 4:
					MessageBox(L"黑方胜！", L"游戏结束");
					IsOver = TRUE;
					GameDoc += L"黑方胜\r\n";
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 1200; // 设定最小跟踪宽度

    lpMMI->ptMinTrackSize.y = 800; // 设定最小跟踪高度

    lpMMI->ptMaxTrackSize.x = 1200; // 设定最大跟踪宽度

    lpMMI->ptMaxTrackSize.y = 800; // 设定最大跟踪高度
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

LRESULT CGoBangTCPDlg::OnSocketS(WPARAM wParam, LPARAM lParam)//服务器处理
{
	char cs[100] = {0};
	switch (lParam)
	{
	case FD_ACCEPT:
		{
			int length = sizeof(addrF);
			Fsock = ::accept(Msock, (sockaddr*)&addrF, &length);
			IsConnected = TRUE;
			GameDoc += "连接成功！\r\n黑棋（服务器）先手！\r\n";
			GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
			IsGiveUp = FALSE;
			break;
		}
	case FD_READ:
		{
			::recv(Fsock, cs, 100, 0);
			//消息处理函数
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
					MessageBox(L"白方胜！", L"游戏结束");
					IsOver = TRUE;
					GameDoc += L"白方胜\r\n";
					GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
					break;
				case 3:
					MessageBox(L"平局！", L"游戏结束"); 
					IsOver = TRUE;
					GameDoc += L"平局\r\n";
					GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
					break;
				case 4:
					MessageBox(L"黑方胜！", L"游戏结束");
					IsOver = TRUE;
					GameDoc += L"黑方胜\r\n";
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
					GameDoc += "悔棋！\r\n";
					GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
					Invalidate();
					RegretAvailable = FALSE;
					break;
				case -2:
					GameDoc += "对方放弃此局！";
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
LRESULT CGoBangTCPDlg::OnSocketC(WPARAM wParam, LPARAM lParam)//客户端处理
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
				MessageBox(L"白方胜！", L"游戏结束");
				IsOver = TRUE;
				GameDoc += L"白方胜\r\n";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				break;
			case 3:
				MessageBox(L"平局！", L"游戏结束"); 
				IsOver = TRUE;
				GameDoc += L"平局\r\n";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				break;
			case 4:
				MessageBox(L"黑方胜！", L"游戏结束");
				IsOver = TRUE;
				GameDoc += L"黑方胜\r\n";
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
				GameDoc += "悔棋！\r\n";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				Invalidate();
				RegretAvailable = FALSE;
				break;
			case -2:
				GameDoc += "对方放弃此局！";
				GetDlgItem(IDC_GAMESHOW)->SetWindowTextW(GameDoc);
				MessageBox(L"对方放弃此局！");
				IsGiveUp = TRUE;
				IsOver = TRUE;
				break;
			}
		}
	}
	return 0;
}

