// Game.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<easyx.h>
#include<conio.h>
#include<mmsystem.h>	
#include <tchar.h>
#pragma comment(lib,"winmm.lib")
#define WIDTH 1600
#define HEIGHT 800


typedef struct {
	char name[20];
	int score;
}USER;

typedef struct Problem {
	int number1;
	int number2;
	char operation;
	int result;
	struct Problem* next;
}PROBLEM;

typedef struct bomb {
	int x, y;
	int RightResult;
	bool isRight = false;
	struct bomb* next;
}Bomb;

IMAGE refresh1,refresh2,bomb, bomb_mask;

void init(USER& user);
void initback();									 //初始化背景									
void playMusic(int a,int b);						 //a.b表示音乐播放状态
int judgeMessage(int x,int y);						//判断鼠标点击的按钮
void newProblem(int* r,int* n1,int *n2,char *op);				//为每一个结点生成题目
void playGame(Bomb bomb1,Bomb bomb2,Bomb bomb3);
int moveBomb(Bomb& bomb1,Bomb& bomb2,Bomb& bomb3,int isFinish);
Problem *initProblem();




int main() {
	int message = 0, x = 0, y = 0,k=0;
	int music = -1, Isinitial = 0; //音乐播放器状态，若Isinitial!=1,表明不是初次开始播放，music==1表示正在播放，music==-1则处于暂停
	int isStart = 0, isExit = 0,isFinish = 0;  //判断是否已经开始游戏，判断是否退出,判断上一道题是否回答结束
	int n = 0, choice = 5;
	char s[5];
	USER user;
	PROBLEM* head;
	user.score = 0;
	MOUSEMSG m;
	Bomb bomb1 = { WIDTH - 1000,0,0 };						//参数为 起始位置 x,y   存储的结果
	Bomb bomb2 = { WIDTH - 600,-250,0 };
	Bomb bomb3 = { WIDTH - 900,-500,0 };
	init(user);
	initback();
	
	while (1) {
		k=moveBomb(bomb1, bomb2, bomb3,isFinish);
		isFinish = 0;
		bomb1.y += 5;
		bomb2.y += 5;
		bomb3.y += 5;
		if (bomb1.y == HEIGHT) {
			bomb1.y = 0;
		}
		else if (bomb2.y == HEIGHT) {
			bomb2.y = 0;
		}
		else if (bomb3.y == HEIGHT) {
			bomb3.y = 0;
		}
		
		while (MouseHit()) {
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN) {
				x = m.x;                                 //x为横坐标
				y = m.y;									// y为纵坐标
				message = judgeMessage(x, y);

				switch (message) {
				case 1:
					isFinish = 1;
					break;

				case 6:
					isExit = MessageBox(NULL, "确定要退出游戏吗？", "提示", MB_YESNO | MB_SYSTEMMODAL);
					if (isExit == 6) {
						exit(0);
					}
					break;
				case 7:
					music = -music;
					Isinitial++;
					playMusic(music, Isinitial);
					break;
				default:
					if (m.x > bomb1.x&& m.x<bomb1.x + 240 && m.y>bomb1.y&& m.y < bomb1.y + 160) {
						choice = 1;

					}
					else if (m.x > bomb2.x&& m.x<bomb2.x + 240 && m.y>bomb2.y&& m.y < bomb2.y + 160) {
						choice = 2;
					}
					else if (m.x > bomb3.x&& m.x<bomb3.x + 240 && m.y>bomb3.y&& m.y < bomb3.y + 160) {
						choice = 3;
					}
				
					break;
				}


				if (choice == k) {
					isFinish = 1;
					user.score += 10;
				}
				else if (choice == 5) {
				}
				else {
					EndBatchDraw();
					MessageBox(NULL, "拆弹失败了！", "游戏结束", MB_OK | MB_SYSTEMMODAL);
					isFinish = 1;
				}
			}
		}
	}
	return 0;
}


void init(USER& user)
{
	initgraph(WIDTH, HEIGHT);
	loadimage(NULL, "image\\welcomebackground.jpg", WIDTH, HEIGHT);

	InputBox(user.name, 20, "请输入用户名：");
	outtextxy(WIDTH / 2 + 400, HEIGHT / 2 - 200, "Name: ");
	outtextxy(WIDTH / 2 + 450, HEIGHT / 2 - 200, user.name);

	char buf[10];
	sprintf_s(buf, "%d", user.score);   //user.score为整型，outtextxy要求输出的是 char[]
	outtextxy(WIDTH / 2 + 400, HEIGHT / 2 - 150, "Score: ");
	outtextxy(WIDTH / 2 + 450, HEIGHT / 2 - 150, buf);
}

