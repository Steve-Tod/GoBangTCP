#pragma once


// CSERVER �Ի���

class CSERVER : public CDialogEx
{
	DECLARE_DYNAMIC(CSERVER)

public:
	CSERVER(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSERVER();

// �Ի�������
	enum { IDD = IDD_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedListen();
};
