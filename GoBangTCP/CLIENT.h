#pragma once


// CCLIENT 对话框

class CCLIENT : public CDialogEx
{
	DECLARE_DYNAMIC(CCLIENT)

public:
	CCLIENT(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCLIENT();

// 对话框数据
	enum { IDD = IDD_CLIENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnect();
};
