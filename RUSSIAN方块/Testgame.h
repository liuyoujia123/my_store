#pragma once
using namespace std;
#include<graphics.h>
#include<vector>
#include"Block.h"
class Testgame
{
public: Testgame(int rows, int cols, int left, int top, int blocksize);
	    void inital();
		void play();
private: 
	void keyevent();//�����û�����
	void updatewindow();//��Ⱦ����
	int getdelay();//���ؾ�����һ�ε��øú���������˶���ʱ��(ms)
	void drop();//ͼ���½�
	void clearline();//���һ��
	void moveleftright(int offset);
	void rotate();//������ת
private:
	int delay;
	bool update;//�Ƿ����
	vector<vector<int>>map;// 0,�հף�û���κη���
	// 5���ǵ����ֶ���˹����w
	int rows;
	int cols;
	int leftmargin;
	int topmargin;
	int blocksize;
	IMAGE imabg;

	Block* curblock;
	Block* nextblock;//Ԥ��ķ���
	Block bakBlock;//��ǰ���齵������У�����������һ�ݷ���ķ��顣
};

