#pragma once
#include "resource.h"
#include <afxadv.h>
#include "Piece.h"



class Game
{
private:
	int m_playerNum;//�������
	CRect m_rtBoard;//���ڴ洢��������λ��
	Piece m_piece[PIECE_NUM];//��������
	Grid m_grids[GRIDS_NUM];//���и���
	int m_state;//-1��Ϸδ��ʼ,0��Ϸ��ʼ,1��Ϸ����
	int NowColor;//��ǰҡɫ�ӵ���ң�red,yellow,blue,green,�������,Ϊ-1Ϊδ��ʼ
public:
	Game();//���캯��
	bool HaveFly(int steps);//��ǰ����Ƿ��зɻ����
	int Proceed(Piece* pe, int steps);//ǰ��,ǰ��������,ǰ���Ĳ���,�ɹ�ǰ������1,������ҡɫ�ӷ���0
	int GetState();//��ȡ��Ϸ״̬
	int GetNowColor();//��ǰ������ɫ
	void BeginGame(int playerNum);//��ʼ������Ϸ
	void Ini();//��ʼ����Ϸ��ͼ����
	void OnDraw(CDC* pDC);//������Ϸ
	void DrawGrids(CDC* pDC);//���Ը���
	void DrawBackground(CDC* pDC);//������Ϸ����
	void SetBoardRect(CRect& rect);//������������λ��
	Piece* PointToPiece(CPoint point);
	void ReDraw();//֪ͨ�������ػ���
	void SetGridsPos();//���ø���λ��
	//CRect GetBoardRect();//
protected:
	void DrawPieces(CDC *pDC);//���ڻ�������
};

