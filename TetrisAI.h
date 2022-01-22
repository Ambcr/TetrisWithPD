#ifndef _TETRISAI_H_
#define _TETRISAI_H_
typedef enum{
	Tetris_I=0,
	Tetris_Z,
	Tetris_S,
	Tetris_T,
	Tetris_L,
	Tetris_J,
	Tetris_O
}TetrisType;

typedef struct {
	int left;
	int right;
}TetrisShapeRange;
typedef struct {
	int x;
	int y;
}TetrisPoint;

typedef struct {
	int dx;//��Ѻ�����
	int rotate;//��ת����v 
}MaxScorePosition;

class TetrisAI
{
public:
	TetrisAI(int width,int height);
	~TetrisAI();
	
	//��ȡAI����
	static TetrisAI* GetTetrisAIObject(int width,int height);

	//���õ�ǰ���̵�״̬(����Ĵ��Σ������Ժ�����)
	void SetFieldCurrentStat(int** field);

	//���õ�ǰ��̬״̬
	void SetShapeCurrentStat(TetrisType shape);

	//��ȡ��ǰ���·��
	MaxScorePosition GetMaxScorePosition();

	//�����Զ�����
	void SetAutoRea(bool flag_auto_reasoning);

private:
	int fieldWidth;
	int fieldHeight;
	int** fieldCurrent;
	int** mergeField;
	TetrisPoint currentShape[4];
	int tetrisFigures[7][4] =
	{
		1,3,5,7, // I
		2,4,5,7, // Z
		3,5,4,6, // S
		3,5,4,7, // T
		2,3,5,7, // L
		3,5,7,6, // J
		2,3,4,5, // O
	};

	//�ı�shape��״̬����ȫ���
	void ShapeDown();

	//��ԭshape����ʼλ��
	void ResetShapeToUp();

	//��תshape
	void RotateShape();

	//���������ĸ߶�
	int GetLandingHeight();

	//����shapedx
	bool SetShapeDX(int dx);

	//��ȡshape�������ϵĶ�̬��Χ
	TetrisShapeRange GetShapeRange();

	//check���õ�ǰλ���Ƿ�ok
	bool CheckIfOK(TetrisPoint* shape_here);

	//��ȡerodedPieceCellsMetric����
	int GeterodedPieceCallsMetric(); 

	//��ȡboardRowTransitions����
	int GetboardRowTransitions();

	//��ȡboardColTransitions����
	int GetboardColTransitions();

	//��ȡboardBuriedHoles����
	int GetboardBuriedHoles();

	//��ȡboardWells����
	int GetboardWells();

	//��ȡ�ܷ�
	int GetScore();

	//�ϲ�shape��field
	void DoMergeShapeAndField();

	//�������Ԫ���Ƿ�һ��
	bool CheckABDiff(int a, int b);


	//��������Ƿ���ʯͷ
	bool IsLeftRightStone(int i, int j);

	//���Ӻ���
	int NumberPlusPlus(int number);

	//�������
	void ShapeRegular();

	//��ӡ����
	void PrintField();

	//��ӡ�ϲ�����
	void PrintMergeField();


};

#endif // !_TETRISAI_H_

