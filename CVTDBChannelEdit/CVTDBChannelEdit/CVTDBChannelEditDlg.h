
// CVTDBChannelEditDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "QuickList.h"
#include "sqlite3.h"
#include "CVTDBUtil.h"
#include "afxwin.h"

#define SOFTWARE_VERSION "V1.0"

// CCVTDBChannelEditDlg 对话框
#define PATH_NAME_HEAD "File : "
#define BOARD_TYPE_HEAD "Board : "
#define MAIN_DIALOG_TITLE "CVTDBChannelEdit"


class CCVTDBChannelEditDlg : public CDialogEx
{
// 构造
public:
	CCVTDBChannelEditDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CVTDBCHANNELEDIT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnGetListItem(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CFont* pFont;
	CString filePath;
	CQuickList ClistCtrlDBData;
	ChannelVector* pChannelVector;
	CVTDBUtil* pCVTDBUtil;
	CStatic CStaticPathStr;
	BOARD_TYPE boardType;
	void UpdateList(int itemCount);
	afx_msg void OnBnClickedOpendb();
	afx_msg void OnBnClickedSavedb();
	afx_msg void OnNMDblclkListDbData(NMHDR *pNMHDR, LRESULT *pResult);
	LRESULT OnListClick(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	BOOL CStringIsAscii(CString str);
	afx_msg void OnClose();
	afx_msg void OnHelpAbout();
	CStatic CStaticBoardTypeStr;
};
