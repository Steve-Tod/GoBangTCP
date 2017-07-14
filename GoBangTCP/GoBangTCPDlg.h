
// GoBangTCPDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#define WM_SOCKETS WM_USER + 200
#define WM_SOCKETC WM_USER + 300

// CGoBangTCPDlg 对话框
class CGoBangTCPDlg : public CDialogEx
{
// 构造
public:
	CGoBangTCPDlg(CWnd* pParent = NULL);	// 标准构造函数
	SOCKET Msock, Fsock;
	sockaddr_in addrM, addrF;
// 对话框数据
	enum { IDD = IDD_GOBANGTCP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClickBUTTONNEW();
	afx_msg void OnClickBUTTONREGRET();
	afx_msg void OnClickBUTTONEXIT();
	afx_msg LRESULT OnSocketS(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSocketC(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	CEdit m_GameDoc;
};
