#pragma once


// CVTEditDataDialog 对话框

class CVTEditDataDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CVTEditDataDialog)

public:
	CVTEditDataDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVTEditDataDialog();

// 对话框数据
	enum { IDD = IDD_EDIT_DATA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
