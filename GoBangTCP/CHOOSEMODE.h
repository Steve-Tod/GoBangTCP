#pragma once
#include "afxwin.h"


// CCHOOSEMODE �Ի���

class CCHOOSEMODE : public CDialogEx
{
	DECLARE_DYNAMIC(CCHOOSEMODE)

public:
	CCHOOSEMODE(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCHOOSEMODE();

// �Ի�������
	enum { IDD = IDD_CHOOSEMODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_Choice;
	afx_msg void OnBnClickedOk();
};
