#pragma once


// CVTDBEditAboutDialog 对话框

class CVTDBEditAboutDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CVTDBEditAboutDialog)

public:
	CVTDBEditAboutDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVTDBEditAboutDialog();

// 对话框数据
	enum { IDD = IDD_ABOUT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
