
// AliceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Alice.h"
#include "AliceDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define UM_SETNAME 0x2560
#define UM_SHOWNEXT 0x2557
#define UM_MAIN 0x2596
#define UM_RETURN 0x2598
#define UM_WRITE 0x2569

#define UM_LOGO_HIDE 0x2123
#define UM_LOGO_SHOW 0x2589

#define UM_FIRST 0x5256

CImage img;
static HWND m_hwnd;//自身窗口句柄
bool inGame = false;//判断是否在游戏中，如果为false，则不会前进剧情
bool Onstart = false;//判断是否显示“引子”字样
int i = 1;
int OnstartTime = 0;//判断显示的时间
bool OnFirst;//同上
int OnFirstTime = 0;

struct SystemData {
	bool Readed[1024];
	bool isFinish;
};

SystemData GameData;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGalGameThesongofdestroyDlg 对话框



CGalGameThesongofdestroyDlg::CGalGameThesongofdestroyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GALGAMETHESONGOFDESTROY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGalGameThesongofdestroyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_BUTTON1, m_back);
	DDX_Control(pDX, IDC_BUTTON1, btn);
	DDX_Control(pDX, IDC_BUTTON2, btne);
	DDX_Control(pDX, IDC_BUTTON5, btnt);
	DDX_Control(pDX, IDC_SAVE, btns);
	DDX_Control(pDX, IDC_LOAD, btnl);
	DDX_Control(pDX, IDC_BUTTON3, btneu);
}

BEGIN_MESSAGE_MAP(CGalGameThesongofdestroyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CGalGameThesongofdestroyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGalGameThesongofdestroyDlg::OnBnClickedCancel)
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(UM_SETNAME, &CGalGameThesongofdestroyDlg::SetNameCall)
	ON_MESSAGE(UM_SHOWNEXT, &CGalGameThesongofdestroyDlg::ShowNextTextCall)
	ON_MESSAGE(UM_MAIN, &CGalGameThesongofdestroyDlg::ToMainCall)
	//ON_BN_CLICKED(IDC_BUTTON1, &CGalGameThesongofdestroyDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CGalGameThesongofdestroyDlg::OnBnClickedButton1)
	ON_WM_MOVE()
	ON_MESSAGE(UM_RETURN, &CGalGameThesongofdestroyDlg::ReTitleCall)
	ON_WM_MOVING()
	ON_WM_WINDOWPOSCHANGING()
	ON_BN_CLICKED(IDC_BUTTON2, &CGalGameThesongofdestroyDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CGalGameThesongofdestroyDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_SAVE, &CGalGameThesongofdestroyDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_LOAD, &CGalGameThesongofdestroyDlg::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON3, &CGalGameThesongofdestroyDlg::OnBnClickedButton3)
	ON_MESSAGE(UM_WRITE, &CGalGameThesongofdestroyDlg::WriteWhiteCall)
	ON_MESSAGE(UM_LOGO_HIDE, &CGalGameThesongofdestroyDlg::HideLogo)
	ON_MESSAGE(UM_LOGO_SHOW, &CGalGameThesongofdestroyDlg::ShowLogo)
	ON_MESSAGE(UM_FIRST, &CGalGameThesongofdestroyDlg::WriteFirst)
END_MESSAGE_MAP()

/*DWORD WINAPI playfarmthread(LPVOID lPvoid) {

}*/

