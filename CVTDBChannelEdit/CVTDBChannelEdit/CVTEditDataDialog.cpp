// CVTEditDataDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "CVTDBChannelEdit.h"
#include "CVTEditDataDialog.h"
#include "afxdialogex.h"


// CVTEditDataDialog 对话框

IMPLEMENT_DYNAMIC(CVTEditDataDialog, CDialogEx)

CVTEditDataDialog::CVTEditDataDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVTEditDataDialog::IDD, pParent)
{

}

CVTEditDataDialog::~CVTEditDataDialog()
{
}

void CVTEditDataDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVTEditDataDialog, CDialogEx)
END_MESSAGE_MAP()


// CVTEditDataDialog 消息处理程序
