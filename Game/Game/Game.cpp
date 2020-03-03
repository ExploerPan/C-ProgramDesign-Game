// Game.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<stdio.h>
#include<easyx.h>
#include<conio.h>
#include<mmsystem.h>				    
#pragma comment(lib,"winmm.lib")
#define WIDTH 1400
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
	int music = -1,Isinitial=0;              //音乐播放器状态，若Isinitial!=1,表明不是初次开始播放，music==1表示正在播放，music==-1则处于暂停
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
			case 4:
				exit;
				break;
			case 5:
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
	IMAGE b1,b11, b2,b22,b3,b33,b4,b44,music;
	initgraph(WIDTH, HEIGHT, SHOWCONSOLE);
	loadimage(NULL, "image\\background.jpg", WIDTH, HEIGHT);
	setlinecolor(BLACK);
	loadimage(&b1, "image\\button_1.jpg", 120, 80);
	//loadimage(&b11, "image\\button_11.jpg", 120, 80);
	putimage(WIDTH - 300, HEIGHT / 2, &b1);
	//putimage(WIDTH - 300, HEIGHT / 2, &b11);
	loadimage(&b2, "image\\button_2.jpg", 120, 80);
	//loadimage(&ib2, "image\\Invertbutton_2.jpg", 120, 80);
	putimage(WIDTH - 150, HEIGHT / 2, &b2);
	//putimage(WIDTH - 150, HEIGHT / 2, &b22);
	loadimage(&b3, "image\\button_3.jpg", 120, 80);
	//loadimage(&b33, "image\\button_33.jpg", 120, 80);
	putimage(WIDTH - 300, HEIGHT / 2+100, &b3);
	//putimage(WIDTH - 300, HEIGHT / 2 + 100, &b33);
	loadimage(&b4, "image\\button_4.jpg", 120, 80);
	//loadimage(&b44, "image\\button_44.jpg", 120, 80);
	putimage(WIDTH - 150, HEIGHT / 2 + 100, &b4);
	//putimage(WIDTH - 150, HEIGHT / 2 + 100, &b44);
	loadimage(&music, "image\\music.png", 120, 80);
	putimage(WIDTH - 300, HEIGHT / 2 + 200, &music);



	//loadimage(&b2,"image"\\button_2.jpg,100,60);
	//loadimage(&ib2,"image\\Invertbutton_2")
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