int PlayVedioEx(HWND hWnd, unsigned int playmode, unsigned int farmrate, LPSTR farmlist, int farmsize, RECT rect) {
		HANDLE hFile = CreateFileA(farmlist, GENERIC_READ, NULL, NULL, OPEN_EXISTING, 0, 0);
		struct ListAdd {
			wchar_t Name[100];
			int size;
		};
//		CDC *pDC = GetDC(m_hwnd);
		ListAdd add;
		CImage farmimg;
		DWORD nSize;
		for (int i = 0; i < farmsize; i++) {
			ReadFile(hFile, &add, sizeof(ListAdd), &nSize, 0);
			if (!farmimg.IsNull())farmimg.Destroy();
			farmimg.Load(add.Name);
			farmimg.Draw(GetDC(hWnd), rect);
			Sleep(farmrate);
		}
	return 0;
}
// CGalGameThesongofdestroyDlg 消息处理程序
Alicedata AliceGetNowMessage(int id) {
	Alicedata data;
	DWORD nSize;
	HANDLE hData = CreateFile(L"data.alice", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(hData, id * sizeof(Alicedata), NULL, FILE_BEGIN);
	ReadFile(hData, &data, sizeof(Alicedata), &nSize, 0);
	CloseHandle(hData);
	return data;
}

void PreservationData() {
	HANDLE hData = CreateFile(L"system.dat", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD nSize;
	WriteFile(hData, &GameData, sizeof(SystemData), &nSize, 0);
	CloseHandle(hData);
}

LRESULT CGalGameThesongofdestroyDlg::SetNameCall(WPARAM wp, LPARAM lp) {//SetName的调用函数
	SetName((LPWSTR)wp);
	return 0;
}

LRESULT CGalGameThesongofdestroyDlg::ShowNextTextCall(WPARAM wp, LPARAM lp) {//ShowNextText的调用函数
	ShowNextText();
	return 0;
}

LRESULT CGalGameThesongofdestroyDlg::ToMainCall(WPARAM wp, LPARAM lp) {//ToMain的调用函数
	ToMain();
	
	return 0;
}

LRESULT CGalGameThesongofdestroyDlg::ReTitleCall(WPARAM wp, LPARAM lp) {//ReTitle的调用函数
	ReTitle();
	return 0;
}
LRESULT CGalGameThesongofdestroyDlg::ShowLogo(WPARAM wp, LPARAM lp) {//显示logo
	CClientDC dc(this);
	image_l.Draw(dc, 0, 0);
	return 0;
}

LRESULT CGalGameThesongofdestroyDlg::HideLogo(WPARAM wp, LPARAM lp) {//隐藏logo
	GetDlgItem(IDC_SAVE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_LOAD)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_SHOW);
	return 0;
}



LRESULT CGalGameThesongofdestroyDlg::WriteWhiteCall(WPARAM wp, LPARAM lp) {
	CClientDC dc(this);
	image_w.Draw(dc.m_hDC, 0, 0);
	return 0;
}

LRESULT CGalGameThesongofdestroyDlg::WriteFirst(WPARAM wp, LPARAM lp) {
	CClientDC dca(this);
	if (!image_f.IsNull()) image_f.Draw(dca.m_hDC, 13, 13);
	return 0;
}

void CGalGameThesongofdestroyDlg::RefreshStatic()
{
	
}


void CGalGameThesongofdestroyDlg::ToMain() {//表示进入游戏

	CDC *pDC = GetDC();
	CImage cgimg;
	cgimg.Load(AliceGetNowMessage(NowTextId).cg);

	cgimg.Draw(pDC->m_hDC, 0, 0);
	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SAVE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_LOAD)->ShowWindow(SW_SHOW);
	SetName(AliceGetNowMessage(0).name);
	inGame = true;
	OnstartTime = 40;
	
	Onstart = true;

	CClientDC DcA(this);
	CFont font;
	font.CreatePointFont(150, _T("宋体"));
	RECT rect;
	rect.bottom = 600;
	rect.top = 490;
	rect.right = 900;
	rect.left = 170;
	DcA.SetBkMode(TRANSPARENT);
	if (GameData.Readed[NowTextId]) {
		DcA.SetTextColor(RGB(0, 255, 255));
	}
	else {
		DcA.SetTextColor(RGB(255, 255, 255));//设置字体为黄色
		GameData.Readed[NowTextId - 1] = true;
	}
	DcA.SelectObject(&font);
	DcA.DrawText(AliceGetNowMessage(0).text, -1, &rect, 0);
	CFont fonto;
	fonto.CreatePointFont(180, _T("黑体"));
	rect.bottom = 485;
	rect.top = 445;
	rect.right = 900;
	rect.left = 115;
	DcA.SelectObject(&fonto);
	DcA.DrawText(m_Name, -1, &rect, 0);
	m_Text = AliceGetNowMessage(0).text;

	if (!image_s.IsNull()) image_s.Draw(DcA.m_hDC, 13, 13);
}


