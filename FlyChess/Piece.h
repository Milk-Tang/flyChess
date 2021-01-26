#pragma once

#define PIECE_NUM 16 //棋子的数量
#define GRIDS_NUM 96 //格子的数量,前0~15为飞机坪,16~19为起飞点
#define Playr_num 4 //玩家数量

class Piece;
class Grid;
class Game;

//格子类
class Grid
{
public:
	friend Game;
	Grid();
	void SetPos(int x,int y);
	void SetPos(POINT &pt);
	Piece* GetPiece();//获取在格子内的棋子,无棋子返回NULL
	bool IsInRegion(POINT pt);//判断点pt是否在格子区域，在区域返回true，否则返回false
private:
	POINT m_center;//格子位置
	Piece* m_pPiece;//格子上的棋子指针
};


//棋子类
class Piece
{
private:
	int m_color;//棋子的颜色,0,1,2,3分别为red, yellow, blue, green //红黄蓝绿
	int m_number;//棋子的位置，棋子在格子数组的编号
	int m_state;//棋子状态，-1为在飞机坪，0为飞行中，1为到达终点
public:
	friend Game;
	Piece();
	int GetState();//返回棋子状态
	static int raduis;//棋子半径
//	void BeginFly();//开始起飞,将飞机设置到起飞地点
	void Prceed();//前进
	void SetColor(int clr);//设置颜色
protected:
	void Draw(CDC *pDC,POINT pt);//画棋子
};