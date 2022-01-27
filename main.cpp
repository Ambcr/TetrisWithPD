#include "SFML/Graphics.hpp"
#include "LOG_UTILS.h"
#include "TetrisAIInterface.h"
#include <time.h>
using namespace sf;
//得分
int score_all = 0;

//定义游戏行列
const int M = 20;
const int N = 10;

//定义方块类型
int tetris_type;

//界面格数据结构
int field[M][N] = {0};

//定义点的数据结构
struct Point
{int x,y;} a[4], b[4];//a 是四个方块的坐标


//定义俄罗斯方块的形状
int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};
sf::Text init_text(const std::wstring& s, const sf::Font& font) {
    sf::Text text;
    text.setString(s); //设置字符串
    text.setFont(font); //设置字体
    text.setCharacterSize(36); //文字大小
    text.setFillColor(sf::Color::Red); //颜色
    text.setPosition(20,410);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined  );
    //属性
    return text;
}

sf::Font init_font(const std::string& s) {
    sf::Font font;
    if (font.loadFromFile(s)) ;
    return font;
}

//checkifok
bool MainCheckIfOK(Point* shape_here)
{
    for (int i = 0; i < 4; ++i) {
        if (shape_here[i].x < 0 || shape_here[i].x >= 10) return false;
        if (shape_here[i].y < 0 || shape_here[i].y >= 20) return false;
    }
    int flag = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (field[shape_here[i].y][shape_here[i].x] != 0) flag += 1;
    }
    if (flag != 0) return false;
    else return true;
}

//shapedown
void MainShapeDown()
{
    Point shape_temp[4];
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 4; ++j) {
            shape_temp[j].x = a[j].x;
            shape_temp[j].y = a[j].y + 1;
        }
        if (MainCheckIfOK(a) && !MainCheckIfOK(shape_temp)) return;
        for (int z = 0; z < 4; z++) {
            a[z].y++;
        }
    }
}

void MainRotateShap(int rot)
{
    for (int num = 0; num < rot; ++num)
    {
        Point p = a[1]; //center of rotation
        for (int i = 0; i < 4; i++)
        {
            int x = a[i].y - p.y;
            int y = a[i].x - p.x;
            a[i].x = p.x - x;
            a[i].y = p.y + y;
        }
    }
}

//归一
void MainResetShape()
{
    int minx = a[0].x;
    int miny = a[0].y;
    for (int i = 0; i < 4; ++i)
    {
        if (a[i].x < minx) minx = a[i].x;
        if (a[i].y < miny) miny = a[i].y;
    }
    if (minx < 0)
    {
        for (int i = 0; i < 4; ++i)
        {
            a[i].x -= minx;
        }
    }
    if (miny < 0)
    {
        for (int i = 0; i < 4; ++i)
        {
            a[i].y -= miny;
        }
    }
}

void MainSetDX(int dx)
{
    int mindx = a[0].x;
    for (int i = 0; i < 4; ++i) {
        if (a[i].x < mindx) mindx = a[i].x;
    }
    for (int i = 0; i < 4; ++i) {
        a[i].x -= (mindx - dx);
    }
}

//检查是否可以移动
bool check()
{
   for (int i=0;i<4;i++)//循环4个点
      if (a[i].x<0 || a[i].x>=N || a[i].y>=M) return 0;//判断方块是否越界
      else if (field[a[i].y][a[i].x]) return 0;//判断移动的位置是否已经有方块了

   return 1;//返回true代表可以移动
};