void CGalGameThesongofdestroyDlg::ReTitle() {//刷新界面
	/*	CFont font;*/
	//	font.CreateFontW(80, _T("黑体"));
	//Invalidate(TRUE);

	CClientDC DcA(this);
	CImage cgimg;
	cgimg.Load(AliceGetNowMessage(NowTextId).cg);
	cgimg.Draw(DcA.m_hDC, 0, 0);
	img.Draw(DcA.m_hDC, 0, 429);
	CFont font;
	font.CreatePointFont(180, _T("黑体"));
	RECT rect;
	rect.bottom = 485;
	rect.top = 445;
	rect.right = 900;
	rect.left = 115;
	DcA.SetBkMode(TRANSPARENT);
	if (GameData.Readed[NowTextId]) {
		DcA.SetTextColor(RGB(0, 255, 255));
	}
	else {
		DcA.SetTextColor(RGB(255, 255, 255));//设置字体为黄色
		GameData.Readed[NowTextId-1] = true;
	}
	DcA.SelectObject(&font);
	DcA.DrawText(AliceGetNowMessage(NowTextId).name, -1, &rect, 0);
	CFont fonto;
	fonto.CreatePointFont(150, _T("宋体"));
	rect.bottom = 600;
	rect.top = 490;
	rect.right = 900;
	rect.left = 170;
	DcA.SelectObject(&fonto);
	DcA.DrawText(AliceGetNowMessage(NowTextId).text, -1, &rect, 0);
	//Invalidate();
	//DcA.DrawText(_T("Heo MFC"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	//DcA.Detach();
	btn.SetBitmaps(IDB_BITMAP2, RGB(255, 255, 255));
	btn.DrawTransparent();
	btne.SetBitmaps(IDB_BITMAP5, RGB(255, 255, 255));
	btne.DrawTransparent();
	btnt.SetBitmaps(IDB_BITMAP6, RGB(255, 255, 255));
	btnt.DrawTransparent();
	btns.SetBitmaps(IDB_BITMAP7, RGB(255, 255, 255));
	btns.DrawTransparent();
	btnl.SetBitmaps(IDB_BITMAP8, RGB(255, 255, 255));
	btnl.DrawTransparent();
	btneu.SetBitmaps(IDB_BITMAP9, RGB(255, 255, 255));
	btneu.DrawTransparent();
	if (Onstart) {
		if (!image_s.IsNull()) image_s.Draw(DcA.m_hDC, 13, 13);
	}
	if (OnFirst) {
		CClientDC dca(this);
		if (!image_f.IsNull()) image_f.Draw(dca.m_hDC, 13, 13);
	}
}

void CGalGameThesongofdestroyDlg::SetName(wchar_t *Name) {//设置人名（不常用）
/*	CFont font;*/
//	font.CreateFontW(80, _T("黑体"));
	//Invalidate(TRUE);
	CClientDC DcA(this);
	CImage cgimg;
	cgimg.Load(AliceGetNowMessage(NowTextId).cg);
	RECT chrect;
	chrect.top = 0;
	chrect.left = 0;
	chrect.right = 1000;
	chrect.bottom = 600;
	cgimg.Draw(DcA.m_hDC, 0, 0);
	img.Draw(DcA.m_hDC, 0, 429);
	CFont font;
	font.CreatePointFont(180, _T("黑体"));
	RECT rect;
	rect.bottom = 485;
	rect.top = 445;
	rect.right = 900;
	rect.left = 115;
	DcA.SetBkMode(TRANSPARENT);
	if (GameData.Readed[NowTextId]) {
		DcA.SetTextColor(RGB(0, 255, 255));
	}
	else {
		DcA.SetTextColor(RGB(255, 255, 255));//设置字体为黄色
		GameData.Readed[NowTextId-1] = true;
	}
	DcA.SelectObject(&font);
	DcA.DrawText(Name, -1, &rect, 0);
	CFont fonto;
	fonto.CreatePointFont(150, _T("宋体"));
	rect.bottom = 600;
	rect.top = 490;
	rect.right = 900;
	rect.left = 170;
	DcA.SelectObject(&fonto);
	if (NowTextId != -1) {
		DcA.DrawText(AliceGetNowMessage(NowTextId).text, -1, &rect, 0);
	}
	m_Name = Name;
	//Invalidate();
	//DcA.DrawText(_T("Heo MFC"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	//DcA.Detach();
	btn.SetBitmaps(IDB_BITMAP2, RGB(255, 255, 255));
	btn.DrawTransparent();
	btne.SetBitmaps(IDB_BITMAP5, RGB(255, 255, 255));
	btne.DrawTransparent();
	btnt.SetBitmaps(IDB_BITMAP6, RGB(255, 255, 255));
	btnt.DrawTransparent();
	btns.SetBitmaps(IDB_BITMAP7, RGB(255, 255, 255));
	btns.DrawTransparent();
	btnl.SetBitmaps(IDB_BITMAP8, RGB(255, 255, 255));
	btnl.DrawTransparent();
	btneu.SetBitmaps(IDB_BITMAP9, RGB(255, 255, 255));
	btneu.DrawTransparent();
	if (Onstart) {
		CClientDC dc(this);
		if (!image_s.IsNull()) image_s.Draw(dc.m_hDC, 13, 13);
	}
	if (OnFirst) {
		CClientDC dca(this);
		if (!image_f.IsNull()) image_f.Draw(dca.m_hDC, 13, 13);
	}
}

