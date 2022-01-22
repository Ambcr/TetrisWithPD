#include"TetrisAI.h"
#include"LOG_UTILS.h"
#include <string>
#include<vector>
TetrisAI::TetrisAI(int width, int height)
{
	fieldHeight = height;
	fieldWidth = width;
	fieldCurrent = new int* [height];
	mergeField = new int* [height];
	for (int i = 0; i < height; i++) {
		fieldCurrent[i] = new int[width];
		mergeField[i] = new int[width];
	}
}
TetrisAI::~TetrisAI() 
{
	for (int i = 0; i < fieldHeight; i++) {
		delete[] fieldCurrent[i];
		delete[] mergeField[i];
	}
	delete[] fieldCurrent;
	delete[] mergeField;
}

//打印棋盘
void TetrisAI::PrintField()
{
	std::cout << std::endl;
	for (int i = 0; i < fieldHeight; ++i)
	{
		for (int j = 0; j < fieldWidth; ++j)
		{
			if (fieldCurrent[i][j] != 0) std::cout << "■";
			else std::cout << "□";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

}

//打印合并棋盘
void TetrisAI::PrintMergeField()
{
	std::cout << std::endl;
	for (int i = 0; i < fieldHeight; ++i)
	{
		for (int j = 0; j < fieldWidth; ++j)
		{
			if (mergeField[i][j] != 0) std::cout << "■";
			else std::cout << "□";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//检查两个元素是否一致
bool TetrisAI::CheckABDiff(int a, int b)
{
	if ((0 == a && 0 != b) || (0 != a && 0 == b)) return true;
	return false;
}

//检查两边是否是石头
bool TetrisAI::IsLeftRightStone(int i, int j)
{
	bool left_flag = false;
	bool right_flag = false;
	if (j - 1 < 0) left_flag = true;
	else {
		if (mergeField[i][j - 1] != 0) left_flag = true;
	}
	if (j + 1 >= fieldWidth) right_flag = true;
	else {
		if (mergeField[i][j + 1] != 0) right_flag = true;
	}
	if (left_flag && right_flag) return true;
	else return false;
}

//连加函数
int TetrisAI::NumberPlusPlus(int number)
{
	return number * (1 + number) / 2;
}

//获取AI对象
 TetrisAI* TetrisAI::GetTetrisAIObject(int width, int height)
{
	return new TetrisAI(width,height);
}

//设置当前棋盘的状态(具体的传参，留待以后讨论)
void TetrisAI::SetFieldCurrentStat(int** field)
{
	for (int i = 0; i < fieldHeight; i++)
	{
		for (int j = 0; j < fieldWidth; j++)
		{
			fieldCurrent[i][j] = field[i][j];
		}
	}
}

//设置当前形状状态
void TetrisAI::SetShapeCurrentStat(TetrisType shape)
{
	//LOG(LOG_DEBUG, "currentshape:"+std::to_string(shape));
	for (int i = 0; i < 4; i++)
	{
		currentShape[i].x = tetrisFigures[shape][i] % 2 ;
		currentShape[i].y = tetrisFigures[shape][i] / 2;
	}
}


//开启自动推理
void TetrisAI::SetAutoRea(bool flag_auto_reasoning)
{
	//暂不实现
}


//改变shape的状态至完全落地
void TetrisAI::ShapeDown()
{
	TetrisPoint shape_temp[4];
	for (int i = 0; i < fieldHeight; ++i) {
		for (int j = 0; j < 4; ++j) {
			shape_temp[j].x = currentShape[j].x;
			shape_temp[j].y=currentShape[j].y+1;
		}
		LOG(LOG_DEBUG,"IN SHAPEDOWN INDEX:"+std::to_string(i));
		LOG(LOG_DEBUG, "IN SHAPEDOWN check ok:" + std::to_string(CheckIfOK(currentShape)));
		LOG(LOG_DEBUG, "IN SHAPEDOWN check next ok:" + std::to_string(CheckIfOK(shape_temp)));
		if (CheckIfOK(currentShape) && !CheckIfOK(shape_temp)) return;
		for (int z = 0; z < 4; z++) {
			currentShape[z].y++;
		}
	}
}

//这个感觉有点多余了
//还原shape至初始位置
void TetrisAI::ResetShapeToUp()
{
	int minheight=currentShape[0].y;
	for (int i = 0; i < 4; ++i) {
		if (currentShape[i].y < minheight) minheight = currentShape[i].y;
	}
	for (int i = 0; i < 4; ++i) {
		currentShape[i].y -= minheight;
	}
}

//方块规整
void TetrisAI::ShapeRegular()
{
	int minx = currentShape[0].x;
	int miny = currentShape[0].y;
	for (int i = 0; i < 4; ++i)
	{
		if (currentShape[i].x < minx) minx = currentShape[i].x;
		if (currentShape[i].y < miny) miny = currentShape[i].y;
	}
	for (int i = 0; i < 4; ++i)
	{
		currentShape[i].x -= minx;
		currentShape[i].y -= miny;
	}

}

//旋转shape
void TetrisAI::RotateShape()
{
	TetrisPoint centor = currentShape[1];
	for (int i = 0; i < 4; i++)
	{
		int x = currentShape[i].y - centor.y;
		int y = currentShape[i].x - centor.x;
		currentShape[i].x = centor.x - x;
		currentShape[i].y = centor.y + y;
	}
}
//设置shapedx
bool TetrisAI::SetShapeDX(int dx)
{
	TetrisShapeRange range = GetShapeRange();
	if (dx > range.right || dx < range.left) return false;
	int mindx = currentShape[0].x;
	for (int i = 0; i < 4; ++i) {
		if (currentShape[i].x < mindx) mindx = currentShape[i].x;
	}
	for (int i = 0; i < 4; ++i) {
		currentShape[i].x -= (mindx-dx);
	}
	return true;

}

//获取shape在棋盘上的动态范围
TetrisShapeRange TetrisAI::GetShapeRange()
{
	TetrisShapeRange shaperange;
	TetrisPoint shape_temp[4];
	int mindx = currentShape[0].x;
	int minheight = currentShape[0].y;
	for (int i = 0; i < 4; ++i) {
		shape_temp[i] = currentShape[i];
		//LOG(LOG_DEBUG,"shapetemp x"+std::to_string(shape_temp[i].x)
		//+" y "+ std::to_string(shape_temp[i].y));
		if (shape_temp[i].x < mindx) mindx = shape_temp[i].x;
		if (shape_temp[i].y < minheight) minheight = shape_temp[i].y;
	}
	//还原至最左顶点
	for (int i = 0; i < 4; ++i) {
		shape_temp[i].x -= mindx;
		shape_temp[i].y -= minheight;
	}
	//for (int i = 0; i < 4; ++i)
	//{
	//	LOG(LOG_DEBUG,"SHAPE_TEMP.X"+std::to_string(shape_temp[i].x)+ "SHAPE_TEMP.Y"+ std::to_string(shape_temp[i].y));
	//}
	//寻找左右界限
	bool leftflag = false;
	for (int i = 0; i < fieldWidth+1; ++i) {
		if (!leftflag&&CheckIfOK(shape_temp)) {
			shaperange.left = i;
			leftflag = true;
		}
		if (leftflag && !CheckIfOK(shape_temp)) {
			shaperange.right = i - 1;
			break;
		}
		for(int j=0;j<4;++j) shape_temp[j].x += 1;	
	}
	//LOG(LOG_DEBUG, "range.left" + std::to_string(shaperange.left) + "range.right" + std::to_string(shaperange.right));
	return shaperange;
}
//check放置当前位置是否ok
bool TetrisAI::CheckIfOK(TetrisPoint* shape_here)
{
	for (int i = 0; i < 4; ++i) {
		if (shape_here[i].x < 0 || shape_here[i].x >= fieldWidth) return false;
		if (shape_here[i].y < 0 || shape_here[i].y >= fieldHeight) return false;
	}
	int flag = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (fieldCurrent[shape_here[i].y][shape_here[i].x] != 0) flag += 1;
	}
	if (flag != 0) return false;
	else return true;
}



//合并shape和field
void TetrisAI::DoMergeShapeAndField()
{
	for (int i = 0; i < fieldHeight; ++i)
	{
		for (int j = 0; j < fieldWidth; ++j)
		{
			mergeField[i][j] = fieldCurrent[i][j];
		}
	}
	for (int i = 0; i < 4; ++i)
	{
 		mergeField[currentShape[i].y][currentShape[i].x] = 1;
	}
}
//获取erodedPieceCellsMetric分数=消除行数*贡献方块数
int TetrisAI::GeterodedPieceCallsMetric()
{
	std::vector<int> row_vector;
	int eliminaterow = 0;
	int shapenum = 0;
	int k = fieldHeight - 1;
	for (int row = fieldHeight - 1; row >= 0; --row)
	{
		bool flag = true;
		for (int col = 0; col < fieldWidth; ++col)
		{
			if (mergeField[row][col] == 0) flag = false;
			mergeField[k][col] = mergeField[row][col];
		}
		if (flag)
		{
			row_vector.push_back(row);
			eliminaterow++;
		}
		else {
			k--;
		}
	}
	for (int i = 0; i < row_vector.size(); ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (currentShape[j].y == row_vector[i]) shapenum++;
		}
	}
	LOG(LOG_DEBUG, "erodedPieceCallsMetric SCORE:" + std::to_string(eliminaterow*shapenum));
	return eliminaterow * shapenum;
}


//获取boardRowTransitions分数
int TetrisAI::GetboardRowTransitions()
{
	int sum = 0;
	for (int i = 0; i < fieldHeight; ++i)
	{
		for (int j = 0; j < fieldWidth-1; ++j)
		{
			if (CheckABDiff(mergeField[i][j], mergeField[i][j+1])) sum++;
		}
	}
	LOG(LOG_DEBUG, "boardRowTransitions SCORE:" + std::to_string(sum));
	return sum;
}

//获取boardColTransitions分数
int TetrisAI::GetboardColTransitions()
{
	int sum = 0;
	for (int i = 0; i < fieldWidth; ++i)
	{
		for (int j = 0; j < fieldHeight-1; ++j)
		{
			if (CheckABDiff(mergeField[j][i], mergeField[j+1][i])) sum++;
		}
	}
	LOG(LOG_DEBUG, "boardColTransitions SCORE:" + std::to_string(sum));
	return sum;
}

//获取boardBuriedHoles分数
int TetrisAI::GetboardBuriedHoles()
{
	bool flag_start = false;
	int sum = 0;
	for (int i = 0; i < fieldWidth; ++i)
	{
		for (int j = 0; j < fieldHeight; ++j)
		{
			if (mergeField[j][i] != 0) flag_start = true;
			if (flag_start && mergeField[j][i] == 0) sum++;
		}
		flag_start = false;
	}
	LOG(LOG_DEBUG, "boardBuriedHoles SCORE:" + std::to_string(sum));
	return sum;
}

//获取boardWells分数
//这里有个疑问，请看下列情形,第二列的＃深多少
/*
# ##
#  #
## #
*/
int TetrisAI::GetboardWells()
{
	std::vector<int> num;
	for (int i = 0; i < fieldWidth; ++i)
	{
		int well = 0;
		bool flag = false;
		for (int j = 0; j < fieldHeight; ++j)
		{
			if (flag && mergeField[j][i] == 0)
			{
				well++;
			}
			else
			{
				if (well != 0) num.push_back(well);
				flag = false;
				well = 0;
			}
			if ((mergeField[j][i] == 0 )&& IsLeftRightStone(j, i)&&(flag==false))
			{
				well++;
				flag = true;
			}
			if((j==fieldHeight-1)&&(well!=0)) num.push_back(well);
		}
	}
	int sum = 0;
	for (int i = 0; i < num.size(); ++i)
	{
		sum += NumberPlusPlus(num[i]);
	}
	LOG(LOG_DEBUG, "boardWells SCORE:" + std::to_string(sum));
	return sum;
}

//获取shape重心高度
int TetrisAI::GetLandingHeight()
{
	int score= fieldHeight - currentShape[1].y;
	LOG(LOG_DEBUG,"LandingHeight SCORE:"+std::to_string(score));
	return score;
}

//获取总分
int TetrisAI::GetScore()
{
	int piececallmetric = GeterodedPieceCallsMetric();
	int score= -45*GetLandingHeight() + 34*piececallmetric - 32*GetboardRowTransitions() -
		93*GetboardColTransitions() - (79 * GetboardBuriedHoles()) - 34*GetboardWells();
	//LOG(LOG_FATAL , "ALL SCORE:" + std::to_string(score));
	return score;
}

//获取当前最佳路径
MaxScorePosition TetrisAI::GetMaxScorePosition()
{
	MaxScorePosition maxscoreposition = {0,0};
	int maxscore = -99999;
	for (int rot = 0; rot < 4; ++rot) 
	{//0:0 1:90 2:180 3:270 4:360
		ShapeRegular();
		TetrisShapeRange range = GetShapeRange();
		LOG(LOG_DEBUG, "range.left= " + std::to_string(range.left) + " range.right= " + std::to_string(range.right));
		for (int i = range.left; i <= range.right; ++i)
		{
			SetShapeDX(i);
			for (int shape_index = 0; shape_index < 4; ++shape_index)
			{
				LOG(LOG_DEBUG, "shape.x=" + std::to_string(currentShape[shape_index].x)+" shape.y="+std::to_string(currentShape[shape_index].y));
			}
			LOG(LOG_DEBUG,"\n");
			ShapeDown();
			//PrintMergeField();
			DoMergeShapeAndField();
			//PrintMergeField();
			for (int shape_index = 0; shape_index < 4; ++shape_index)
			{
				LOG(LOG_DEBUG, "shapedown shape.x=" + std::to_string(currentShape[shape_index].x) + "shapedown shape.y=" + std::to_string(currentShape[shape_index].y));
			}
			LOG(LOG_DEBUG, "\n");
			int score = GetScore();
			if (score > maxscore) 
			{
				maxscore = score;
				maxscoreposition.dx = i;
				maxscoreposition.rotate = rot;
			}
			ResetShapeToUp();
		}
		RotateShape();
	}
	return maxscoreposition;
}