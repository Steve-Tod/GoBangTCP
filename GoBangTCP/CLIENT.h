#pragma once


// CCLIENT �Ի���

class CCLIENT : public CDialogEx
{
	DECLARE_DYNAMIC(CCLIENT)

public:
	CCLIENT(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCLIENT();

// �Ի�������
	enum { IDD = IDD_CLIENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnect();
};
