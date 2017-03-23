#pragma once
#include "ColoredListCtrl.h"
#include "ControlCAN.h"
#include "EditMsgDlg.h"

// CCanCommDlg 对话框
#define USE_TIMER

#define MSG_LEN      8
#define TIMOUT_XMIT  10
#define TIMOUT_TIM   200

typedef struct _CAN_MSG_T_
{
	UINT Id;
	BYTE Dlc;
	UINT Cycle;
	BYTE Data[MSG_LEN];
	UINT Count;
	UINT FrameNum;
	UINT LastTime;
	UINT TimeCnt;
	BOOL PreSet;
	BYTE PreData[MSG_LEN];
}CanMsg_t, *CanMsg_ptr;

class CMainPageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainPageDlg)

public:
	CMainPageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainPageDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CColoredListCtrl m_XmitList;
	CColoredListCtrl m_RecvList;
	CArray<CanMsg_t> m_XmitArray;
	BOOL m_XmitEnable;
	BOOL m_RcevEnable;
	BOOL m_XmitModif;
	BOOL m_SameId;
	UINT m_XmitRow;


	CCriticalSection m_CriticalSection;


	CString m_Editprintstr;
	CEdit m_Editprint;

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

	afx_msg void OnNMRClickListXmit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuAddmsg();
	afx_msg void OnMenuEditmsg();
	afx_msg void OnMenuDelmsg();
	afx_msg void OnMenuClrmsg();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void XmitMsgAdd();
	// Convert a EditMsg to CanMsg
	CanMsg_t MsgConvert(EditMsg_t EditMsg);
	static UINT XmitThread(void *param);
	afx_msg void OnNMDblclkListXmit(NMHDR *pNMHDR, LRESULT *pResult);
};
