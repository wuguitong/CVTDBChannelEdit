// CVTEditDataDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CVTDBChannelEdit.h"
#include "CVTEditDataDialog.h"
#include "afxdialogex.h"


// CVTEditDataDialog �Ի���

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


// CVTEditDataDialog ��Ϣ�������
