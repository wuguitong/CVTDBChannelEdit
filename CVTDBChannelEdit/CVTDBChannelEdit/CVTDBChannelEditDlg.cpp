
// CVTDBChannelEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CVTDBChannelEdit.h"
#include "CVTDBChannelEditDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCVTDBChannelEditDlg 对话框



CCVTDBChannelEditDlg::CCVTDBChannelEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCVTDBChannelEditDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCVTDBChannelEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DB_DATA, ClistCtrlDBData);
	DDX_Control(pDX, IDC_STATIC_PATH_STR, CStaticPathStr);
}

BEGIN_MESSAGE_MAP(CCVTDBChannelEditDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOPENDB, &CCVTDBChannelEditDlg::OnBnClickedOpendb)
	ON_BN_CLICKED(IDSAVEDB, &CCVTDBChannelEditDlg::OnBnClickedSavedb)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DB_DATA, &CCVTDBChannelEditDlg::OnNMDblclkListDbData)

	//
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_DB_DATA, OnEndlabeleditList)
	ON_MESSAGE(WM_QUICKLIST_GETLISTITEMDATA, OnGetListItem)
	ON_MESSAGE(WM_QUICKLIST_CLICK, OnListClick)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CCVTDBChannelEditDlg::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CCVTDBChannelEditDlg 消息处理程序

