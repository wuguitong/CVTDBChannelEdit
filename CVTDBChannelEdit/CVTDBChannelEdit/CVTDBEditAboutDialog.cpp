// CVTDBEditAboutDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "CVTDBChannelEdit.h"
#include "CVTDBEditAboutDialog.h"
#include "afxdialogex.h"


// CVTDBEditAboutDialog 对话框

IMPLEMENT_DYNAMIC(CVTDBEditAboutDialog, CDialogEx)

CVTDBEditAboutDialog::CVTDBEditAboutDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVTDBEditAboutDialog::IDD, pParent)
{

}

CVTDBEditAboutDialog::~CVTDBEditAboutDialog()
{
}

void CVTDBEditAboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVTDBEditAboutDialog, CDialogEx)
END_MESSAGE_MAP()


// CVTDBEditAboutDialog 消息处理程序
