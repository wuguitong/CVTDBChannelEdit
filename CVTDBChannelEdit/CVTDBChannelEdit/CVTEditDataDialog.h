#pragma once


// CVTEditDataDialog �Ի���

class CVTEditDataDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CVTEditDataDialog)

public:
	CVTEditDataDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVTEditDataDialog();

// �Ի�������
	enum { IDD = IDD_EDIT_DATA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
