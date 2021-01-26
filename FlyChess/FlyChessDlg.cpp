// FlyChessDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "FlyChess.h"
#include "FlyChessDlg.h"
#include "afxdialogex.h"

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFlyChessDlg 对话框



CFlyChessDlg::CFlyChessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FLYCHESS_DIALOG, pParent)
{

	


	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFlyChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFlyChessDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_THROW_DICE, &CFlyChessDlg::OnBnClickedThrowDice)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_4P, &CFlyChessDlg::On4p)
	ON_COMMAND(ID_SET_BK, &CFlyChessDlg::OnSetBk)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_COMMAND(IDC_2P, &CFlyChessDlg::On2p)
	ON_COMMAND(IDC_3P, &CFlyChessDlg::On3p)
	ON_COMMAND(ID_MUSIC_SET, &CFlyChessDlg::OnMusicSet)
	ON_COMMAND(ID_MUSIC_CLOSE, &CFlyChessDlg::OnMusicClose)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CFlyChessDlg 消息处理程序

BOOL CFlyChessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	/*if (!VikeySafety())
		exit(0);*/

	
	CDC* pDC = GetDC();
	HDC hdc = pDC->m_hDC;//获得屏幕设备描述表句柄; 
	int iScrWidth = GetDeviceCaps(hdc, HORZRES);//获取屏幕水平分辨率; 
	int iScrWidth1 = GetDeviceCaps(hdc,HORZSIZE);//获取屏幕宽度
	int iScrHeight = GetDeviceCaps(hdc, VERTRES);//获取屏幕垂直分辨率; 
	int iScrHeight1 = GetDeviceCaps(hdc, VERTSIZE);//获取屏幕高度
	ReleaseDC(pDC);//释放屏幕设备描述表 
	MoveWindow(0.15 * iScrWidth, 0.1 * iScrHeight, (float)iScrWidth/ iScrWidth1 *(0.7 * iScrHeight1)*1.2, 0.7 * iScrHeight, TRUE);
	
	//初始化提示文本字体颜色大小
	m_NoticeColor = RGB(0, 250, 250);
	m_NoticeFont.CreatePointFont(100, L"宋体");


	SetDlgItemText(IDC_NOW_PlAYER, L"游戏状态提示栏");
	m_TimerFlag = 0;
	m_bkPath.Format(L"res/l.bmp");
	m_RbkPath.Format(L"res/F6F6F600.bmp");
	if (NULL == LoadImage(AfxGetInstanceHandle(), m_bkPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)
		||NULL == LoadImage(AfxGetInstanceHandle(), m_RbkPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE))
		MessageBox(L"背景图片加载失败!");
	m_musicPath.Format(L"res/bkmusic.wav"); 
	if(sndPlaySound(m_musicPath,SND_ASYNC|SND_LOOP)==FALSE)
		MessageBox(L"音乐加载失败!");

	GetDlgItem(IDC_THROW_DICE)->EnableWindow(FALSE);//设置色子按钮初始状态不可点击
	CRect rect;
	GetClientRect(rect);
	//传递rect给m_game
	m_game.SetBoardRect(rect);
	//初始化游戏
	m_game.Ini();
	//m_game.NewGame();//新游戏 
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CFlyChessDlg::OnPaint()
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
		//创建客户区
		CClientDC dc(this);
		//让游戏内核完成绘制工作
		m_game.OnDraw(&dc);
		RandDiceDraw(m_num);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFlyChessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFlyChessDlg::DrawNotice(CString text)
{
	CRect rtClient, rtText;
	GetClientRect(rtClient);
	rtText.SetRect(rtClient.left + rtClient.Width() * 0.82, rtClient.top + rtClient.Height() * 0.1,
		rtClient.left + rtClient.Width() * 0.97, rtClient.top + rtClient.Height() * 0.3);
	CClientDC dcClient(this);
	CDC memDC;//双缓冲用
	memDC.CreateCompatibleDC(&dcClient);
	HGDIOBJ hd = LoadImage(AfxGetInstanceHandle(), m_RbkPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (NULL == hd)
		return;
	CBitmap bmp;
	BITMAP bm;
	bmp.Attach(hd);
	bmp.GetBitmap(&bm);
	memDC.SelectObject(&bmp);
	dcClient.SetStretchBltMode(COLORONCOLOR);
		//计算缩放比例
	double dx = bm.bmWidth * 1.0 /( rtClient.Width()*0.2) ;
	double dy = bm.bmHeight * 1.0 / rtClient.Height();
	int x, y, width, height;
	x = (rtText.left -(rtClient.left+rtClient.Width()*0.8)) * dx;
	y = (rtText.top - rtClient.top) * dy;
	width = rtText.Width() * dx;
	height = rtText.Height() * dy;
	dcClient.StretchBlt(rtText.left, rtText.top, rtText.Width(), rtText.Height(),
		&memDC, x, y, width, height, SRCCOPY);
	CFont* pOldFont;
	pOldFont = dcClient.SelectObject(&m_NoticeFont);
	dcClient.SetBkMode(TRANSPARENT);
	dcClient.SetTextColor(m_NoticeColor);
	dcClient.DrawText(text, rtText,DT_CENTER);//| DT_VCENTER
	dcClient.SelectObject(pOldFont);
	memDC.DeleteDC();
	bmp.DeleteObject();

}

void CFlyChessDlg::OnBnClickedThrowDice()
{
	GetDlgItem(IDC_THROW_DICE)->EnableWindow(FALSE);//摇一次色子后设置按钮不可点击
	srand((unsigned int)time(0));
	if (m_TimerFlag == 1)//当前色子已经摇完
	{
		TCHAR* NowPlay[] = { L"红",L"黄",L"蓝",L"绿" };
		if (m_num != 6)
			if (m_game.HaveFly(m_num) == 0)
			{
				Sleep(1000);
				m_NoticeText.Format(L"%s色玩家掷骰子", NowPlay[m_game.GetNowColor()]);
				DrawNotice(m_NoticeText);
				m_flag = 0;
				GetDlgItem(IDC_THROW_DICE)->EnableWindow(TRUE);//摇一次色子后设置按钮不可点击
				m_TimerFlag = 0;//摇色子开始
				return; //进入下一个玩家环节
			}
		m_flag = 1;
		m_NoticeText.Format(L"%s色玩家选择棋子", NowPlay[m_game.GetNowColor()]);
		GetDlgItem(IDC_THROW_DICE)->EnableWindow(FALSE);//摇一次色子后设置按钮不可点击
		m_TimerFlag = 0;//设置色子可以开始摇
		return;
	}
	//色子开始摇
	SetTimer(1, 100, NULL); 
	m_TimerFlag = 0;//摇色子开始
}
void CFlyChessDlg::RandDiceDraw(int n)//摇色子动画
{
	if (n == 0)
		return;
	CRect DiceRect;
	GetDlgItem(IDC_PIC_DICE)->GetWindowRect(DiceRect);
	ScreenToClient(DiceRect);
	CClientDC dc(this);
	CDC memDC;//内存DC
	memDC.CreateCompatibleDC(&dc);
	CBitmap bmp;
	BITMAP bm;
	//画色子
	bmp.LoadBitmapW(IDB_DICE);
	bmp.GetBitmap(&bm);
	int nLenPic = bm.bmWidth/6;//图片中每个色子的边长
	//准备内存dc
	memDC.SelectObject(&bmp);
	int nOldMode=dc.SetStretchBltMode(COLORONCOLOR); //设置拉伸模式为颜色保真
	dc.TransparentBlt(DiceRect.left, DiceRect.top, DiceRect.Height(), DiceRect.Height(), &memDC, (n-1)* nLenPic, 0, nLenPic, nLenPic, RGB(200, 200, 200));
	dc.SetStretchBltMode(nOldMode); //还原拉伸模式
	memDC.DeleteDC(); //销毁内存DC
	//释放资源
	bmp.DeleteObject();
}

void CFlyChessDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_flag==0)//不可选择棋子
		return;
	Piece* pe = m_game.PointToPiece(point);
	if (pe==NULL)//选区无效，需要重选
		return;
	if (m_game.Proceed(pe, m_num) == 0)//前进未结束
		return;
	TCHAR* NowPlay[] = { L"红",L"黄",L"蓝",L"绿" };
	m_NoticeText.Format(L"%s色玩家掷色子", NowPlay[m_game.GetNowColor()]);
	DrawNotice(m_NoticeText);
	GetDlgItem(IDC_THROW_DICE)->EnableWindow(TRUE);//前进结束后色子为可摇状态
	m_flag = 0;//不可选择棋子
	m_game.ReDraw();
	if (m_game.GetState() == 1)//游戏结束
	{
		CString text[4];
		text[0].Format(L"红棋获胜!");
		text[1].Format(L"黄棋获胜!");
		text[2].Format(L"蓝棋获胜!");
		text[3].Format(L"绿棋获胜!");
		MessageBox(text[m_game.GetNowColor()],L"游戏结束!");
		GetDlgItem(IDC_THROW_DICE)->EnableWindow(FALSE);//前进结束后色子为不可摇状态
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}




