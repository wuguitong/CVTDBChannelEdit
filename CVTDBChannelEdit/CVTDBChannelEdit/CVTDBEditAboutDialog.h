#pragma once


// CVTDBEditAboutDialog �Ի���

class CVTDBEditAboutDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CVTDBEditAboutDialog)

public:
	CVTDBEditAboutDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVTDBEditAboutDialog();

// �Ի�������
	enum { IDD = IDD_ABOUT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
