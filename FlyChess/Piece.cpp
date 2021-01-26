#include "stdafx.h"
#include "Piece.h"
#include "resource.h"
int Piece::raduis = 10;//默认为10

void Piece::Prceed()
{
	if (m_number == 44 + m_color * 13)//在终点线的前一点
	{
		m_number = 25 + 6 * m_color;
	}
	else if (m_number == 95)//在主圈上编号最大的点
	{
		m_number = 44;
	}
	else if (m_number >= 44 && m_number < 95)//在主圈上
	{
		++m_number; 
	}
	else if (m_number <= 25 + m_color * 6 && m_number > 20 + m_color * 6)//在终点线上
	{
		--m_number;
	}
	else if (m_number == m_color * 5 + 4)//在起飞点
	{
		m_number = 47 + m_color * 13;
	}
	else if (m_number == 20 + m_color * 6)//在终点
	{
		++m_number;
	}
}

void Piece::SetColor( int clr)
{
	m_color = clr;
}


void Piece::Draw(CDC* pDC,POINT pt)
{
	int r = Piece::raduis;//半径
	static int PicID[4] = { IDB_RED_PIECE,IDB_YELLOW_PIECE,IDB_BLUE_PIECE ,IDB_GREEN_PIECE };
	CBitmap bmp;
	BITMAP bm;
	//加载图片,并获取其大小
	bmp.LoadBitmapW(PicID[m_color]);
	//检索位图属性
	bmp.GetBitmap(&bm);
	//准备内存dc
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);//创建与目标DC兼容的内存DC
	memDC.SelectObject(&bmp);//内存DC选择位图

	int nOldMode = pDC->SetStretchBltMode(COLORONCOLOR); //设置拉伸模式为颜色保真
	pDC->TransparentBlt(pt.x-r, pt.y-r, 2*r, 2*r, &memDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255,255,255));
	pDC->SetStretchBltMode(nOldMode); //还原拉伸模式
	memDC.DeleteDC(); //销毁内存DC

	//释放资源
	//memDC.DeleteDC();
	bmp.DeleteObject();
}

Piece::Piece()
{
	m_state = -1;
}

int Piece::GetState()
{
	return m_state;
}	
/*
void Piece::BeginFly()
{
	m_state = 0;
	m_number = color + 15;
}*/

Grid::Grid()
{
	m_pPiece = NULL;
}

void Grid::SetPos(int x, int y)
{
	m_center.x = x;
	m_center.y = y;
}

void Grid::SetPos(POINT &pt)
{
	m_center = pt;
}

bool Grid::IsInRegion(POINT pt)
{
	if(sqrt((pt.x-m_center.x)*(pt.x-m_center.x)+(pt.y-m_center.y)*(pt.y-m_center.y)>Piece::raduis))
		return false;
	return true;
}

Piece* Grid::GetPiece()
{
	return m_pPiece;
}

