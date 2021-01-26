
// FlyChessDlg.h: 头文件
//

#pragma once
#include "Game.h"	

// CFlyChessDlg 对话框
class CFlyChessDlg : public CDialogEx
{
// 构造
public:
	CFlyChessDlg(CWnd* pParent = nullptr);	// 标准构造函数
	Game m_game;//游戏内核对象

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FLYCHESS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	int m_num;//摇出的色子的数
	bool m_flag;//是否进入选择棋子阶段
	CString m_bkPath;//背景图片文件路径
	CString m_RbkPath;//右侧背景图片文件路径
	CString m_musicPath;//背景音乐文件路径
	int m_TimerFlag;//记录此次摇色子的次数
	CString m_NoticeText;//提示文本内容
	CFont m_NoticeFont;//提示文本字体
	COLORREF m_NoticeColor;//提示文本颜色
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void RandDiceDraw(int );//绘制色子动画
	DECLARE_MESSAGE_MAP()

public:
	void DrawNotice(CString notice);//绘制提示文本
public:
	afx_msg void OnBnClickedThrowDice();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void On4p();
	afx_msg void OnSetBk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void On2p();
	afx_msg void On3p();
	afx_msg void OnMusicSet();
	afx_msg void OnMusicClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
