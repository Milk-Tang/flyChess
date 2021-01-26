#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	NowColor = 0;//红子先走
	m_state = -1;//游戏未开始
	for (int i = 0; i < 4; ++i)//设置棋子颜色，初始位置
		for (int j = i * 4; j < (i + 1) * 4; ++j)
		{
			m_piece[4 * i + j].SetColor(i);
			m_piece[4 * i + j].m_number = 5 * i + j;
		}
}

bool Game::HaveFly(int steps)//判断当前玩家摇出色子后是否可以选择棋子前进
{
	int i ;
	if (steps == 6)
		return true;
	for (i = NowColor * 4; i < (1 + NowColor) * 4; ++i)
		if (m_piece[i].m_state == 0)
			return true;
	NowColor = (++NowColor) % m_playerNum;//此玩家无飞机起飞，进入下一个玩家环节
	return false;
		
}

int Game::Proceed(Piece* pe, int steps)
{
	int i, m;
	int OldNumber = pe->m_number;
	if (pe->m_state == 1)//飞机已经到达终点，当前下棋人不变
		return 0;
	else if (pe->m_state == -1)
	{
		if (steps == 6)
		{
			pe->m_state = 0;
			pe->m_number = pe->m_color * 5 + 4;//调整飞机位置到起飞点
			return 1;//飞机成功前进，不改变当前下棋人
		}
		return 0;//选择的棋子未起飞且未摇到6，当前下棋人不变
	}
	else
	{
		if (pe->m_number >=21+ pe->m_color * 6&&pe->m_number<=25+pe->m_color//在终点线上
			&&pe->m_number-steps<20+pe->m_color*6)//且需要倒退s
			pe->m_number=(20+pe->m_color*6)*2-(pe->m_number-steps);//在终点线倒退
		for (i = 0; i < steps; ++i)//一步步前进
		{
			pe->Prceed();
			ReDraw();
		}
		//单步走完
		m = pe->m_number;
		if ((m - 44) % 4 == pe->m_color && m > 44&& pe->m_number != 64 + 13 * pe->m_color)//走到同色格子切格子在主飞行圈内，且不在超级加速点
		{
			for(i=0;i<4;++i)
				pe->Prceed();
		}
		else if (pe->m_number == 64 + 13 * pe->m_color)//棋子到达超强加速点
			pe->m_number += 12;//超强加速,其他颜色的棋子在超强加速点是不会被打回的
		for (i = pe->m_color * 4; i < (pe->m_color + 1) * 4; ++i)
			if (m_piece[i].m_number == OldNumber)//判断是否有叠背飞机
			{
				Proceed(&m_piece[i], steps);
				if (steps != 6)
					--NowColor;
			}
		if (m == pe->m_color * 6 + 20)//棋子落到终点
		{
			pe->m_state = 1;
			//判断游戏是否结束
			m_state = 1;
			for (i = pe->m_color * 4; i < (pe->m_color + 1) * 4; ++i)
			{
				if (m_piece[i].m_state != 1)
				{
					m_state = 0;//游戏未结束
					break;
				}
			}
			if(m_state==1)//游戏结束
				return 1;
		}
		if (m_grids[m].m_pPiece != NULL)//当前位置有棋子
		{
			if (m_grids[m].m_pPiece->m_color != pe->m_color)//非同色棋子，打回该处棋子到飞机坪
			{
				for (i = m_grids[m].m_pPiece->m_color * 4; i < (m_grids[m].m_pPiece->m_color + 1) * 4; ++i)
					if (m == m_piece[i].m_number)
					{
						m_piece[i].m_number = i + m_piece[i].m_color;
						m_piece[i].m_state = -1;
					}
			}
		}
		m_grids[m].m_pPiece = pe; //新到的棋子叠在格子最上方
		//m_game.ReDraw();//重绘游戏
		if (steps == 6)//可以再摇一次色子
			return 1; //当前下棋人不变
		else
		{
			NowColor = (++NowColor) % m_playerNum;//下棋人改变
			return 1;
		}
	}
}
int Game::GetState()
{
	return m_state;
}

int Game::GetNowColor()
{
	return NowColor;
}

