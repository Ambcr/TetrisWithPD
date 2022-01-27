#pragma once
#include <iostream>

#ifdef DLL_BUILD
#define DLL_EXPORT _declspec(dllexport)
#else
#define DLL_EXPORT _declspec(dllimport)
#endif
typedef enum {
	Tetris_I = 0,
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

class DLL_EXPORT TetrisAIInterface
{
public:

	static TetrisAIInterface* CreateInterface(int i,int j);

	//���õ�ǰ���̵�״̬(����Ĵ��Σ������Ժ�����)
	virtual void SetFieldCurrentStat(int** field)=0;

	//���õ�ǰ��̬״̬
	virtual void SetShapeCurrentStat(TetrisType shape)=0;

	//��ȡ��ǰ���·��
	virtual MaxScorePosition GetMaxScorePosition()=0;

	//�����Զ�����
	virtual void SetAutoRea(bool flag_auto_reasoning)=0;

	//��ȡ�汾��Ϣ
	virtual std::string GetAIVersion()=0;


};
extern "C" DLL_EXPORT TetrisAIInterface * GetTetrisAIObject(int i, int j);