#pragma once
#include "resource.h"
#include <afxadv.h>
#include "Piece.h"



class Game
{
private:
	int m_playerNum;//玩家数量
	CRect m_rtBoard;//用于存储整个棋盘位置
	Piece m_piece[PIECE_NUM];//所有棋子
	Grid m_grids[GRIDS_NUM];//所有格子
	int m_state;//-1游戏未开始,0游戏开始,1游戏结束
	int NowColor;//当前摇色子的玩家，red,yellow,blue,green,红黄蓝绿,为-1为未开始
public:
	Game();//构造函数
	bool HaveFly(int steps);//当前玩家是否有飞机起飞
	int Proceed(Piece* pe, int steps);//前进,前进的棋子,前进的步数,成功前进返回1,可重新摇色子返回0
	int GetState();//获取游戏状态
	int GetNowColor();//当前下棋颜色
	void BeginGame(int playerNum);//开始四人游戏
	void Ini();//初始化游戏地图数据
	void OnDraw(CDC* pDC);//绘制游戏
	void DrawGrids(CDC* pDC);//测试格子
	void DrawBackground(CDC* pDC);//绘制游戏背景
	void SetBoardRect(CRect& rect);//用于设置棋盘位置
	Piece* PointToPiece(CPoint point);
	void ReDraw();//通知主窗口重绘制
	void SetGridsPos();//设置格子位置
	//CRect GetBoardRect();//
protected:
	void DrawPieces(CDC *pDC);//用于绘制棋子
};

