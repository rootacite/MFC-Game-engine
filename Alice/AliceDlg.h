
// AliceDlg.h : 头文件
//

#pragma once
#include "afxext.h"
#include "gdiplusheaders.h"
#include "BtnST.h"
#include "afxwin.h"
using namespace std;

#define VEDIOMODE_BLOCK 0
#define VEDIOMODE_DIRECT 1

struct Alicedata {
	wchar_t text[100];
	int nSize;
	wchar_t name[15];
	wchar_t cg[100];
};

class CGalGameThesongofdestroyDlg : public CDialogEx
{
// 构造
public:
	CGalGameThesongofdestroyDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GALGAMETHESONGOFDESTROY_DIALOG };
#endif

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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnBnClickedButton1();
	CFont font;
	CFont fontc;
	LRESULT SetNameCall(WPARAM wp, LPARAM lp);
	LRESULT ShowNextTextCall(WPARAM wp, LPARAM lp);
	LRESULT ToMainCall(WPARAM wp, LPARAM lp);
	LRESULT ReTitleCall(WPARAM wp, LPARAM lp);
	LRESULT ShowLogo(WPARAM wp, LPARAM lp);
	LRESULT HideLogo(WPARAM wp, LPARAM lp);
	LRESULT WriteWhiteCall(WPARAM wp, LPARAM lp);
	LRESULT WriteFirst(WPARAM wp, LPARAM lp);
//	int PlayVedioEx(unsigned int playmode, unsigned int farmrate, LPSTR farmlist, int farmsize);
	void RefreshStatic();
	void ToMain();
	void ReTitle();
	void SetName(wchar_t *Name);
	void ShowNextText();
	long NowTextId = 0;
	//BITMAP m_start(_T("res\\开始.png"));
	//BITMAP m_start_on(L"res\\开始_按下.png");
	CImage image_l;
	LPWSTR m_Name;
	LPWSTR m_Text;
	CGalGameThesongofdestroyDlg *DlgPoint = this;
//	CBitmapButton m_back;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButtonST btn;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	CImage image;
	CImage image_s;
	CImage image_w;
	CImage image_f;
	CButtonST btne;
	afx_msg void OnBnClickedButton2();
	CButtonST btnt;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	CButtonST btns;
	CButtonST btnl;
	CButtonST btneu;
	afx_msg void OnBnClickedButton3();
};