DWORD WINAPI ToMainWen(LPVOID lPvoid) {//界面逐渐变白特效
	Sleep(1800);
	int i = 0;
	while (i < 50) {
		SendMessage(m_hwnd, UM_WRITE, 0, 0);
		Sleep(80);
		i++;
	}
	Sleep(1300);
	SendMessage(m_hwnd, UM_LOGO_SHOW, 0, 0);
	Sleep(5000);
	SendMessage(m_hwnd, UM_LOGO_HIDE, 0, 0);
//	SendMessage(m_hwnd, UM_RETURN, 0, 0);
	SendMessage(m_hwnd, UM_SHOWNEXT, 0, 0);

	inGame = true;
	OnFirstTime = 40;

	OnFirst = true;
	SendMessage(m_hwnd, UM_FIRST, 0, 0);
	return 0;
}

void CGalGameThesongofdestroyDlg::ShowNextText() {//显示下一文本
	POINT p;
	GetCursorPos(&p);
	CWnd *hwnd = WindowFromPoint(p);
	if (hwnd == GetDlgItem(IDC_SAVE)) {
		return;
	}
	if (hwnd == GetDlgItem(IDC_LOAD)) {
		return;
	}
	if (hwnd == GetDlgItem(IDC_BUTTON5)) {
		return;
	}
	if (hwnd == GetDlgItem(IDC_BUTTON2)) {
		return;
	}
	NowTextId++;
	CClientDC DcA(this);
	CImage cgimg;
//	MessageBox( AliceGetNowMessage(NowTextId).cg);
	cgimg.Load(AliceGetNowMessage(NowTextId).cg);
	cgimg.Draw(DcA.m_hDC, 0, 0);
	img.Draw(DcA.m_hDC, 0, 429);

	CFont font;
	font.CreatePointFont(150, _T("宋体"));
	RECT rect;
	rect.bottom = 600;
	rect.top = 490;
	rect.right = 900;
	rect.left = 170;
	DcA.SetBkMode(TRANSPARENT);
	if (GameData.Readed[NowTextId]) {
		DcA.SetTextColor(RGB(0, 255, 255));
	}
	else {
		DcA.SetTextColor(RGB(255, 255, 255));//设置字体为黄色
		GameData.Readed[NowTextId-1] = true;
	}
	DcA.SelectObject(&font);
	DcA.DrawText(AliceGetNowMessage(NowTextId).text, -1, &rect, 0);
	CFont fonto;
	fonto.CreatePointFont(180, _T("黑体"));
	rect.bottom = 485;
	rect.top = 445;
	rect.right = 900;
	rect.left = 115;
	DcA.SelectObject(&fonto);
	DcA.DrawText(AliceGetNowMessage(NowTextId).name, -1, &rect, 0);
	m_Text = AliceGetNowMessage(NowTextId).text;
	m_Name = AliceGetNowMessage(NowTextId).name;
	btn.SetBitmaps(IDB_BITMAP2, RGB(255, 255, 255));
	btn.DrawTransparent();
	btne.SetBitmaps(IDB_BITMAP5, RGB(255, 255, 255));
	btne.DrawTransparent();
	btnt.SetBitmaps(IDB_BITMAP6, RGB(255, 255, 255));
	btnt.DrawTransparent();
	btns.SetBitmaps(IDB_BITMAP7, RGB(255, 255, 255));
	btns.DrawTransparent();
	btnl.SetBitmaps(IDB_BITMAP8, RGB(255, 255, 255));
	btnl.DrawTransparent();
	btneu.SetBitmaps(IDB_BITMAP9, RGB(255, 255, 255));
	btneu.DrawTransparent();
	if (Onstart) {
		CClientDC dc(this);
		if (!image_s.IsNull()) image_s.Draw(dc.m_hDC, 13, 13);
	}
	if (OnFirst) {
		CClientDC dca(this);
		if (!image_f.IsNull()) image_f.Draw(dca.m_hDC, 13, 13);
	}
	if (NowTextId == 18) {
		inGame = false;
		GetDlgItem(IDC_SAVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LOAD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_HIDE);
		ReTitle();
		CreateThread(0, 0, ToMainWen, 0, 0, 0);
	}
}
HHOOK Mouse;
LRESULT CALLBACK MouseProc(int nCode, WPARAM wp, LPARAM lp) {//鼠标钩子
	//MessageBoxA(0,"","",0);
	if ((wp == WM_LBUTTONDOWN)&&inGame) {
		SendMessage(m_hwnd, UM_SHOWNEXT, 0, 0);
	}
	
	return CallNextHookEx(Mouse, nCode, wp, lp);
}

