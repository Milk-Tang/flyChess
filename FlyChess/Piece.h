#pragma once

#define PIECE_NUM 16 //���ӵ�����
#define GRIDS_NUM 96 //���ӵ�����,ǰ0~15Ϊ�ɻ�ƺ,16~19Ϊ��ɵ�
#define Playr_num 4 //�������

class Piece;
class Grid;
class Game;

//������
class Grid
{
public:
	friend Game;
	Grid();
	void SetPos(int x,int y);
	void SetPos(POINT &pt);
	Piece* GetPiece();//��ȡ�ڸ����ڵ�����,�����ӷ���NULL
	bool IsInRegion(POINT pt);//�жϵ�pt�Ƿ��ڸ������������򷵻�true�����򷵻�false
private:
	POINT m_center;//����λ��
	Piece* m_pPiece;//�����ϵ�����ָ��
};


//������
class Piece
{
private:
	int m_color;//���ӵ���ɫ,0,1,2,3�ֱ�Ϊred, yellow, blue, green //�������
	int m_number;//���ӵ�λ�ã������ڸ�������ı��
	int m_state;//����״̬��-1Ϊ�ڷɻ�ƺ��0Ϊ�����У�1Ϊ�����յ�
public:
	friend Game;
	Piece();
	int GetState();//��������״̬
	static int raduis;//���Ӱ뾶
//	void BeginFly();//��ʼ���,���ɻ����õ���ɵص�
	void Prceed();//ǰ��
	void SetColor(int clr);//������ɫ
protected:
	void Draw(CDC *pDC,POINT pt);//������
};