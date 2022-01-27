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
	int dx;//最佳横坐标
	int rotate;//旋转次数v 
}MaxScorePosition;

class DLL_EXPORT TetrisAIInterface
{
public:

	static TetrisAIInterface* CreateInterface(int i,int j);

	//设置当前棋盘的状态(具体的传参，留待以后讨论)
	virtual void SetFieldCurrentStat(int** field)=0;

	//设置当前形态状态
	virtual void SetShapeCurrentStat(TetrisType shape)=0;

	//获取当前最佳路径
	virtual MaxScorePosition GetMaxScorePosition()=0;

	//开启自动推理
	virtual void SetAutoRea(bool flag_auto_reasoning)=0;

	//获取版本信息
	virtual std::string GetAIVersion()=0;


};
extern "C" DLL_EXPORT TetrisAIInterface * GetTetrisAIObject(int i, int j);