DWORD WINAPI ReWriteEx(LPVOID lPvoid){
	Sleep(100);

	if (inGame)SendMessage(m_hwnd, UM_RETURN, 0, 0);
	return 0;
}

HHOOK MouseLL;
LRESULT CALLBACK MouseProcLL(int nCode, WPARAM wp, LPARAM lp) {
	if ((wp == WM_LBUTTONDOWN) && inGame) {
		POINT p;
		GetCursorPos(&p);
		HWND hwnd;
		hwnd = WindowFromPoint(p);
		if (hwnd != m_hwnd) {
			CreateThread(0, 0, ReWriteEx, 0, 0, 0);
		}
	}
	return CallNextHookEx(Mouse, nCode, wp, lp);
}

HHOOK keyboard;
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wp, LPARAM lp) {//键盘钩子
/*	PKBDLLHOOKSTRUCT KeyClass = (PKBDLLHOOKSTRUCT)lp;
	if (!inGame) {
		return CallNextHookEx(keyboard, nCode, wp, lp);
	}
	switch (wp) {
	case WM_KEYDOWN:
		if (KeyClass->vkCode == 'S') {
			SendMessage(m_hwnd, UM_SHOWNEXT, 0, 0);
		}
		break;
	case WM_KEYUP:
		break;
	default:
		break;
	}*/
	return CallNextHookEx(keyboard, nCode, wp, lp);
}

void TransparentPNG(CImage *png)//使PNG变透明
{
	for (int i = 0; i <png->GetWidth(); i++)  				//遍历像素处理
	{
		for (int j = 0; j <png->GetHeight(); j++)
		{
			unsigned char* pucColor = reinterpret_cast<unsigned char*>(png->GetPixelAddress(i, j));
			pucColor[0] = pucColor[0] * pucColor[3] / 255;
			pucColor[1] = pucColor[1] * pucColor[3] / 255;
			pucColor[2] = pucColor[2] * pucColor[3] / 255;
		}
	}
}

DWORD WINAPI ToLogo(LPVOID lPvoid) {
	Sleep(3500);
	ShowWindow(GetDlgItem(m_hwnd, IDC_STATICLOGO), SW_HIDE);
	ShowWindow(GetDlgItem(m_hwnd, IDC_BUTTON1), SW_SHOW);
	ShowWindow(GetDlgItem(m_hwnd, IDC_BUTTON3), SW_SHOW);
	return 0;
}

BOOL CGalGameThesongofdestroyDlg::OnInitDialog()
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

	HANDLE hData = CreateFile(L"system.dat", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hData == INVALID_HANDLE_VALUE) {
		CloseHandle(hData);
		hData = CreateFile(L"system.dat", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hData == INVALID_HANDLE_VALUE) {
			MessageBoxA(m_hWnd, "游戏出错！system.dat文件无法创建！", "错误", MB_ICONERROR);
			CloseHandle(hData);
			exit(-1);
		}
		GameData.isFinish = false;
		for (int i = 0; i < sizeof(GameData.Readed); i++) {
			GameData.Readed[i] = false;
		}
		DWORD nSize;
		WriteFile(hData, &GameData, sizeof(SystemData), &nSize, 0);
		CloseHandle(hData);
	}
	else {
		DWORD nSize;
		ReadFile(hData, &GameData, sizeof(SystemData), &nSize, 0);
		CloseHandle(hData);
	}

	Mouse = SetWindowsHookEx(WH_MOUSE, MouseProc, 0, GetCurrentThreadId());
	MouseLL = SetWindowsHookEx(WH_MOUSE_LL, MouseProcLL, GetModuleHandle(0),0);
	keyboard= SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, GetModuleHandle(0), 0);

	m_hwnd = m_hWnd;
