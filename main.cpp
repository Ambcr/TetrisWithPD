#include "SFML/Graphics.hpp"
#include "LOG_UTILS.h"
#include "TetrisAIInterface.h"
#include <time.h>
using namespace sf;
//�÷�
int score_all = 0;

//������Ϸ����
const int M = 20;
const int N = 10;

//���巽������
int tetris_type;

//��������ݽṹ
int field[M][N] = {0};

//���������ݽṹ
struct Point
{int x,y;} a[4], b[4];//a ���ĸ����������


//�������˹�������״
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
    text.setString(s); //�����ַ���
    text.setFont(font); //��������
    text.setCharacterSize(36); //���ִ�С
    text.setFillColor(sf::Color::Red); //��ɫ
    text.setPosition(20,410);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined  );
    //����
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

//��һ
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

//����Ƿ�����ƶ�
bool check()
{
   for (int i=0;i<4;i++)//ѭ��4����
      if (a[i].x<0 || a[i].x>=N || a[i].y>=M) return 0;//�жϷ����Ƿ�Խ��
      else if (field[a[i].y][a[i].x]) return 0;//�ж��ƶ���λ���Ƿ��Ѿ��з�����

   return 1;//����true��������ƶ�
};


int main()
{
    bool AI_FLAG = true;
    LOG(LOG_INFO, "Tetris start!");
    srand(time(0));//��rand�ṩseed

    RenderWindow window(VideoMode(320, 480), "The Game!");//��������

    Texture t1,t2,t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/frame.png");
    LOG(LOG_INFO, "Tetris Load picture already!");
    Sprite s(t1), background(t2), frame(t3);

    //Tetris��ʼ��
    int dx=0; bool rotate=0; int colorNum=rand()%8;//���巽���x���꣬��ת��������ɫ��index
    float timer=0,delay=0.3; 
    int blocktype = rand() % 7;//�����������
    tetris_type = blocktype;
    for (int i = 0; i < 4; i++)//���ݷ������͵����ݽṹת��ÿ������ľ�������
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
    while (window.isOpen())//�ж�window�Ǵ򿪵�״̬
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        Event e;
        while (window.pollEvent(e))//��ȡ�����¼�
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
    if (rotate)//˳ʱ��90��任
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
        for (int i=0;i<4;i++) { b[i]=a[i]; a[i].y+=1; }//��������һ��λ��
               
        if (!check())//�����ƶ�
        {
         for (int i=0;i<4;i++) field[b[i].y][b[i].x]=colorNum;//�������֮ǰ��ɫ��

         colorNum=1+rand()%7;//���ɫ��1-7
         int n=rand()%7;//�����������
         tetris_type = n;
         AI_FLAG = true;
         for (int i=0;i<4;i++)//���ݷ������͵����ݽṹת��ÿ������ľ�������
           {
            a[i].x = figures[n][i] % 2+4;
            a[i].y = figures[n][i] / 2;
           }
         if (!check()) break;
        }

         timer=0;
      }


    LOG(LOG_INFO, "Tetris check lines!");//������Ƿ������������ײ㿪ʼѭ��
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

    dx=0; rotate=0; delay=0.001;//��ʼx����Ϊ0����תΪ0����ʱΪ0
    //��ǰλ�������Ѿ���ȡ�������̵ľ�������Լ����ڻ�еķ��������
    //���ǿ����ڴ˴���ȡ���̵�״̬��Ȼ��������Ӧ��AI����

    //AI ����
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
    window.clear(Color::White);    //�������
    window.draw(background);//���Ʊ���ͼ
          
    for (int i=0;i<M;i++)//�������̽���
     for (int j=0;j<N;j++)
       {
         if (field[i][j]==0) continue;
         s.setTextureRect(IntRect(field[i][j]*18,0,18,18));
         s.setPosition(j*18,i*18);
         s.move(28,31); //offset
         window.draw(s);
       }

    for (int i=0;i<4;i++)//���Ƶ�ǰ�����
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
