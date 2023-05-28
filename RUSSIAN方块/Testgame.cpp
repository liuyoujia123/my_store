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
	//�����������
	srand(time(NULL));
	//������Ϸ����
	initgraph(938, 896);
	//���ر���ͼƬ
	loadimage(&imabg, "res/bg2.png");
	//��ʼ����Ϸ���е�����
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
		
		keyevent();//�����û�����
		timer += getdelay();
		if (timer > delay)
		{
			timer = 0;
			drop();
			update = true;
			//��Ⱦ����
		}
		if (update)
		{
			update = false;
			updatewindow();// ��Ⱦ����(������Ϸ����)
			clearline();                //������Ϸ�������

		}
	}
}

void Testgame::keyevent()
{
	unsigned char ch;//char��-128,127,�޷�����0,255
	bool rotateflag = false;
	int dx = 0;
	if (_kbhit())
	{
		ch = _getch();
		//������·���������Զ����������ַ�
		//������� �����ϣ�����������Ⱥ󷵻أ�224 ��72
		//������£����£�����������Ⱥ󷵻أ�224,80
		// ����224,75
		// ���ң�224,77
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
		//to do ��ת����ʵ��
	}
	if (dx != 0)
	{  //ʵ�������ƶ�
		moveleftright(dx);
		update = true;
	}
}

void Testgame::updatewindow()
{
	putimage(0, 0, &imabg);//���Ʊ���ͼƬ
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
//��һ�ε��÷���0
//���ؾ�����һ�ε��ã�����˶���ms
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
	//��������顰�̻���
		delete curblock;
		curblock = nextblock;
		nextblock = new Block;
	}
	
}

void Testgame::clearline()
{
	int k = rows - 1;//�洢���ݵ�����
	for (int i = rows-1; i >= 0; i--)
	{    //����i���Ƿ�����
		int count = 0;
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j])
			{		count++;
		     }
			map[k][j] = map[i][j];//һ��ɨ��һ�ߴ洢
		}

		if (count < cols)
		{
			k--;
			//��������
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