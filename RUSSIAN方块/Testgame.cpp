#include "Testgame.h"
#include<time.h>
#include<stdlib.h>
#include"Block.h"
#include<conio.h>

const int speed_normal = 500;
const int speed_quick = 50;
Testgame::Testgame(int rows, int cols, int left, int top, int blocksize)
{   
	this->rows = rows;
	this->cols = cols;
	leftmargin = left;
	topmargin = top;
	this->blocksize = blocksize;
	for (int i = 0; i < rows; i++)
	{
		vector<int> maprows;
		for (int j = 0; j < cols; j++)
		{
			maprows.push_back(0);
		}
		map.push_back(maprows);
	}
}

void Testgame::inital()
{
	delay = speed_normal;
	//配置随机种子
	srand(time(NULL));
	//创建游戏窗口
	initgraph(938, 896);
	//加载背景图片
	loadimage(&imabg, "res/bg2.png");
	//初始化游戏区中的数据
	char data[20][10];
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < cols; j++)
			map[i][j] = 0;

	}
}
void Testgame::play()
{
	inital();
	int timer = 0;
	nextblock = new Block;
	curblock = nextblock;
	nextblock = new Block;
	while (1)
	{
		
		keyevent();//接受用户输入
		timer += getdelay();
		if (timer > delay)
		{
			timer = 0;
			drop();
			update = true;
			//渲染画面
		}
		if (update)
		{
			update = false;
			updatewindow();// 渲染画面(更新游戏画面)
			clearline();                //更新游戏相关数据

		}
	}
}

void Testgame::keyevent()
{
	unsigned char ch;//char是-128,127,无符号是0,255
	bool rotateflag = false;
	int dx = 0;
	if (_kbhit())
	{
		ch = _getch();
		//如果按下方向键，会自动返回两个字符
		//如果按下 （向上）方向键，会先后返回：224 ，72
		//如果按下（向下）方向键，会先后返回：224,80
		// 向左，224,75
		// 向右，224,77
		if (ch == 224)
		{
			ch = _getch();
			switch (ch)
			{
			case 72: 
				rotateflag = true;
				break;
			case 80: 
				delay = speed_quick; 
				break;
			case 75: 
				dx = -1; 
				break;
			case 77: 
				dx = 1; 
				break;
			default:break;
			}
     	}
	}
	if (rotateflag)
	{
		rotate();
		update = true;
		//to do 旋转，待实现
	}
	if (dx != 0)
	{  //实现左右移动
		moveleftright(dx);
		update = true;
	}
}

void Testgame::updatewindow()
{
	putimage(0, 0, &imabg);//绘制背景图片
	IMAGE** imgs = Block::getimages();
	BeginBatchDraw();
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{   if (map[i][j] == 0)
				continue;
		int x = j * blocksize + leftmargin;
		int y = i * blocksize + topmargin;
		putimage(x, y, imgs[map[i][j]-1]);
		}
	}
	curblock->draw(leftmargin,topmargin);
	nextblock->draw(leftmargin + 12 * blocksize, topmargin );
	EndBatchDraw();
}
//第一次调用返回0
//返回距离上一次调用，间隔了多少ms
int Testgame::getdelay()
{
	static unsigned long long lasttime = 0;

	unsigned long long currentime = GetTickCount();
	if (lasttime == 0)
	{
		lasttime = currentime;
		return 0;
	}
	else {
		int ret = currentime - lasttime;
		lasttime = currentime;
		return ret;
	}
}

void Testgame::drop()
{
	bakBlock = *curblock;
	curblock->drop();
	if (!curblock->blockmap(map))
	{
		bakBlock.solidify(map);
		delay = speed_normal;
	//把这个方块“固化”
		delete curblock;
		curblock = nextblock;
		nextblock = new Block;
	}
	
}

void Testgame::clearline()
{
	int k = rows - 1;//存储数据的行数
	for (int i = rows-1; i >= 0; i--)
	{    //检查第i行是否满行
		int count = 0;
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j])
			{		count++;
		     }
			map[k][j] = map[i][j];//一边扫描一边存储
		}

		if (count < cols)
		{
			k--;
			//不是满行
		}
	}
}

void Testgame::moveleftright(int offset)
{
	bakBlock = *curblock;
	curblock->moveleftright(offset);
	if (!curblock->blockmap(map))
	{
		*curblock = bakBlock;
	}
}

void Testgame::rotate()
{
	if (curblock->getblocktype() == 6)
	{
		return;
    }
	bakBlock = *curblock;
	curblock->rerverse();
	if (!curblock->blockmap(map))
	{
		*curblock = bakBlock;
	}
}