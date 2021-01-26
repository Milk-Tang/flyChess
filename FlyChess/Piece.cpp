#include "stdafx.h"
#include "Piece.h"
#include "resource.h"
int Piece::raduis = 10;//Ĭ��Ϊ10

void Piece::Prceed()
{
	if (m_number == 44 + m_color * 13)//���յ��ߵ�ǰһ��
	{
		m_number = 25 + 6 * m_color;
	}
	else if (m_number == 95)//����Ȧ�ϱ�����ĵ�
	{
		m_number = 44;
	}
	else if (m_number >= 44 && m_number < 95)//����Ȧ��
	{
		++m_number; 
	}
	else if (m_number <= 25 + m_color * 6 && m_number > 20 + m_color * 6)//���յ�����
	{
		--m_number;
	}
	else if (m_number == m_color * 5 + 4)//����ɵ�
	{
		m_number = 47 + m_color * 13;
	}
	else if (m_number == 20 + m_color * 6)//���յ�
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
	int r = Piece::raduis;//�뾶
	static int PicID[4] = { IDB_RED_PIECE,IDB_YELLOW_PIECE,IDB_BLUE_PIECE ,IDB_GREEN_PIECE };
	CBitmap bmp;
	BITMAP bm;
	//����ͼƬ,����ȡ���С
	bmp.LoadBitmapW(PicID[m_color]);
	//����λͼ����
	bmp.GetBitmap(&bm);
	//׼���ڴ�dc
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);//������Ŀ��DC���ݵ��ڴ�DC
	memDC.SelectObject(&bmp);//�ڴ�DCѡ��λͼ

	int nOldMode = pDC->SetStretchBltMode(COLORONCOLOR); //��������ģʽΪ��ɫ����
	pDC->TransparentBlt(pt.x-r, pt.y-r, 2*r, 2*r, &memDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255,255,255));
	pDC->SetStretchBltMode(nOldMode); //��ԭ����ģʽ
	memDC.DeleteDC(); //�����ڴ�DC

	//�ͷ���Դ
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