void Game::BeginGame(int playerNum)
{
	NowColor = 0;//红色最先开始掷骰子
	m_state = 0;//游戏开始
	m_playerNum = playerNum;
	int i;
	for (i = 0; i < PIECE_NUM; ++i)
	{
		//初始化棋子
		m_piece[i].m_state = -1;
		m_piece[i].m_number = i + m_piece[i].m_color;
		m_piece[i].m_state = -1;
		//初始化格子
		m_grids[m_piece[i].m_number].m_pPiece = NULL;
	}
	ReDraw();//重绘游戏
}


void Game::SetGridsPos()
{
	int i, j;
	float nLenth = ((float)m_rtBoard.Width()) / 824;//图片中每个像素距在控件中的长度
	//棋盘中心点
	POINT BoardCenter;
	BoardCenter.x = m_rtBoard.left + nLenth * 413;
	BoardCenter.y = m_rtBoard.top + nLenth * 413;
	Piece::raduis = 20 * nLenth;
	float GridLen = 97 * nLenth;//主地图上一个格子在程序中的长度
	float GridWidth = 47.5 * nLenth;//主地图上一个格子在程序中的宽度
	//设置红格子飞机坪初始位置
	POINT pt[GRIDS_NUM];//中心点右下方四分之一区域内格子与中心点的横纵坐标距离
	//0~4
	pt[2].x = pt[1].y = pt[0].x = pt[0].y = 271 * nLenth;
	pt[2].y = pt[1].x = pt[3].x = pt[3].y = 348 * nLenth;
	pt[4].x = 373 * nLenth;
	pt[4].y = 198 * nLenth;
	//HOME/20
	pt[20].x = 44 * nLenth;
	pt[20].y = 0;
	//21~25
	for (i = 21; i < 26; ++i)
	{
		pt[i].x = (i - 21) * GridWidth + nLenth * 101;
		pt[i].y = 0;
	}
	//44~46
	for (i = 44; i < 47; ++i)
	{
		pt[i].x = 361 * nLenth;
		pt[i].y = (i - 44) * GridWidth;
	}
	//47
	pt[i].x = 337 * nLenth;
	pt[i].y = 150 * nLenth;
	++i;
	//48~49
	for (; i < 50; ++i)
	{
		pt[i].x = 289 * nLenth - GridWidth * (i - 48);
		pt[i].y = 169 * nLenth;
	}
	//50
	pt[i].x = 185 * nLenth;
	pt[i].y = 150 * nLenth;
	++i;
	//51
	pt[i].x = 142 * nLenth;
	pt[i].y = 178 * nLenth;
	++i;
	//52~53
	for (; i < 54; ++i)
	{
		pt[i].x = 166 * nLenth;
		pt[i].y = (i - 52) * GridWidth + 241 * nLenth;
	}
	//54
	pt[i].x = 147 * nLenth;
	pt[i].y = 339 * nLenth;
	++i;
	//55~56
	for (; i < 57; ++i)
	{
		pt[i].x = 94 * nLenth - (i - 55) * GridWidth;
		pt[i].y = 361 * nLenth;
	}


	//设置所有格子的坐标
	for (i = 0; i < GRIDS_NUM; ++i)
		m_grids[i].SetPos(pt[i].x + BoardCenter.x, pt[i].y + BoardCenter.y);

	int m;
	for (i = 0; i < 3; ++i)//将点坐标旋转,由24个点扩展到96个点 2,1 1,-2 -2,-1 -1,2
	{
		if (i == 0)
		{
			for (m = 0; m < 5; ++m)
				m_grids[m + i * 5 + 5].SetPos(BoardCenter.x - pt[m].y, BoardCenter.y + pt[m].x);
			for (m = 0; m < 6; ++m)
				m_grids[m + i * 6 + 26].SetPos(BoardCenter.x - pt[m + 20].y, BoardCenter.y + pt[m + 20].x);
			for (m = 0; m < 13; ++m)
				m_grids[m + i * 13 + 57].SetPos(BoardCenter.x - pt[m + 44].y, BoardCenter.y + pt[m + 44].x);
		}
		else if (i == 1)
		{
			for (m = 0; m < 5; ++m)
				m_grids[m + i * 5 + 5].SetPos(BoardCenter.x - pt[m].x, BoardCenter.y - pt[m].y);
			for (m = 0; m < 6; ++m)
				m_grids[m + i * 6 + 26].SetPos(BoardCenter.x - pt[m + 20].x, BoardCenter.y - pt[m + 20].y);
			for (m = 0; m < 13; ++m)
				m_grids[m + i * 13 + 57].SetPos(BoardCenter.x - pt[m + 44].x, BoardCenter.y - pt[m + 44].y);
		}
		else if (i == 2)
		{
			for (m = 0; m < 5; ++m)
				m_grids[m + i * 5 + 5].SetPos(BoardCenter.x + pt[m].y, BoardCenter.y - pt[m].x);
			for (m = 0; m < 6; ++m)
				m_grids[m + i * 6 + 26].SetPos(BoardCenter.x + pt[m + 20].y, BoardCenter.y - pt[m + 20].x);
			for (m = 0; m < 13; ++m)
				m_grids[m + i * 13 + 57].SetPos(BoardCenter.x + pt[m + 44].y, BoardCenter.y - pt[m + 44].x);
		}
	}
}
void Game::Ini()//调用一次即可
{
	SetGridsPos();
	int i, j;
	for (i = 0; i < 4; ++i)//设置棋子颜色，位置0~3,4~7
		for (j = 0; j < 4; ++j)
		{
			m_piece[4*i+j].SetColor(i);
			m_piece[4*i+j].m_number = 5*i+j;
		}

}

