// Game.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<stdio.h>
#include<easyx.h>
#include<conio.h>
#include<mmsystem.h>			
#include<Windows.h>
#pragma comment(lib,"winmm.lib")
#define WIDTH 1600
#define HEIGHT 800


typedef struct {
	char name[20];
	int score;
}USER;

void init(USER& user);
void initback();                            //初始化背景
void playMusic(int a,int b);                 //a.b表示音乐播放状态
int judgeMessage(int x,int y);                //判断鼠标点击的按钮

int main() {
	int message=0,x=0,y=0;
	int music = -1,Isinitial=0; //音乐播放器状态，若Isinitial!=1,表明不是初次开始播放，music==1表示正在播放，music==-1则处于暂停
	int isExit = 0;				//判断是否退出
	USER user;
	user.score = 0;
	MOUSEMSG m;
	init(user);
	initback();
	while (1) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			x = m.x;                                 //x为横坐标
			y = m.y;									// y为纵坐标
			message=judgeMessage(x,y);
			switch (message) {
			case 6:
				isExit = MessageBox(NULL,"确定要退出游戏吗？", "提示", MB_YESNO| MB_SYSTEMMODAL);
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
				continue;
			}
		}
	}
	_getch();
	return 0;
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





// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
