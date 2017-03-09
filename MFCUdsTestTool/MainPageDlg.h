#pragma once
#include "ColoredListCtrl.h"
#include "ControlCAN.h"
#include "SendMsgDlg.h"
#include "ProductDlg.h"

// CCanCommDlg �Ի���

class CMainPageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainPageDlg)

public:
	CMainPageDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainPageDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CColoredListCtrl m_List;
	BOOL m_CanRxEn;

	CString m_Editprintstr;
	CEdit m_Editprint;


	CSendMsgDlg m_SendMsg;
	CProductDlg m_Product;

	CCriticalSection m_CriticalSection;

public:
	enum {
		DlgNone,
		DlgTxMsg,
	    DlgProduc};
public:
	afx_msg void OnBnClickedCheckRecv();
	afx_msg void OnBnClickedButtonClear();
	virtual BOOL OnInitDialog();
	// Insert a Can msg to list
	void InsertItem(INT Dire, VCI_CAN_OBJ * pCanObj);
	void PrintLog(UINT LogColor, CString LogContent);
	void SetOverlayDlg(UINT DlgIndex);
};
