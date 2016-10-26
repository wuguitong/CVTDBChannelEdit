#pragma once
#include "afxwin.h"

typedef struct {
	CString title;
	CString details;
	CString index;
	unsigned int BITMAP;
}AboutDetailsInfo;
static AboutDetailsInfo aboutDetailsInfos[]
{
	{ 
		_T("Open DB"),
		_T("  Step1:Click on Open DB button.\n\n  Step2:Select db file and open it."),
		_T(""),
		IDB_BITMAP_OPEN_DB,
	},
	{
		_T("Item Declare"),
		_T("  Item 1:\"File:\" is show the file path.\n\n  Item 2:\"Board:\" is show the db get from which board.\n\n  Item 3:Channel Select for delete.\n\n  Item 4:Show Channel Pos and can be edited.\n\n  Item 5:Show Channel Name and can be edited.\n\n  Item 6:Show Channel Skip and can be selected.\n\n  Item 7:Show Channel Lock and can be selected.\n\n  Item 7:Show Channel Type and it can not change."),
		_T(""),
		IDB_BITMAP_ITEM_DECLARE, 
	},
	{
		_T("Edit Pos"),
		_T("  Double click Pos item then it can be edited."),
		_T(""),
		IDB_BITMAP_EDIT_POS,
	},
	{
		_T("Edit Name"),
		_T("  Double click Name item then it can be edited."),
		_T(""),
		IDB_BITMAP_EDIT_NAME,
	},
	{
		_T("Skip and Lock"),
		_T("  Click Skip checkbox to select or not,the same as Lock"),
		_T(""),
		IDB_BITMAP_CHANGE_LOCK_SKIP,
	},
	{
		_T("Del Channel"),
		_T("  Step1:Select channel .\n\n  Step2:Click Delete Button to delete channel which was selected."),
		_T(""),
		IDB_BITMAP_DELETE_CHANNEL,
	},
	{
		_T("Save DB"),
		_T("  Click Save DB Button to save data to db file.\n\n  In addition,when close windows it will ask if save data to db"),
		_T(""),
		IDB_BITMAP_SAVE_DB,
	},
};

#define ABOUT_INFO_COUNT (sizeof(aboutDetailsInfos)/sizeof(aboutDetailsInfos[0]))

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
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonAboutNext();
	afx_msg void OnBnClickedButtonAboutPrev();
	CButton CButtonAboutPrev;
	CButton CButtonAboutNext;
	CStatic CStaticAboutIndexText;
	CStatic CStaticAboutTitleText;
	CStatic CStaticAboutDetailsText;
	CStatic CStaticAboutPicture;
	CFont* pTitleFont;
	CFont* pDetailsFont;
	CFont* pIndexFont;
	CFont* pGroupFont;
	int infoIndex;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
