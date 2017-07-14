#pragma once
#include "afxwin.h"


// CCHOOSEMODE 对话框

class CCHOOSEMODE : public CDialogEx
{
	DECLARE_DYNAMIC(CCHOOSEMODE)

public:
	CCHOOSEMODE(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCHOOSEMODE();

// 对话框数据
	enum { IDD = IDD_CHOOSEMODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_Choice;
	afx_msg void OnBnClickedOk();
};