int main()
{
    bool AI_FLAG = true;
    LOG(LOG_INFO, "Tetris start!");
    srand(time(0));//给rand提供seed

    RenderWindow window(VideoMode(320, 480), "The Game!");//创建窗口

    Texture t1,t2,t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/frame.png");
    LOG(LOG_INFO, "Tetris Load picture already!");
    Sprite s(t1), background(t2), frame(t3);

    //Tetris初始化
    int dx=0; bool rotate=0; int colorNum=rand()%8;//定义方块的x坐标，旋转，还有颜色的index
    float timer=0,delay=0.3; 
    int blocktype = rand() % 7;//随机方块类型
    tetris_type = blocktype;
    for (int i = 0; i < 4; i++)//根据方块类型的数据结构转换每个方格的具体坐标
    {
        a[i].x = figures[blocktype][i] % 2+4;
        a[i].y = figures[blocktype][i] / 2;
    }
    auto tetrishandle = LoadLibrary(L"TetrisAI.dll");
    TetrisAIInterface* tetrisai = nullptr;
    if (tetrishandle)
    {
        typedef TetrisAIInterface* (CALLBACK* DllFunc)(int, int);

        DllFunc CreateObject = (DllFunc)GetProcAddress(tetrishandle,"GetTetrisAIObject");
        
        tetrisai= CreateObject(10,20);
        //TetrisAI* a = new TetrisAI(10,20);
        LOG(LOG_FATAL, "TetrisAIVersion:" + std::to_string((int)tetrisai));
        LOG(LOG_FATAL, "TetrisAIVersion:" + tetrisai->GetAIVersion());
        //LOG(LOG_FATAL, "SUCCESS TO LOAD LIBRARY TetrisAI.dll code:" + std::to_string((int)tetrishandle));
    }
    else LOG(LOG_FATAL,"FIALED TO LOAD LIBRARY TetrisAI.dll code:" + std::to_string((int)tetrishandle));
    //TetrisAI* tetrisai = TetrisAI::GetTetrisAIObject(10, 20);

    Clock clock;
    LOG(LOG_INFO, "Tetris game start");
    //for (int i = 1; i < 20; ++i)
    //{
    //    field[i][0] = 1;
    //}
    while (window.isOpen())//判断window是打开的状态
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        Event e;
        while (window.pollEvent(e))//获取键盘事件
        {
            LOG(LOG_DEBUG, "Tetris get input Event!");
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Up) rotate = true;
                else if (e.key.code == Keyboard::Left) dx = -1;
                else if (e.key.code == Keyboard::Right) dx = 1;
                //else if (e.key.code == Keyboard::Down) {
                //    delay = 0.05;
                //    LOG(LOG_DEBUG, "Tetris key DOWN PRESSED!");
                //}
        }
    LOG(LOG_INFO, "Tetris key pressed!");
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
        delay = 0.05;
        LOG(LOG_DEBUG, "Tetris key DOWN PRESSED!");
    }
    //LOG(LOG_DEBUG, "Tetris DELAY:"+std::to_string(delay));

    LOG(LOG_INFO, "Tetris Move!");
    //// <- Move -> ///
    for (int i=0;i<4;i++)  { b[i]=a[i]; a[i].x+=dx; }
    if (!check()) for (int i=0;i<4;i++) a[i]=b[i];


    LOG(LOG_INFO, "Tetris Rotate!");
    //////Rotate//////
    if (rotate)//顺时针90°变换
      {
        Point p = a[1]; //center of rotation
        for (int i=0;i<4;i++)
          {
            int x = a[i].y-p.y;
            int y = a[i].x-p.x;
            a[i].x = p.x - x;
            a[i].y = p.y + y;
           }
           if (!check()) for (int i=0;i<4;i++) a[i]=b[i];
      }

    LOG(LOG_INFO, "Tetris Tick!");
    ///////Tick//////
    if (timer>delay)
      {
        for (int i=0;i<4;i++) { b[i]=a[i]; a[i].y+=1; }//方块下落一个位置
               
        if (!check())//不能移动
        {
         for (int i=0;i<4;i++) field[b[i].y][b[i].x]=colorNum;//界面填充之前的色块

         colorNum=1+rand()%7;//随机色块1-7
         int n=rand()%7;//随机方块类型
         tetris_type = n;
         AI_FLAG = true;
         for (int i=0;i<4;i++)//根据方块类型的数据结构转换每个方格的具体坐标
           {
            a[i].x = figures[n][i] % 2+4;
            a[i].y = figures[n][i] / 2;
           }
         if (!check()) break;
        }

         timer=0;
      }


    LOG(LOG_INFO, "Tetris check lines!");//检查行是否可消除，从最底层开始循环
    ///////check lines//////////
    int k=M-1;
    for (int i=M-1;i>0;i--)
    {
        int count=0;
        for (int j=0;j<N;j++)
        {
            if (field[i][j]) count++;
            field[k][j]=field[i][j];
        }
        if (count<N) k--;
        else {
            score_all++;
        }
    }

    dx=0; rotate=0; delay=0.001;//初始x坐标为0，旋转为0，延时为0
    //当前位置我们已经获取到了棋盘的具体情况以及正在活动中的方块的坐标
    //我们可以在此处读取棋盘的状态，然后设置相应的AI策略

    //AI 策略
    if (AI_FLAG)
    {
        tetrisai->SetShapeCurrentStat((TetrisType)tetris_type);
        int* ptr_arr[20];
        for (int i = 0; i < 20; ++i) {
            ptr_arr[i] = field[i];
        }
        tetrisai->SetFieldCurrentStat(ptr_arr);
        MaxScorePosition position = tetrisai->GetMaxScorePosition();
        LOG(LOG_DEBUG,"MAX SCORE dx:"+std::to_string(position.dx)+" rotate:"+std::to_string(position.rotate));
        MainRotateShap(position.rotate);
        MainResetShape();
        MainSetDX(position.dx);
        MainShapeDown();

        AI_FLAG = false;
    }



    LOG(LOG_INFO, "Tetris draw!");
    /////////draw//////////
    window.clear(Color::White);    //清除界面
    window.draw(background);//绘制背景图
          
    for (int i=0;i<M;i++)//绘制棋盘界面
     for (int j=0;j<N;j++)
       {
         if (field[i][j]==0) continue;
         s.setTextureRect(IntRect(field[i][j]*18,0,18,18));
         s.setPosition(j*18,i*18);
         s.move(28,31); //offset
         window.draw(s);
       }

    for (int i=0;i<4;i++)//绘制当前活动方块
      {
        s.setTextureRect(IntRect(colorNum*18,0,18,18));
        s.setPosition(a[i].x*18,a[i].y*18);
        s.move(28,31); //offset
        window.draw(s);
      }

    window.draw(frame);
    auto font = init_font("arial.ttf");
    auto text = init_text(L"score:"+std::to_string(score_all), font);
    window.draw(text);
    window.display();
    }
    LOG(LOG_FATAL,"score:"+std::to_string(score_all));
    return 0;
}
