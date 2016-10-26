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
	infoIndex = 0;
	CString indexStr;
	for (int i = 0; i < ABOUT_INFO_COUNT; i++)
	{
		aboutDetailsInfos[i].index.Format(_T("%d/%d"), i + 1, ABOUT_INFO_COUNT);
	}
}

CVTDBEditAboutDialog::~CVTDBEditAboutDialog()
{
}

void CVTDBEditAboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ABOUT_PREV, CButtonAboutPrev);
	DDX_Control(pDX, IDC_BUTTON_ABOUT_NEXT, CButtonAboutNext);
	DDX_Control(pDX, IDC_STATIC_ABOUT_INDEX_TEXT, CStaticAboutIndexText);
	DDX_Control(pDX, IDC_STATIC_ABOUT_TITLE_TEXT, CStaticAboutTitleText);
	DDX_Control(pDX, IDC_STATIC_ABOUT_DETAILS_TEXT, CStaticAboutDetailsText);
	DDX_Control(pDX, IDC_STATIC_ABOUT_PICTURE, CStaticAboutPicture);
}


BEGIN_MESSAGE_MAP(CVTDBEditAboutDialog, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_ABOUT_NEXT, &CVTDBEditAboutDialog::OnBnClickedButtonAboutNext)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT_PREV, &CVTDBEditAboutDialog::OnBnClickedButtonAboutPrev)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CVTDBEditAboutDialog 消息处理程序


BOOL CVTDBEditAboutDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	pTitleFont = new CFont;
	pTitleFont->CreateFont(30, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_BOLD, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("Microsoft Sans Serif")); // lpszFac
	CStaticAboutTitleText.SetFont(pTitleFont);
	pDetailsFont = new CFont;
	pDetailsFont->CreateFont(20, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_NORMAL, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("Microsoft Sans Serif")); // lpszFac
	CStaticAboutDetailsText.SetFont(pDetailsFont);
	pIndexFont = new CFont;
	pIndexFont->CreateFont(35, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_BOLD, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("Microsoft Sans Serif")); // lpszFac
	CStaticAboutIndexText.SetFont(pIndexFont);
	pGroupFont = new CFont;
	pGroupFont->CreateFont(20, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_NORMAL, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("Microsoft Sans Serif")); // lpszFac
	GetDlgItem(IDC_STATIC_GROUP_DETAILS)->SetFont(pGroupFont);
	GetDlgItem(IDC_STATIC_GROUP_PICTURE)->SetFont(pGroupFont);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CVTDBEditAboutDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	CRect rect;

	//GetClientRect(rect);
	//dc.FillSolidRect(rect, RGB(255, 255, 255));   
	//dc.SetBkMode(TRANSPARENT);
	//CRect prect;
	//GetDlgItem(IDC_STATIC_GROUP_PICTURE)->GetClientRect(&prect);   //获取区域
	//FillRect(GetDlgItem(IDC_STATIC_GROUP_PICTURE)->GetDC()->GetSafeHdc(), &prect, CBrush(RGB(255, 255, 255)));     //填充颜色

	CStaticAboutPicture.GetWindowRect(&rect);
	ScreenToClient(&rect);

	CDC memDC; // 定义内存设备
	CBitmap bmpBack, *pBmp;
	BITMAP    bitmap;
	bmpBack.LoadBitmap(aboutDetailsInfos[infoIndex].BITMAP);
	memDC.CreateCompatibleDC(&dc);
	dc.SetStretchBltMode(COLORONCOLOR);
	pBmp = memDC.SelectObject(&bmpBack);
	bmpBack.GetBitmap(&bitmap);
	dc.SetStretchBltMode(HALFTONE);
	dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),
		&memDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	memDC.SelectObject(pBmp);

	CStaticAboutTitleText.SetWindowTextW(aboutDetailsInfos[infoIndex].title);
	CStaticAboutDetailsText.SetWindowTextW(aboutDetailsInfos[infoIndex].details);
	CStaticAboutIndexText.SetWindowTextW(aboutDetailsInfos[infoIndex].index);

	if (infoIndex == ABOUT_INFO_COUNT - 1)
	{
		CButtonAboutNext.EnableWindow(false);
	}else if (infoIndex == 0)
	{
		CButtonAboutPrev.EnableWindow(false);
	}
	else if ((infoIndex > 0) && (infoIndex < ABOUT_INFO_COUNT - 1))
	{
		CButtonAboutPrev.EnableWindow(true);
		CButtonAboutNext.EnableWindow(true);
	}
	else{
		CButtonAboutNext.EnableWindow(false);
		CButtonAboutPrev.EnableWindow(false);
	}

	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CVTDBEditAboutDialog::OnBnClickedButtonAboutNext()
{
	// TODO:  在此添加控件通知处理程序代码
	infoIndex++;
	if (infoIndex >= ABOUT_INFO_COUNT - 1)
	{
		infoIndex = ABOUT_INFO_COUNT -1;
	}
	Invalidate();
}


void CVTDBEditAboutDialog::OnBnClickedButtonAboutPrev()
{
	// TODO:  在此添加控件通知处理程序代码
	infoIndex--;
	if (infoIndex <= 0)
	{
		infoIndex = 0;
	}
	Invalidate();
}


HBRUSH CVTDBEditAboutDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_ABOUT_DETAILS_TEXT:
		pDC->SetTextColor(RGB(200, 30, 20));
		break;
	case IDC_STATIC_ABOUT_INDEX_TEXT:
		pDC->SetTextColor(RGB(30, 50, 120));
		break;
	case IDC_STATIC_ABOUT_TITLE_TEXT:
		pDC->SetTextColor(RGB(130, 20, 50));
		break;
	default:
		break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
