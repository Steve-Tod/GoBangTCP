
// GoBangTCPDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#define WM_SOCKETS WM_USER + 200
#define WM_SOCKETC WM_USER + 300

// CGoBangTCPDlg �Ի���
class CGoBangTCPDlg : public CDialogEx
{
// ����
public:
	CGoBangTCPDlg(CWnd* pParent = NULL);	// ��׼���캯��
	SOCKET Msock, Fsock;
	sockaddr_in addrM, addrF;
// �Ի�������
	enum { IDD = IDD_GOBANGTCP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