void initback() {
	IMAGE start,add,hint,exit,save,read,music,music_mask,button_mask;

	initgraph(WIDTH, HEIGHT, SHOWCONSOLE);
	loadimage(NULL, "image\\background.jpg", WIDTH, HEIGHT);
	setlinecolor(BLACK);

	loadimage(&button_mask,"image\\button_mask.jpg", 120, 80);           //加载按钮掩码图

	loadimage(&start, "image\\start.jpg", 120, 80);                       //制作开始按钮
	putimage(WIDTH - 300, HEIGHT / 2, &button_mask,SRCAND);
	putimage(WIDTH - 300, HEIGHT / 2, &start, SRCPAINT);

	loadimage(&add, "image\\add.jpg", 120, 80);							//制作增加按钮
	putimage(WIDTH - 300, HEIGHT / 2 + 200, &button_mask, SRCAND);
	putimage(WIDTH - 300, HEIGHT / 2 + 200, &add,SRCPAINT);
    
	loadimage(&hint, "image\\hint.jpg", 120, 80);					//制作提示按钮
	putimage(WIDTH - 300, HEIGHT / 2 + 100, &button_mask,SRCAND);
	putimage(WIDTH - 300, HEIGHT / 2 + 100, &hint,SRCPAINT);

	loadimage(&read, "image\\read.jpg", 120, 80);					//制作读取按钮
	putimage(WIDTH - 150, HEIGHT / 2 + 100, &button_mask,SRCAND);
	putimage(WIDTH - 150, HEIGHT / 2 + 100, &read, SRCPAINT);

	loadimage(&save, "image\\save.jpg", 120, 80);						//制作存档按钮
	putimage(WIDTH - 150, HEIGHT / 2, &button_mask, SRCAND);
	putimage(WIDTH - 150, HEIGHT / 2, &save, SRCPAINT);

	loadimage(&exit, "image\\exit.jpg", 120, 80);						//制作退出按钮
	putimage(WIDTH - 150, HEIGHT / 2 + 200, &button_mask,SRCAND);
	putimage(WIDTH - 150, HEIGHT / 2 + 200, &exit, SRCPAINT);

	loadimage(&music, "image\\music.jpg", 330, 80);						//制作背景音乐播放器按钮
	loadimage(&music_mask, "image\\music_mask.jpg", 330, 80);
	putimage(WIDTH - 330, HEIGHT / 2 + 300, &music_mask,SRCAND);
	putimage(WIDTH - 330, HEIGHT / 2 + 300, &music,SRCINVERT);
	
	settextstyle(20, 20, "宋体");
	setbkmode(TRANSPARENT);
	outtextxy(WIDTH - 250, HEIGHT / 2 - 100, "当前题目");

	getimage(&refresh1, WIDTH - 1000, 0, 650, HEIGHT);
	getimage(&refresh2, WIDTH - 300, HEIGHT / 2 - 50, 240, 25);

	loadimage(&bomb, "image\\bomb.jpg", 240, 160);
	loadimage(&bomb_mask, "image\\bomb_mask.jpg", 240, 160);

}


int judgeMessage(int x, int y) {
	int message;
	if (x > WIDTH - 300 && x<WIDTH - 180 && y>HEIGHT / 2 && y < HEIGHT / 2 + 80) {
		message = 1;
	}
	else if (x > WIDTH - 150 && x<WIDTH - 30 && y>HEIGHT / 2 && y < HEIGHT / 2 + 80) {
		message = 2;
	}
	else if (x > WIDTH - 300 && x<WIDTH - 180 && y>HEIGHT / 2 + 100 && y < HEIGHT / 2 + 180) {
		message = 3;
	}
	else if (x > WIDTH - 150 && x<WIDTH - 30 && y>HEIGHT / 2 + 100 && y < HEIGHT / 2 + 180) {
		message = 4;
	}
	else if (x > WIDTH - 300 && x<WIDTH - 180 && y>HEIGHT / 2 + 200 && y < HEIGHT / 2 + 280) {
		message = 5;
	}
	else if (x > WIDTH - 150 && x<WIDTH - 30 && y>HEIGHT / 2 + 200 && y < HEIGHT / 2 + 280) {
		message = 6;
	}
	else if (x > WIDTH - 305 && x<WIDTH-25 && y>HEIGHT / 2 + 300 && y < HEIGHT / 2 + 380)	{
		message = 7;
	}
	else {
		message = 0;
	}
	return message;
}

void playMusic(int a,int b) {
	if (a == 1&&b == 1) {
		mciSendString("open music.mp3", 0, 0, 0);
		mciSendString("play music.mp3 repeat", 0, 0, 0);
	}
	else if(a == 1&&b != 1){
		mciSendString("pause music.mp3", 0, 0, 0);
	}
	else if (a == -1 && b != 1) {
		mciSendString("resume music.mp3", 0, 0, 0);
	}
}