//	image.Load(_T("exit.png"));
//	image.GetBits();

	// TODO: 在此添加额外的初始化代码
	btn.SetBitmaps(IDB_BITMAP2, RGB(255, 255, 255));
	btn.DrawTransparent();
	btne.SetBitmaps(IDB_BITMAP5, RGB(255, 255, 255));
	btne.DrawTransparent();
	btnt.SetBitmaps(IDB_BITMAP6, RGB(255, 255, 255));
	btnt.DrawTransparent();
	btns.SetBitmaps(IDB_BITMAP7, RGB(255, 255, 255));
	btns.DrawTransparent();
	btnl.SetBitmaps(IDB_BITMAP8, RGB(255, 255, 255));
	btnl.DrawTransparent();
	btneu.SetBitmaps(IDB_BITMAP9, RGB(255, 255, 255));
	btneu.DrawTransparent();
	SetTimer(1, 80, NULL); 
	CPaintDC dc(this);


	if (i == 1) {
		i--;
		CreateThread(0, 0, ToLogo, 0, 0, 0);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void RemoveFocus() {//表示去除所有焦点
	SetForegroundWindow(0);
}

void CGalGameThesongofdestroyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if (nID == SC_MINIMIZE) {
			RemoveFocus();
		}
		if (nID == SC_CLOSE) {
			PreservationData();
		}
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGalGameThesongofdestroyDlg::OnPaint()
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
		CPaintDC dc(this);
		/*if (!img.IsNull()) img.Destroy();
		img.Load(_T("res\\Text.dcx"));                          
		TransparentPNG(&img);                            
		if (!img.IsNull()) img.Draw(dc.m_hDC, 0, 400); 
		CDialogEx::OnPaint();

		SetName(_T("学生"));
		//ShowNextText();
//		CPaintDC dc(this);*/
		if (!image_s.IsNull()) image_s.Destroy();
		image_s.Load(_T("res\\Cate.dcx"));
		TransparentPNG(&image_s);

		if (!image_w.IsNull()) image_w.Destroy();
		image_w.Load(_T("res\\White.dcx"));
		TransparentPNG(&image_w);

		if (!image_f.IsNull()) image_f.Destroy();
		image_f.Load(_T("res\\First.dcx"));
		TransparentPNG(&image_f);

		if (!image_l.IsNull()) image_l.Destroy();
		image_l.Load(_T("res\\Logo.dcx"));
	//	TransparentPNG(&image_l);
	//	Sleep(3000);
		
	//	MessageBox(_T(""));
		CImage startimg;
		startimg.Load(L"res\\开始界面.bmp");
		startimg.Draw(dc, 0, 0);
	}
	if (inGame)ReTitle();
	CDialogEx::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGalGameThesongofdestroyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGalGameThesongofdestroyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void CGalGameThesongofdestroyDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
	inGame = false;
	if (MessageBoxA(m_hWnd, "真的要退出吗？", "确定", MB_ICONQUESTION | MB_YESNO) == 6)
	{
		PreservationData();
		exit(1);
	}
	else {
		inGame = true;
	}
}


BOOL CGalGameThesongofdestroyDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnEraseBkgnd(pDC);
	/*HBITMAP m_hBitmap;
	HDC m_hBkDC;
	m_hBitmap  = ::LoadBitmap(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP4));
	m_hBkDC = ::CreateCompatibleDC(pDC->m_hDC);
	if (m_hBitmap && m_hBkDC)
	{
		TransparentBlt(pDC->m_hDC, 0, 0, 1000, 600, pDC->m_hDC, 0, 0, 1000, 600, RGB(255, 0, 255));//RGB(0,0,0xff)改成你的背景颜色
		::SelectObject(m_hBkDC, m_hBitmap);
		::StretchBlt(pDC->m_hDC, 0, 0, 1000, 600, m_hBkDC, 0, 0, 1000, 600, SRCCOPY);
		::DeleteObject(m_hBitmap);
		::DeleteDC(m_hBkDC);
	}*/

	//if(inGame)ReTitle();
	return TRUE;
}


LRESULT CGalGameThesongofdestroyDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int ret = CDialog::OnNcHitTest(point);

	if (HTTOP == ret || HTBOTTOM == ret || HTLEFT == ret || HTRIGHT == ret
		|| HTBOTTOMLEFT == ret || HTBOTTOMRIGHT == ret || HTTOPLEFT == ret)
		return HTCLIENT;

	return ret;
	//return CDialogEx::OnNcHitTest(point);
}


HBRUSH CGalGameThesongofdestroyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);//设置背景透明 
		pDC->SetTextColor(RGB(255, 255, 255));//设置字体为黄色 
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CGalGameThesongofdestroyDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if(inGame)ReTitle();

	if (Onstart) {
		OnstartTime--;
		if (OnstartTime == 0) {
			Onstart = false;
			ReTitle();
		}
	}
	if (OnFirst) {
		OnFirstTime--;
		if (OnFirstTime == 0) {
			OnFirst = false;
			ReTitle();
		}
	}
	if (GetKeyState(163)< 0 &&inGame) {
		SendMessage(UM_SHOWNEXT, 0, 0);
	}
	if (GetKeyState(162)< 0 && inGame) {
		SendMessage(UM_SHOWNEXT, 0, 0);
	}
	CDialogEx::OnTimer(nIDEvent);
}

DWORD WINAPI MainCallThread(LPVOID lPvoid) {
	RECT rect;
	GetClientRect(m_hwnd, &rect);
	//UnhookWindowsHookEx(MouseLL);

	EnableWindow(GetDlgItem(m_hwnd, IDC_BUTTON1), FALSE);
	EnableWindow(GetDlgItem(m_hwnd, IDC_BUTTON3), FALSE);
	int i = 0;
	while (i < 50) {
		SendMessage(m_hwnd, UM_WRITE, 0, 0);
		Sleep(80);
		i++;
	}
	EnableWindow(GetDlgItem(m_hwnd, IDC_BUTTON1), FALSE);
	EnableWindow(GetDlgItem(m_hwnd, IDC_BUTTON3), FALSE);

	//MouseLL = SetWindowsHookEx(WH_MOUSE_LL, MouseProcLL, GetModuleHandle(0), 0);

	if (GameData.Readed[0] == 0)
		PlayVedioEx(m_hwnd
			, VEDIOMODE_BLOCK, 65, "opfarmlist.fal", 55, rect);

	if (!img.IsNull()) img.Destroy();
	img.Load(_T("res\\Text.dcx"));
	TransparentPNG(&img);
	if (!img.IsNull()) img.Draw(GetDC(m_hwnd), 0, 400);

	SendMessage(m_hwnd, UM_MAIN, 0, 0);
	return 0;
}

void CGalGameThesongofdestroyDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//WaitForSingleObject();
	CreateThread(0, 0, MainCallThread, 0, 0, 0);
}


void CGalGameThesongofdestroyDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码
}


void CGalGameThesongofdestroyDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnMoving(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
}


void CGalGameThesongofdestroyDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	int nMaxX = GetSystemMetrics(SM_CXSCREEN);
	int nMaxY = GetSystemMetrics(SM_CYSCREEN);

	// 判断窗口X坐标有无超过左边桌面。
	if (lpwndpos->x < 0)
	{
		lpwndpos->x = -6;
	}

	// 判断窗口X坐标有无超过右边桌面。
	if (lpwndpos->x + lpwndpos->cx > nMaxX)
	{
		lpwndpos->x  = nMaxX - lpwndpos->cx+6;
	}

	// 判断窗口Y坐标有无超过顶部桌面。
	if (lpwndpos->y<0)
	{
		lpwndpos->y = 0;
	}

	// 判断窗口Y坐标有无超过底部桌面。
	if (lpwndpos->y + lpwndpos->cy > nMaxY)
	{
		lpwndpos->y = nMaxY - lpwndpos->cy+6;
	}

	CDialog::OnWindowPosChanging(lpwndpos);

	return;
	// TODO: 在此处添加消息处理程序代码
}


void CGalGameThesongofdestroyDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	inGame = false;
	if (MessageBoxA(m_hWnd, "真的要退出吗？", "确定", MB_ICONQUESTION|MB_YESNO) == 6) 
	{
		PreservationData();
		exit(1);
	}
	else 
	{
		inGame = true;
	}
}


void CGalGameThesongofdestroyDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//获取exe程序当前路径
	/*extern CGalGameThesongofdestroyApp theApp;

	TCHAR szAppName[MAX_PATH];
	::GetModuleFileName(theApp.m_hInstance, szAppName, MAX_PATH);

	CString strAppFullName;
	strAppFullName.Format(_T("%s"), szAppName);

	//重启程序	
	STARTUPINFO StartInfo;
	PROCESS_INFORMATION procStruct;
	memset(&StartInfo, 0, sizeof(STARTUPINFO));
	StartInfo.cb = sizeof(STARTUPINFO);
	::CreateProcess(
		(LPCTSTR)strAppFullName,
		NULL,
		NULL,
		NULL,
		FALSE,
		NORMAL_PRIORITY_CLASS,
		NULL,
		NULL,
		&StartInfo,
		&procStruct);
	TerminateProcess(GetCurrentProcess(), 1);*/
	inGame = false;
	if (MessageBoxA(m_hWnd, "真的要返回标题吗？", "确定", MB_ICONQUESTION | MB_YESNO) == 6)
	{
		NowTextId = 0;
		Onstart = false;
		GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SAVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LOAD)->ShowWindow(SW_HIDE);
		CDC *pDC = GetDC();
		HBITMAP m_hBitmap;
		HDC m_hBkDC;
		m_hBitmap = ::LoadBitmap(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP4));
		m_hBkDC = ::CreateCompatibleDC(pDC->m_hDC);
		if (m_hBitmap && m_hBkDC)
		{
			TransparentBlt(pDC->m_hDC, 0, 0, 1000, 600, pDC->m_hDC, 0, 0, 1000, 600, RGB(255, 0, 255));//RGB(0,0,0xff)改成你的背景颜色
			::SelectObject(m_hBkDC, m_hBitmap);
			::StretchBlt(pDC->m_hDC, 0, 0, 1000, 600, m_hBkDC, 0, 0, 1000, 600, SRCCOPY);
			::DeleteObject(m_hBitmap);
			::DeleteDC(m_hBkDC);
		}
	}
	else
	{
		inGame = true;
	}
}



void CGalGameThesongofdestroyDlg::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码
	inGame = false;
	struct DataFormat {
		int NowSound;
		wchar_t NowCG[15];
		char flag;
	};
	CFileDialog dlg(FALSE, NULL, NULL, NULL, _T("存档文件 (*.data)|*.data||"), NULL);
	dlg.m_ofn.lpstrTitle = _T("存档");
	CString szFileName;
	CString ext;
	if (dlg.DoModal() == IDOK)
	{
		inGame = true;
		szFileName = dlg.GetPathName();//文件名
		ext = dlg.GetFileExt();//取得扩展名
		ext.MakeLower();//转为小写
	}
	else {
		inGame = true;
		return;
	}
	szFileName += ".data";
	HANDLE SaveHandle = CreateFile(szFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	DataFormat Data;
	DWORD nSize;
	Data.NowSound = NowTextId;
	memcpy(Data.NowCG, AliceGetNowMessage(NowTextId).cg, 15);
	Data.flag = 0;
	WriteFile(SaveHandle, (char*)&Data, sizeof(DataFormat), &nSize, 0);
	CloseHandle(SaveHandle);
}


void CGalGameThesongofdestroyDlg::OnBnClickedLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	inGame = false;
	struct DataFormat {
		int NowSound;
		int NowCG;
		char flag;
	};
	CFileDialog dlg(TRUE, NULL, NULL, NULL, _T("存档文件 (*.data)|*.data||"), NULL);
	dlg.m_ofn.lpstrTitle = _T("读档");
	CString szFileName;
	CString ext;
	if (dlg.DoModal() == IDOK)
	{
		inGame = true;
		szFileName = dlg.GetPathName();//文件名
		ext = dlg.GetFileExt();//取得扩展名
		ext.MakeLower();//转为小写
	}
	else {
		inGame = true;
		return;
	}
	HANDLE SaveHandle = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	char Buf[sizeof(DataFormat)];
	DataFormat Data;
	DWORD nSize;
	ReadFile(SaveHandle, Buf, sizeof(DataFormat), &nSize, 0);
	memcpy(&Data, Buf, sizeof(DataFormat));
	itoa(Data.NowSound, Buf, 10);
//	MessageBoxA(0, Buf, "", 0);
	NowTextId = Data.NowSound - 1;
	SendMessage(UM_SHOWNEXT, 0, 0);
	CloseHandle(SaveHandle);
}


void CGalGameThesongofdestroyDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	PreservationData();
	exit(1);
}