void CFlyChessDlg::OnSetBk()
{
	CFileDialog dlg(TRUE, 0, 0, 6UL, L"位图(*.bmp)|*.bmp||");
	if (dlg.DoModal() != IDOK)
		return;
	m_bkPath = dlg.GetPathName();
	Invalidate(TRUE);
	// TODO: 在此添加命令处理程序代码
}


BOOL CFlyChessDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HGDIOBJ hd = LoadImage(AfxGetInstanceHandle(), m_bkPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HGDIOBJ hd1 = LoadImage(AfxGetInstanceHandle(), m_RbkPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hd == NULL||hd1==NULL) return CDialogEx::OnEraseBkgnd(pDC);//默认返回
	CBitmap bmp,bmp1;
	bmp.Attach(hd);bmp1.Attach(hd1);
	CRect rtClient;//客户区大小
	GetClientRect(rtClient);
	BITMAP bm,bm1;
	bmp.GetBitmap(&bm);bmp1.GetBitmap(&bm1);
	CDC memDC,memDC1;//内存DC
	memDC.CreateCompatibleDC(pDC), memDC1.CreateCompatibleDC(pDC);
	memDC.SelectObject(&bmp);memDC1.SelectObject(&bmp1);
	pDC->SetStretchBltMode(COLORONCOLOR);



	int x, y, width, height;
	int x1, y1, width1, height1;
	x = rtClient.left; y = rtClient.top; width = rtClient.Width() * 0.8, height = rtClient.Height();
	x1 = rtClient.left+width; y1 = y; width1 = rtClient.Width()*0.2, height1 = height;
	pDC->StretchBlt(x,y,width,height, &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	pDC->StretchBlt(x1, y1, width1, height1, &memDC1, 0, 0, bm1.bmWidth, bm1.bmHeight, SRCCOPY);
	memDC.DeleteDC();memDC1.DeleteDC();
	bmp.DeleteObject();bmp1.DeleteObject();
	return	TRUE;
}


void CFlyChessDlg::OnTimer(UINT_PTR nIDEvent)
{
	static int i = 0;
	switch (nIDEvent)
	{
	case 1:	 if(++i>10){
				i = 0;
				m_TimerFlag = 1;//表示摇色子已完成
				OnBnClickedThrowDice();
				KillTimer(1);
				break;
			}
			 m_num = rand() % 6 + 1; RandDiceDraw(m_num);// 不重画背景
			break;
	}
}


void CFlyChessDlg::On2p()
{
	m_num = 0;//色子为0，可以开始摇色子
	m_flag = 0;//设置不可选择棋子
	m_game.BeginGame(2);
	GetDlgItem(IDC_THROW_DICE)->EnableWindow(TRUE);
	m_NoticeText.Format(L"红色玩家掷骰子");
	DrawNotice(m_NoticeText);
}


void CFlyChessDlg::On3p()
{
	m_num = 0;//色子为0，游戏未开始
	m_flag = 0;//设置不可选择棋子
	m_game.BeginGame(3);
	GetDlgItem(IDC_THROW_DICE)->EnableWindow(TRUE);
	m_NoticeText.Format(L"红色玩家掷骰子");
	DrawNotice(m_NoticeText);
}
void CFlyChessDlg::On4p()
{
	m_num = 0;//色子为0，游戏未开始
	m_flag = 0;//设置不可选择棋子
	m_game.BeginGame(4);
	GetDlgItem(IDC_THROW_DICE)->EnableWindow(TRUE);
	m_NoticeText.Format(L"红色玩家掷骰子");
	DrawNotice(m_NoticeText);
}

void CFlyChessDlg::OnMusicSet()
{
	CFileDialog dlg(TRUE,0,0,6UL,L"音乐(*.wav)|*.wav||");
	if (dlg.DoModal() != IDOK)
		return;
	m_musicPath = dlg.GetPathName();
	if (NULL == sndPlaySound(NULL, SND_ASYNC))
	{
		MessageBox(L"音乐加载失败"); return;
	}
	sndPlaySound(m_musicPath, SND_ASYNC | SND_LOOP);
}


void CFlyChessDlg::OnMusicClose()
{
	sndPlaySound(NULL, SND_ASYNC);
}

void CFlyChessDlg::OnSize(UINT nType, int cx, int cy)//窗口大小改变事件处理函数
{
	CRect rect;
	CWnd* wnd=NULL;
	GetClientRect(rect);
	if( (wnd = GetDlgItem(IDC_NOW_PlAYER)) !=NULL)//设置提示信息按钮位置大小
		wnd->MoveWindow(rect.left + rect.Width() * 0.82, rect.top + rect.Height() * 0.1, rect.Width() * 0.15,rect.Height() * 0.2,FALSE);
	if ( (wnd = GetDlgItem(IDC_PIC_DICE) ) !=NULL)//设置色子位置大小
		wnd->MoveWindow(rect.left + rect.Width() * 0.82, rect.top + rect.Height() * 0.4, rect.Width() * 0.15, rect.Height() * 0.2, FALSE);
	if ((wnd = GetDlgItem(IDC_THROW_DICE)) != NULL)//设置摇色子按钮位置大小
		wnd->MoveWindow(rect.left + rect.Width() * 0.82, rect.top + rect.Height() * 0.7, rect.Width() * 0.15, rect.Height() * 0.2, FALSE);
	DrawNotice(m_NoticeText);
	m_game.SetBoardRect(rect);//游戏棋盘位置重设
	m_game.SetGridsPos();//游戏格子位置重设
	CDialogEx::OnSize(nType, cx, cy);
	Invalidate(TRUE);//重画整个游戏
}


void CFlyChessDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)//设置窗口最小最大尺寸
{
	CDC* pDC = GetDC();
	HDC hdc = pDC->m_hDC;//获得屏幕设备描述表句柄; 
	int iScrWidth = GetDeviceCaps(hdc, HORZRES);//获取屏幕水平分辨率; 
	int iScrHeight = GetDeviceCaps(hdc, VERTRES);//获取屏幕垂直分辨率; 
	int minx = iScrWidth * 0.55;
	int miny = iScrHeight * 0.5;
	if (lpMMI->ptMinTrackSize.x < minx)
		lpMMI->ptMinTrackSize.x = minx;
	if (lpMMI->ptMinTrackSize.y <= miny)
		lpMMI->ptMinTrackSize.y = miny;
	ReleaseDC(pDC);//释放屏幕设备描述表 

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