PROBLEM *initProblem() {
	int num = 1;
	int num1 = 0, num2 = 0,res=0;
	char ope = '+';								
	int* a = &res;						//存储表达式的结果
	int* b = &num1;							//生成的第一个数字
	int* c = &num2;							//生成的第二个数字
	char* op = &ope;						////生成的运算符号
	PROBLEM * p, * head = NULL, * r = NULL;
	while (num < 15) {
		p = (PROBLEM*)malloc(sizeof(PROBLEM));
		newProblem(a,b,c,op);
		if (p == NULL) {
			printf("内存分配不成功！");
		}
		else {
			p->number1 = num1;
			p->number2 = num2;
			p->operation = ope;
			p->result = res;
			p->next = NULL;
			if (head == NULL) {
				head = p;
				r = p;
			}
			else {
				r->next = p;
				r = p;
			}
			num++;
		}
		return head;
	}

}

void newProblem(int* r,int* n1,int* n2,char* op) {
	int op_kind,isInteger;
	srand((int)time(NULL));
	*n1 = rand() % 101;
	*n2 = rand() % 101;
	op_kind = rand() % 3+1;
	switch (op_kind) {
	case 1:
		*op = '+';
		*r = *n1 + *n2;
		break;
	case 2:
		*op = '-';
		*r = *n1 - *n2;
		break;
	case 3:
		*op = '*';
		*r = *n1 * *n2;
		break;
	
	}
	
}

int moveBomb(Bomb& bomb1, Bomb& bomb2, Bomb& bomb3,int isFinish) {
	char s[5];
	int k, n;
	Problem* head;

	head = initProblem();
	if (isFinish == 1) {
		clearrectangle(WIDTH - 300, HEIGHT/2-50, WIDTH - 60, HEIGHT/2-25);
		putimage(WIDTH-300,HEIGHT/2-50, &refresh2);
		_stprintf_s(s, "%d", head->number1);					//将整型转换成字符串，打印题目
		outtextxy(WIDTH - 300, HEIGHT / 2 - 50, s);

		outtextxy(WIDTH - 240, HEIGHT / 2 - 50, head->operation);

		_stprintf_s(s, "%d", head->number2);
		outtextxy(WIDTH - 200, HEIGHT / 2 - 50, s);

		outtextxy(WIDTH - 140, HEIGHT / 2 - 50, "=");

		outtextxy(WIDTH - 80, HEIGHT / 2 - 50, "?");

		n = rand() % 3+1;
		switch (n) {
		case 1:
			bomb1.RightResult = head->result;
			printf("%d", bomb1.RightResult);
			bomb1.isRight = true;
			bomb2.RightResult = rand() % 201;
			bomb3.RightResult = rand() % 101;
			break;
		case 2:
			bomb2.RightResult = head->result;
			printf("%d",bomb2.RightResult);
			bomb2.isRight = true;
			bomb1.RightResult = rand() % 201;
			bomb3.RightResult = rand() % 101;
			break;
		case 3:
			bomb3.RightResult = head->result;
			printf("%d", bomb3.RightResult);
			bomb3.isRight = true;
			bomb2.RightResult = rand() % 201;
			bomb1.RightResult = rand() % 101;
			break;
		}
		head = head->next;
	}
			if (bomb1.isRight) {
				k = 1;
			}
			else if (bomb2.isRight) {
				k = 2;
			}
			else if(bomb3.isRight){
				k = 3;
			}
		
		BeginBatchDraw();
		putimage(bomb1.x, bomb1.y, &bomb_mask, SRCAND);
		putimage(bomb1.x, bomb1.y, &bomb, SRCPAINT);
		_stprintf_s(s, "%d",bomb1.RightResult);
		outtextxy(bomb1.x + 60, bomb1.y + 80, s);

		putimage(bomb2.x, bomb2.y, &bomb_mask, SRCAND);
		putimage(bomb2.x, bomb2.y, &bomb, SRCPAINT);
		_stprintf_s(s, "%d",bomb2.RightResult);
		outtextxy(bomb2.x + 60, bomb2.y + 80, s);

		putimage(bomb3.x, bomb3.y, &bomb_mask, SRCAND);
		putimage(bomb3.x, bomb3.y, &bomb, SRCPAINT);
		_stprintf_s(s, "%d",bomb3.RightResult);
		outtextxy(bomb3.x + 60, bomb3.y + 80, s);

		FlushBatchDraw();

		clearrectangle(WIDTH - 1000, 0, WIDTH - 350, HEIGHT);
		putimage(WIDTH - 1000, 0, &refresh1);

		Sleep(100);
		return k;

}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