BOOL CCVTDBChannelEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	USES_CONVERSION;
	//debug
	AllocConsole();
	FILE *fp;
	freopen_s(&fp, "CONOUT$", "w+t", stdout);
	//debug
	ListView_SetExtendedListViewStyleEx(ClistCtrlDBData.m_hWnd, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	ListView_SetExtendedListViewStyleEx(ClistCtrlDBData.m_hWnd, LVS_EX_SUBITEMIMAGES, LVS_EX_SUBITEMIMAGES);
	ListView_SetExtendedListViewStyleEx(ClistCtrlDBData.m_hWnd, LVS_EX_CHECKBOXES, LVS_EX_CHECKBOXES);
	ListView_SetExtendedListViewStyleEx(ClistCtrlDBData.m_hWnd, LVS_EX_HEADERDRAGDROP, LVS_EX_HEADERDRAGDROP);
	ListView_SetExtendedListViewStyleEx(ClistCtrlDBData.m_hWnd, LVS_EX_GRIDLINES, LVS_EX_GRIDLINES);
	ClistCtrlDBData.InsertColumn(CHANNEL_SELECT, _T(""), LVCFMT_LEFT, 25);
	ClistCtrlDBData.InsertColumn(CHANNEL_POS ,  _T("Channel Pos"), LVCFMT_CENTER, 150);
	ClistCtrlDBData.InsertColumn(CHANNEL_NAME , _T("Channel Name"), LVCFMT_CENTER, 300);
	ClistCtrlDBData.InsertColumn(CHANNEL_SKIP , _T("Channel Skip"), LVCFMT_CENTER, 150);
	ClistCtrlDBData.InsertColumn(CHANNEL_LOCK , _T("Channel Lock"), LVCFMT_CENTER, 150);
	ClistCtrlDBData.InsertColumn(CHANNEL_TYPE , _T("channel Type"), LVCFMT_CENTER, 150);

	pFont = new CFont;
	pFont->CreateFont(20, // nHeight 
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
	ClistCtrlDBData.SetFont(pFont);
	CHeaderCtrl*  pHeaderCtrl = (CHeaderCtrl*)ClistCtrlDBData.GetHeaderCtrl();
	pHeaderCtrl->EnableWindow(FALSE);
	pCVTDBUtil = NULL;
	pChannelVector = NULL;
	UpdateList(0);
	CStaticPathStr.SetWindowTextW(A2T(PATH_NAME_HEAD));
	CStaticPathStr.SetFont(pFont);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCVTDBChannelEditDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCVTDBChannelEditDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCVTDBChannelEditDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCVTDBChannelEditDlg::OnBnClickedOpendb()
{
	// TODO:  在此添加控件通知处理程序代码
	USES_CONVERSION;
	CString filter = A2T("(*.db;)|*.db;||");
	TCHAR *path = new TCHAR[MAX_PATH];
	ZeroMemory(path, MAX_PATH);
	GetCurrentDirectory(MAX_PATH, path);
	CFileDialog openFileDlg(TRUE, L"db", NULL, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	openFileDlg.m_ofn.lpstrInitialDir = path;
	delete path;
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK){
		pCVTDBUtil = CVTDBUtil::GetInstance();
		filePath = openFileDlg.GetPathName();
		pCVTDBUtil->SetDBFilePath(filePath);
		if (pCVTDBUtil->OpenDb() == TRUE)
		{
			result = pCVTDBUtil->ParseRAWData();
			pCVTDBUtil->CloseDb();
			pChannelVector = pCVTDBUtil->GetChannelVectorByPos();
			UpdateList(pChannelVector->size());
			CStaticPathStr.SetWindowTextW(A2T(PATH_NAME_HEAD) + filePath);
			if ((pChannelVector->size() == 0) && result)
			{
				MessageBox(_T("There is no channel!"), _T("Prompt"), MB_OK);
			}
			if (!result)
			{
				AfxMessageBox(A2T("Open DB Fail!"));
			}
		}
		else
		{
			AfxMessageBox(A2T("Open DB Fail!"));
		}
	}
}




void CCVTDBChannelEditDlg::OnBnClickedSavedb()
{
	USES_CONVERSION;
	// TODO:  在此添加控件通知处理程序代码
	BOOL result = FALSE;
	if (pCVTDBUtil == NULL)
	{
		AfxMessageBox(A2T("No DB Open!"));
		return;
	}
	if (pCVTDBUtil->OpenDb())
	{
		result = pCVTDBUtil->SaveDataToDb();
		pCVTDBUtil->CloseDb();
		if (result)
		{
			MessageBox(_T("DB Save Success!"), _T("Prompt"), MB_OK);
		}
		else{
			AfxMessageBox(A2T("Save DB Fail!"));
		}
	}
	else
	{
		AfxMessageBox(A2T("Can not Open DB!"));
	}
}


void CCVTDBChannelEditDlg::OnNMDblclkListDbData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}
LRESULT CCVTDBChannelEditDlg::OnGetListItem(WPARAM wParam, LPARAM lParam)
{
	USES_CONVERSION;
	ASSERT((HWND)wParam == ClistCtrlDBData.GetSafeHwnd());
	CQuickList::CListItemData* data = (CQuickList::CListItemData*) lParam;
	int item = data->GetItem();
	int subItem = data->GetSubItem();
	if (pChannelVector == NULL)
	{
		return 0;
	}
	if (data->GetItem() % 2 == 0)
	{
		data->m_colors.m_textColor = RGB(10, 5, 20);
		data->m_colors.m_backColor = RGB(180, 180, 255);
		data->m_colors.m_hotTextColor = RGB(170, 10, 150);
		data->m_colors.m_selectedBackColorNoFocus = RGB(20, 20, 120);
		data->m_colors.m_selectedBackColor = RGB(30, 0, 96);
		data->m_colors.m_selectedTextColor = RGB(240, 200, 128);
	}
	else
	{
		data->m_colors.m_textColor = RGB(40, 10, 40);
		data->m_colors.m_backColor = RGB(220, 220, 255);
		data->m_colors.m_hotTextColor = RGB(170, 10, 150);
		data->m_colors.m_selectedBackColorNoFocus = RGB(20, 60, 120);
		data->m_colors.m_selectedBackColor = RGB(30, 50, 96);
		data->m_colors.m_selectedTextColor = RGB(240, 250, 128);
	}
	if ((*pChannelVector)[item].isSelect)
	{
		data->m_colors.m_backColor = RGB(250, 250, 20);
	}
	switch (subItem)
	{
	case CHANNEL_SELECT:
		data->m_button.m_draw = true;
		data->m_button.m_noSelection = 1;
		data->m_button.m_style = DFCS_BUTTONCHECK;
		data->m_text = "";
		if ((*pChannelVector)[item].isSelect)
			data->m_button.m_style |= DFCS_CHECKED;
		break;
	case CHANNEL_POS:
		data->m_allowEdit = true;
		data->m_maxCharacter = 4;
		data->m_onlyNumber = true;
		data->m_text.Format(L"%d",(*pChannelVector)[item].channelPos + 1);
		break;
	case CHANNEL_NAME:
		data->m_allowEdit = true;
		
		if ((*pChannelVector)[item].channelType != TV_ATV_TYPE)
		{
			data->m_maxCharacter = 24;
			WORD name[DTV_CHANNEL_NAME_BYTE_SIZE];
			MApp_TranslateCharTableToUnicode((BYTE *)(*pChannelVector)[item].name, name, DTV_CHANNEL_NAME_BYTE_SIZE);
			data->m_text.Format(L"%s", name);
		}
		else{
			data->m_maxCharacter = 7;
			data->m_text.Format(L"%s", A2T((*pChannelVector)[item].name));
		}
		break;
	case CHANNEL_SKIP:
		data->m_button.m_draw = true;
		data->m_button.m_noSelection = 1;
		data->m_button.m_style = DFCS_BUTTONCHECK;
		data->m_text = "Skip";
		if ((*pChannelVector)[item].isSkip)
			data->m_button.m_style |= DFCS_CHECKED;
		break;
	case CHANNEL_LOCK:
		data->m_button.m_draw = true;
		data->m_button.m_noSelection = 1;
		data->m_button.m_style = DFCS_BUTTONCHECK;
		data->m_text = "Lock";
		if ((*pChannelVector)[item].isLock)
			data->m_button.m_style |= DFCS_CHECKED;
		break;
	case CHANNEL_TYPE:
		switch ((*pChannelVector)[item].channelType)
		{
		case TV_ATV_TYPE:
			data->m_text = "ATV";
			break;
		case TV_DTV_TYPE:
			data->m_text = "DTV";
			break;
		case TV_RADIO_TYPE:
			data->m_text = "RADIO";
			break;
		case TV_DATA_TYPE:
			data->m_text = "DATA";
			break;
		default:
			data->m_text = "UNKONW";
			break;
		}
		break;
	default:
		break;
	}
	return 0;
}
//User clicked on list. Toggle checkbox/image if hit
LRESULT CCVTDBChannelEditDlg::OnListClick(WPARAM wParam, LPARAM lParam)
{
	ASSERT((HWND)wParam == ClistCtrlDBData.GetSafeHwnd());
	CQuickList::CListHitInfo *hit = (CQuickList::CListHitInfo*) lParam;
	if (hit->m_onButton)
	{
		TRACE(_T("On check box. "));
		if (hit->m_subitem == CHANNEL_SKIP)
		{
			(*pChannelVector)[hit->m_item].isSkip = !(*pChannelVector)[hit->m_item].isSkip;
			ClistCtrlDBData.RedrawCheckBoxs(hit->m_item, hit->m_item, hit->m_subitem);
		}
		if (hit->m_subitem == CHANNEL_LOCK)
		{
			(*pChannelVector)[hit->m_item].isLock = !(*pChannelVector)[hit->m_item].isLock;
			ClistCtrlDBData.RedrawCheckBoxs(hit->m_item, hit->m_item, hit->m_subitem);
		}
		if (hit->m_subitem == CHANNEL_SELECT)
		{
			(*pChannelVector)[hit->m_item].isSelect = !(*pChannelVector)[hit->m_item].isSelect;
			ClistCtrlDBData.RedrawItems(hit->m_item, hit->m_item);
		}
	}
	return 0;
}
void CCVTDBChannelEditDlg::UpdateList(int itemCount)
{
	ClistCtrlDBData.LockWindowUpdate();
	ClistCtrlDBData.SetItemCount(itemCount);
	ClistCtrlDBData.UnlockWindowUpdate();
	ClistCtrlDBData.RedrawItems(
		ClistCtrlDBData.GetTopIndex(),
		ClistCtrlDBData.GetTopIndex() + ClistCtrlDBData.GetCountPerPage());
}

void CCVTDBChannelEditDlg::OnBnClickedButtonDelete()
{
	// TODO:  在此添加控件通知处理程序代码

	USES_CONVERSION;
	if (pCVTDBUtil == NULL)
		return;
	if (pCVTDBUtil->DeleteSelectChannel() > 0)
	{
		pCVTDBUtil->UpadtePosValueToContinue();
		pCVTDBUtil->UpdateAtvChannelNo();
		UpdateList(pChannelVector->size());
	}
	else
	{
		AfxMessageBox(A2T("NO DB Data Delete!"));
	}
}
void CCVTDBChannelEditDlg::OnEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult)
{
	USES_CONVERSION;
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	unsigned int pos = 0;
	int iItem = pDispInfo->item.iItem;
	int iSubItem = pDispInfo->item.iSubItem;
	CString str;
	switch (pDispInfo->item.iSubItem)
	{
	case CHANNEL_POS:
		str = pDispInfo->item.pszText;
		pos = _wtoi(pDispInfo->item.pszText);
		if ((pos >= pChannelVector->size()) || (pos <= 0) || !(str.SpanIncluding(_T("0123456789")) == str))
		{
			str.Format(L"Enter error please enter 1 to %d",pChannelVector->size());
			AfxMessageBox(str);
		}else{
			ClistCtrlDBData.SetItemState(pos - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			ClistCtrlDBData.SetItemState((*pChannelVector)[iItem].channelPos, 0, LVIS_SELECTED | LVIS_FOCUSED);
			pCVTDBUtil->ChangeChannnelPos((*pChannelVector)[iItem].channelPos, pos - 1);
			UpdateList(pChannelVector->size());
		}
		break;
	case CHANNEL_NAME:
		str = pDispInfo->item.pszText;
		if ((*pChannelVector)[iItem].channelType != TV_ATV_TYPE){
			if (strlen(T2A(str.GetBuffer())) > DTV_CHANNEL_NAME_BYTE_SIZE)
			{
				AfxMessageBox(A2T("Name is too long,edit fail!"));
			}
			else
			{
				MApp_TranslateUnicodeToCharTable((BYTE *)(*pChannelVector)[iItem].name, (WORD *)str.GetBuffer(), DTV_CHANNEL_NAME_BYTE_SIZE);
			}
		}
		else
		{
			if (strlen(T2A(str.GetBuffer())) > ATV_CHANNEL_NAME_BYTE_SIZE)
			{
				AfxMessageBox(A2T("Name is too long,edit fail!"));
			}
			else if (!CStringIsAscii(str))
			{
				AfxMessageBox(A2T("ATV Name should be Ascii"));
			}
			else
			{
				strcpy((*pChannelVector)[iItem].name, T2A(str));
			}
		}
		ClistCtrlDBData.RedrawSubitems(iItem, iItem, iSubItem);
		break;
	default:
		break;
	}
}
BOOL CCVTDBChannelEditDlg::CStringIsAscii(CString str)
{
	USES_CONVERSION;
	char * pStr = T2A(str.GetBuffer());
	int len = strlen(pStr);
	for (int i = 0; i < len; i++)
	{
		if (pStr[i] < 0 || pStr[i]>127)
		{
			return FALSE;
		}
	}
	return TRUE;
}