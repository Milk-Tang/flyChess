#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	NowColor = 0;//��������
	m_state = -1;//��Ϸδ��ʼ
	for (int i = 0; i < 4; ++i)//����������ɫ����ʼλ��
		for (int j = i * 4; j < (i + 1) * 4; ++j)
		{
			m_piece[4 * i + j].SetColor(i);
			m_piece[4 * i + j].m_number = 5 * i + j;
		}
}

bool Game::HaveFly(int steps)//�жϵ�ǰ���ҡ��ɫ�Ӻ��Ƿ����ѡ������ǰ��
{
	int i ;
	if (steps == 6)
		return true;
	for (i = NowColor * 4; i < (1 + NowColor) * 4; ++i)
		if (m_piece[i].m_state == 0)
			return true;
	NowColor = (++NowColor) % m_playerNum;//������޷ɻ���ɣ�������һ����һ���
	return false;
		
}

int Game::Proceed(Piece* pe, int steps)
{
	int i, m;
	int OldNumber = pe->m_number;
	if (pe->m_state == 1)//�ɻ��Ѿ������յ㣬��ǰ�����˲���
		return 0;
	else if (pe->m_state == -1)
	{
		if (steps == 6)
		{
			pe->m_state = 0;
			pe->m_number = pe->m_color * 5 + 4;//�����ɻ�λ�õ���ɵ�
			return 1;//�ɻ��ɹ�ǰ�������ı䵱ǰ������
		}
		return 0;//ѡ�������δ�����δҡ��6����ǰ�����˲���
	}
	else
	{
		if (pe->m_number >=21+ pe->m_color * 6&&pe->m_number<=25+pe->m_color//���յ�����
			&&pe->m_number-steps<20+pe->m_color*6)//����Ҫ����s
			pe->m_number=(20+pe->m_color*6)*2-(pe->m_number-steps);//���յ��ߵ���
		for (i = 0; i < steps; ++i)//һ����ǰ��
		{
			pe->Prceed();
			ReDraw();
		}
		//��������
		m = pe->m_number;
		if ((m - 44) % 4 == pe->m_color && m > 44&& pe->m_number != 64 + 13 * pe->m_color)//�ߵ�ͬɫ�����и�����������Ȧ�ڣ��Ҳ��ڳ������ٵ�
		{
			for(i=0;i<4;++i)
				pe->Prceed();
		}
		else if (pe->m_number == 64 + 13 * pe->m_color)//���ӵ��ﳬǿ���ٵ�
			pe->m_number += 12;//��ǿ����,������ɫ�������ڳ�ǿ���ٵ��ǲ��ᱻ��ص�
		for (i = pe->m_color * 4; i < (pe->m_color + 1) * 4; ++i)
			if (m_piece[i].m_number == OldNumber)//�ж��Ƿ��е����ɻ�
			{
				Proceed(&m_piece[i], steps);
				if (steps != 6)
					--NowColor;
			}
		if (m == pe->m_color * 6 + 20)//�����䵽�յ�
		{
			pe->m_state = 1;
			//�ж���Ϸ�Ƿ����
			m_state = 1;
			for (i = pe->m_color * 4; i < (pe->m_color + 1) * 4; ++i)
			{
				if (m_piece[i].m_state != 1)
				{
					m_state = 0;//��Ϸδ����
					break;
				}
			}
			if(m_state==1)//��Ϸ����
				return 1;
		}
		if (m_grids[m].m_pPiece != NULL)//��ǰλ��������
		{
			if (m_grids[m].m_pPiece->m_color != pe->m_color)//��ͬɫ���ӣ���ظô����ӵ��ɻ�ƺ
			{
				for (i = m_grids[m].m_pPiece->m_color * 4; i < (m_grids[m].m_pPiece->m_color + 1) * 4; ++i)
					if (m == m_piece[i].m_number)
					{
						m_piece[i].m_number = i + m_piece[i].m_color;
						m_piece[i].m_state = -1;
					}
			}
		}
		m_grids[m].m_pPiece = pe; //�µ������ӵ��ڸ������Ϸ�
		//m_game.ReDraw();//�ػ���Ϸ
		if (steps == 6)//������ҡһ��ɫ��
			return 1; //��ǰ�����˲���
		else
		{
			NowColor = (++NowColor) % m_playerNum;//�����˸ı�
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
	NowColor = 0;//��ɫ���ȿ�ʼ������
	m_state = 0;//��Ϸ��ʼ
	m_playerNum = playerNum;
	int i;
	for (i = 0; i < PIECE_NUM; ++i)
	{
		//��ʼ������
		m_piece[i].m_state = -1;
		m_piece[i].m_number = i + m_piece[i].m_color;
		m_piece[i].m_state = -1;
		//��ʼ������
		m_grids[m_piece[i].m_number].m_pPiece = NULL;
	}
	ReDraw();//�ػ���Ϸ
}


void Game::SetGridsPos()
{
	int i, j;
	float nLenth = ((float)m_rtBoard.Width()) / 824;//ͼƬ��ÿ�����ؾ��ڿؼ��еĳ���
	//�������ĵ�
	POINT BoardCenter;
	BoardCenter.x = m_rtBoard.left + nLenth * 413;
	BoardCenter.y = m_rtBoard.top + nLenth * 413;
	Piece::raduis = 20 * nLenth;
	float GridLen = 97 * nLenth;//����ͼ��һ�������ڳ����еĳ���
	float GridWidth = 47.5 * nLenth;//����ͼ��һ�������ڳ����еĿ��
	//���ú���ӷɻ�ƺ��ʼλ��
	POINT pt[GRIDS_NUM];//���ĵ����·��ķ�֮һ�����ڸ��������ĵ�ĺ����������
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


	//�������и��ӵ�����
	for (i = 0; i < GRIDS_NUM; ++i)
		m_grids[i].SetPos(pt[i].x + BoardCenter.x, pt[i].y + BoardCenter.y);

	int m;
	for (i = 0; i < 3; ++i)//����������ת,��24������չ��96���� 2,1 1,-2 -2,-1 -1,2
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
void Game::Ini()//����һ�μ���
{
	SetGridsPos();
	int i, j;
	for (i = 0; i < 4; ++i)//����������ɫ��λ��0~3,4~7
		for (j = 0; j < 4; ++j)
		{
			m_piece[4*i+j].SetColor(i);
			m_piece[4*i+j].m_number = 5*i+j;
		}

}

void Game::OnDraw(CDC* pDC)
{
	DrawBackground(pDC);//������
	DrawPieces(pDC);//������
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
	//����ͼƬ
	bmp.LoadBitmapW(IDB_BOARD_BK);
	//��ȡλͼ�Ĵ�С
	BITMAP bm;
	bmp.GetBitmap(&bm);
	//׼���ڴ�dc
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);//������Ŀ��DC���ݵ�Ŀ��DC
	memDC.SelectObject(&bmp);//�ڴ�DCѡ��λͼ
	pDC->SetStretchBltMode(COLORONCOLOR);//��֤��ɫ��ʧ��
	//���ڴ�DC�Ͻ�λͼ������Ŀ��DC��pDC��
	pDC->StretchBlt(m_rtBoard.left, m_rtBoard.top, m_rtBoard.Width(), m_rtBoard.Height(),
		&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	//�ͷ���Դ
	memDC.DeleteDC();
	bmp.DeleteObject();
}

void Game::SetBoardRect(CRect& rect)//����һ���ͻ��������������������λ��
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
	//֪ͨ�������ػ�ָ�����򣬲����ػ汳��
	AfxGetMainWnd()->InvalidateRect(m_rtBoard, FALSE);
}
//
//CRect Game::GetBoardRect()
//{
//	return m_rtBoard;
//}