void Game::OnDraw(CDC* pDC)
{
	DrawBackground(pDC);//画背景
	DrawPieces(pDC);//画棋子
}

void Game::DrawGrids(CDC* pDC)
{
	int r = Piece::raduis;
	CBrush brush, * OldBrush = NULL;
	brush.CreateSolidBrush(RGB(0, 0, 0));
	for (int i = 0; i < GRIDS_NUM; ++i)
	{
		pDC->SelectObject(&brush);
		pDC->Ellipse(m_grids[i].m_center.x - r, m_grids[i].m_center.y - r, m_grids[i].m_center.x + r, m_grids[i].m_center.y + r);
	}
	brush.DeleteObject();
}

void Game::DrawBackground(CDC* pDC)
{
	CBitmap bmp;
	//加载图片
	bmp.LoadBitmapW(IDB_BOARD_BK);
	//获取位图的大小
	BITMAP bm;
	bmp.GetBitmap(&bm);
	//准备内存dc
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);//创建与目标DC兼容的目标DC
	memDC.SelectObject(&bmp);//内存DC选择位图
	pDC->SetStretchBltMode(COLORONCOLOR);//保证颜色不失真
	//从内存DC上将位图拷贝到目标DC（pDC）
	pDC->StretchBlt(m_rtBoard.left, m_rtBoard.top, m_rtBoard.Width(), m_rtBoard.Height(),
		&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	//释放资源
	memDC.DeleteDC();
	bmp.DeleteObject();
}

void Game::SetBoardRect(CRect& rect)//传递一个客户区坐标过来，设置棋盘位置
{
	int nLen = min(rect.Width()*1.2, rect.Height());
	m_rtBoard.SetRect(rect.left+rect.Height()*0.03, rect.top, rect.Height() * 1.03, rect.bottom);
}

Piece* Game::PointToPiece(CPoint point)
{
	//ScreenToClient();
	int r = Piece::raduis, x1 = point.x, y1 = point.y;
	long x, y;
	for (int i = 0; i < PIECE_NUM; i++) {
			x = m_grids[ m_piece[i].m_number].m_center.x;
			y = m_grids[m_piece[i].m_number].m_center.y;
			double dist = sqrt( (x - x1) * (x - x1)  + (y - y1) * (y - y1));
			if (dist > r) continue;
			if(m_piece[i].m_color==NowColor)
				return &m_piece[i];
		}
	return NULL;
}


void Game::DrawPieces(CDC *pDC)
{
	for (int i = 0; i < PIECE_NUM; ++i)
	{
		m_piece[i].Draw(pDC, m_grids[m_piece[i].m_number].m_center);
	}
		
}

void Game::ReDraw()
{
	//通知主窗口重绘指定区域，不用重绘背景
	AfxGetMainWnd()->InvalidateRect(m_rtBoard, FALSE);
}
//
//CRect Game::GetBoardRect()
//{
//	return m_rtBoard;
//}

