#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<easyx.h>
#include<conio.h>
#include<mmsystem.h>	
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
	int num;	//题目编号
	struct Problem* next;
}PROBLEM;

typedef struct bomb {
	int x, y;
	int RightResult;
	bool isRight = false;
}Bomb;

IMAGE refresh1, refresh2, refresh3, bomb, bomb_mask;

void initwelcome(USER& user);		//初始化欢迎界面，音乐，登陆信息
void initback();					  //初始化背景									
void playMusic(int a, int b);			 //a.b表示音乐播放状态
int judgeMessage(int x, int y);				//判断鼠标点击的按钮
void showInfo(USER& user, int problemNum, int& problemSum);
void showProblem(Problem* head, int isFinish, int problemNum, int& problemSum, USER& user);		//刷新显示题目
void showBomb(Bomb& bomb1, Bomb& bomb2, Bomb& bomb3);				//显示炸弹
void setAnswer(Problem* head, Bomb& bomb1, Bomb& bomb2, Bomb& bomb3, int problemNum, int isFinish);//显示答案及标出正确炸弹的信息
int judgechoice(int x, int y, Bomb& bomb1, Bomb& bomb2, Bomb& bomb3);	//判断玩家选择的答案
bool judgeTorF(int choice, Bomb& bomb1, Bomb& bomb2, Bomb& bomb3);		//判断玩家答案是否正确
void gameover(int problemNum, int problemSum, USER& user);			 //判断是否做完所有题
Problem* initProblem();							//初始化题目 建立链表
Problem* hint(Problem* head, int problemNum);		//链表中删除题目
Problem* insert(Problem* head, int& problemSum);		//链表中增加题目
void save(Problem* head, USER& user, int& problemNum, int& problemSum);
PROBLEM* read(USER& user, int& problemNum, int& problemSum);

int main() {
	int message = 0, x = 0, y = 0, rightresult = 0, problemNum = 1, problemSum = 15;

	//音乐播放器状态，若Isinitial!=1,表明不是初次开始播放，music==1表示正在播放，music==-1则处于暂停
	int music = -1, Isinitial = 0;

	//判断是否已经开始游戏，判断是否退出,判断上一道题是否回答结束,判断是否使用提示，判断是否增加题目
	int isStart = 0, isExit = 0, isFinish = 0, isHint = 0, isInsert;

	int n = 0, choice = 0;
	USER user;
	PROBLEM* head, * p;
	user.score = 0;
	MOUSEMSG m;
	//以下参数为:起始位置 x,起始位置y，存储的结果
	Bomb bomb1 = { WIDTH - 1000,0,0 };
	Bomb bomb2 = { WIDTH - 600,-250,0 };
	Bomb bomb3 = { WIDTH - 900,-500,0 };
	initwelcome(user);
	initback();
	head = initProblem();
	while (1) {
		p = head;
		if (isStart == 1) {
			showInfo(user, problemNum, problemSum);
			showProblem(p, isFinish, problemNum, problemSum, user);
			gameover(problemNum, problemSum, user);
			setAnswer(p, bomb1, bomb2, bomb3, problemNum, isFinish);
			showBomb(bomb1, bomb2, bomb3);

			isFinish = 0;
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
					if (isStart == 0) {
						isStart = 1;
					}
					else {
						MessageBox(NULL, "游戏已经开始了！", "提示", MB_OK | MB_SYSTEMMODAL);
					}
					break;
				case 2:
					save(head, user, problemNum, problemSum);
					break;
				case 3:
					isHint = MessageBox(NULL, "确定要请求提示吗？\n提示将扣除20分！", "警告", MB_YESNO | MB_SYSTEMMODAL);
					if (isHint == 6) {
						head = hint(head, problemNum);
						problemSum--;
						user.score -= 20;							//使用一次提示扣除20分
						isFinish = 1;
					}
					break;
				case 4:
					head=read(user, problemNum, problemSum);
					break;
				case 5:
					isInsert = MessageBox(NULL, "准备好增加3个挑战了吗？", "提示", MB_YESNO | MB_SYSTEMMODAL);
					if (isInsert == 6) {
						head = insert(head, problemSum);
					}
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
					choice = judgechoice(x, y, bomb1, bomb2, bomb3);
					if (judgeTorF(choice, bomb1, bomb2, bomb3) && choice != 0) {
						isFinish = 1;
						problemNum++;
						user.score += 10;
					}
					else if (!judgeTorF(choice, bomb1, bomb2, bomb3) && choice != 0 && isStart != 0) {
						isFinish = 1;
						problemNum++;
						mciSendString("open music\\explode.mp3", 0, 0, 0);
						mciSendString("play music\\explode.mp3 ", 0, 0, 0);
						if (user.score > 0) {
							user.score = 0;
							MessageBox(NULL, "拆错了一个炸弹！\n您的分数将被清零/(ㄒoㄒ)/~~", "提示", MB_OK | MB_SYSTEMMODAL);
						}
						else {
							user.score -= 20;
							MessageBox(NULL, "拆错了一个炸弹！\n您的分数已经是负数了！不能清零，只好减去20分啦\n加油啊/(ㄒoㄒ)/~~", "提示", MB_OK | MB_SYSTEMMODAL);
						}
						Sleep(500);
						mciSendString("close music\\explode.mp3 ", 0, 0, 0);
					}
					else {

					}
					break;
				}

			}
		}
	}
	return 0;
}

void initwelcome(USER& user)
{
	//mciSendString("open music\\welcomemusic.mp3", 0, 0, 0);
	//mciSendString("play music\\welcomemusic.mp3 repeat", 0, 0, 0);
	initgraph(WIDTH, HEIGHT);
	loadimage(NULL, "image\\welcomebackground.jpg", WIDTH, HEIGHT);

	InputBox(user.name, 20, "请输入您的昵称：");
	outtextxy(WIDTH / 2 + 400, HEIGHT / 2 - 200, "昵称： ");
	outtextxy(WIDTH / 2 + 450, HEIGHT / 2 - 200, user.name);

	char buf[10];
	sprintf_s(buf, "%d", user.score);   //user.score为整型，outtextxy要求输出的是 char[]
	outtextxy(WIDTH / 2 + 400, HEIGHT / 2 - 150, "Score: ");
	outtextxy(WIDTH / 2 + 450, HEIGHT / 2 - 150, buf);

	//mciSendString("close music\\welcomemusic.mp3", 0, 0, 0);
}

void initback() {
	IMAGE start, add, hint, exit, save, read, music, music_mask, button_mask;

	initgraph(WIDTH, HEIGHT, SHOWCONSOLE);
	loadimage(NULL, "image\\background.jpg", WIDTH, HEIGHT);
	setlinecolor(BLACK);

	loadimage(&button_mask, "image\\button_mask.jpg", 120, 80);           //加载按钮掩码图

	loadimage(&start, "image\\start.jpg", 120, 80);                       //制作开始按钮
	putimage(WIDTH - 300, HEIGHT / 2, &button_mask, SRCAND);
	putimage(WIDTH - 300, HEIGHT / 2, &start, SRCPAINT);

	loadimage(&add, "image\\add.jpg", 120, 80);							//制作增加按钮
	putimage(WIDTH - 300, HEIGHT / 2 + 200, &button_mask, SRCAND);
	putimage(WIDTH - 300, HEIGHT / 2 + 200, &add, SRCPAINT);

	loadimage(&hint, "image\\hint.jpg", 120, 80);					//制作提示按钮
	putimage(WIDTH - 300, HEIGHT / 2 + 100, &button_mask, SRCAND);
	putimage(WIDTH - 300, HEIGHT / 2 + 100, &hint, SRCPAINT);

	loadimage(&read, "image\\read.jpg", 120, 80);					//制作读取按钮
	putimage(WIDTH - 150, HEIGHT / 2 + 100, &button_mask, SRCAND);
	putimage(WIDTH - 150, HEIGHT / 2 + 100, &read, SRCPAINT);

	loadimage(&save, "image\\save.jpg", 120, 80);						//制作存档按钮
	putimage(WIDTH - 150, HEIGHT / 2, &button_mask, SRCAND);
	putimage(WIDTH - 150, HEIGHT / 2, &save, SRCPAINT);

	loadimage(&exit, "image\\exit.jpg", 120, 80);						//制作退出按钮
	putimage(WIDTH - 150, HEIGHT / 2 + 200, &button_mask, SRCAND);
	putimage(WIDTH - 150, HEIGHT / 2 + 200, &exit, SRCPAINT);

	loadimage(&music, "image\\music.jpg", 330, 80);						//制作背景音乐播放器按钮
	loadimage(&music_mask, "image\\music_mask.jpg", 330, 80);
	putimage(WIDTH - 330, HEIGHT / 2 + 300, &music_mask, SRCAND);
	putimage(WIDTH - 330, HEIGHT / 2 + 300, &music, SRCINVERT);

	settextstyle(20, 20, "宋体");
	setbkmode(TRANSPARENT);
	outtextxy(WIDTH - 250, HEIGHT / 2 - 100, "当前题目");

	getimage(&refresh1, WIDTH - 1000, 0, 650, HEIGHT);					//刷新炸弹显示区
	getimage(&refresh2, WIDTH - 300, HEIGHT / 2 - 50, 300, 50);			//刷新当前题目
	getimage(&refresh3, WIDTH - 349, HEIGHT / 2 - 325, 349, 201);		//刷新分数、剩余题目、时间区域

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
	else if (x > WIDTH - 305 && x<WIDTH - 25 && y>HEIGHT / 2 + 300 && y < HEIGHT / 2 + 380) {
		message = 7;
	}
	else {
		message = 0;
	}
	return message;
}

void playMusic(int a, int b) {
	if (a == 1 && b == 1) {
		mciSendString("open music\\gamemusic.mp3", 0, 0, 0);
		mciSendString("play music\\gamemusic.mp3 repeat", 0, 0, 0);
	}
	else if (a == 1 && b != 1) {
		mciSendString("resume music\\gamemusic.mp3", 0, 0, 0);
	}
	else if (a == -1 && b != 1) {
		mciSendString("pause music\\gamemusic.mp3", 0, 0, 0);
	}
}

PROBLEM* initProblem() {
	int num = 1, op_kind;
	int num1 = 0, num2 = 0, res = 0;
	char ope = '+';
	PROBLEM* p, * head = NULL, * r = NULL;
	srand((unsigned)time(NULL));
	while (num <= 15) {
		p = (PROBLEM*)malloc(sizeof(PROBLEM));
		num1 = rand() % 101;
		num2 = rand() % 101;
		op_kind = rand() % 3 + 1;
		switch (op_kind) {
		case 1:
			ope = '+';
			res = num1 + num2;
			break;
		case 2:
			ope = '-';
			res = num1 - num2;
			break;
		case 3:
			ope = '*';
			res = num1 * num2;
			break;
		}
		p->number1 = num1;
		p->number2 = num2;
		p->operation = ope;
		p->result = res;
		p->num = num;
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

void showInfo(USER& user, int problemNum, int& problemSum) {
	char buf[10];
	clearrectangle(WIDTH - 350, HEIGHT / 2 - 325, WIDTH - 150, HEIGHT / 2 - 125);
	putimage(WIDTH - 349, HEIGHT / 2 - 325, &refresh3);
	outtextxy(WIDTH - 300, HEIGHT / 2 - 350, "昵称: ");
	outtextxy(WIDTH - 200, HEIGHT / 2 - 350, user.name);

	sprintf_s(buf, "%d", user.score);
	outtextxy(WIDTH - 300, HEIGHT / 2 - 300, "分数: ");
	outtextxy(WIDTH - 200, HEIGHT / 2 - 300, buf);

	sprintf_s(buf, "%d", problemSum - problemNum+1);
	outtextxy(WIDTH - 300, HEIGHT / 2 - 250, "剩余：");
	outtextxy(WIDTH - 200, HEIGHT / 2 - 250, buf);

}

void showProblem(Problem* p, int isFinish, int problemNum, int& problemSum, USER& user) {
	char s[10];
	if (isFinish == 1 && problemNum <= problemSum) {
		while (p->num != problemNum) {
			p = p->next;
		}
		clearrectangle(WIDTH - 300, HEIGHT / 2 - 50, WIDTH - 60, HEIGHT / 2 - 25);
		putimage(WIDTH - 300, HEIGHT / 2 - 50, &refresh2);
		_stprintf_s(s, "%d", p->number1);					//将整型转换成字符串，打印题目
		outtextxy(WIDTH - 300, HEIGHT / 2 - 50, s);

		outtextxy(WIDTH - 240, HEIGHT / 2 - 50, p->operation);

		_stprintf_s(s, "%d", p->number2);
		outtextxy(WIDTH - 200, HEIGHT / 2 - 50, s);

		outtextxy(WIDTH - 140, HEIGHT / 2 - 50, "=");

		outtextxy(WIDTH - 90, HEIGHT / 2 - 50, "?");
	}
}

void showBomb(Bomb& bomb1, Bomb& bomb2, Bomb& bomb3) {
	char s[10];
	BeginBatchDraw();
	putimage(bomb1.x, bomb1.y, &bomb_mask, SRCAND);
	putimage(bomb1.x, bomb1.y, &bomb, SRCPAINT);
	_stprintf_s(s, "%d", bomb1.RightResult);
	outtextxy(bomb1.x + 60, bomb1.y + 80, s);

	putimage(bomb2.x, bomb2.y, &bomb_mask, SRCAND);
	putimage(bomb2.x, bomb2.y, &bomb, SRCPAINT);
	_stprintf_s(s, "%d", bomb2.RightResult);
	outtextxy(bomb2.x + 60, bomb2.y + 80, s);

	putimage(bomb3.x, bomb3.y, &bomb_mask, SRCAND);
	putimage(bomb3.x, bomb3.y, &bomb, SRCPAINT);
	_stprintf_s(s, "%d", bomb3.RightResult);
	outtextxy(bomb3.x + 60, bomb3.y + 80, s);

	bomb1.y += 5;
	bomb2.y += 5;
	bomb3.y += 5;

	FlushBatchDraw();

	clearrectangle(WIDTH - 1000, 0, WIDTH - 350, HEIGHT);
	putimage(WIDTH - 1000, 0, &refresh1);

	Sleep(100);

	if (bomb1.y == HEIGHT) {
		bomb1.y = 0;
	}
	else if (bomb2.y == HEIGHT) {
		bomb2.y = 0;
	}
	else if (bomb3.y == HEIGHT) {
		bomb3.y = 0;
	}
}

void setAnswer(Problem* p, Bomb& bomb1, Bomb& bomb2, Bomb& bomb3, int problemNum, int isFinish) {
	int n = 0;
	if (isFinish == 1) {
		while (p->num != problemNum) {
			p = p->next;
		}
		n = rand() % 3 + 1;
		switch (n) {
		case 1:
			do {
				bomb1.RightResult = p->result;
				bomb1.isRight = true;
				bomb2.isRight = false;
				bomb3.isRight = false;
				printf("%d ", bomb1.RightResult);
				bomb2.RightResult = rand() % 201;
				bomb3.RightResult = rand() % 101;
			} while (bomb1.RightResult == bomb2.RightResult || bomb1.RightResult == bomb3.RightResult);
			break;
		case 2:
			do {
				bomb2.RightResult = p->result;
				bomb1.isRight = false;
				bomb2.isRight = true;
				bomb3.isRight = false;
				printf("%d ", bomb2.RightResult);
				bomb1.RightResult = rand() % 201;
				bomb3.RightResult = rand() % 101;
			} while (bomb2.RightResult == bomb1.RightResult || bomb2.RightResult == bomb3.RightResult);
			break;
		case 3:
			do {
				bomb3.RightResult = p->result;
				bomb1.isRight = false;
				bomb2.isRight = false;
				bomb3.isRight = true;
				printf("%d ", bomb3.RightResult);
				bomb2.RightResult = rand() % 201;
				bomb1.RightResult = rand() % 101;
			} while (bomb3.RightResult == bomb1.RightResult || bomb3.RightResult == bomb2.RightResult);
			break;
		}
	}

}

int judgechoice(int x, int y, Bomb& bomb1, Bomb& bomb2, Bomb& bomb3) {
	if (x > bomb1.x&& x<bomb1.x + 240 && y>bomb1.y&& y < bomb1.y + 160) {
		return 1;
	}
	else if (x > bomb2.x&& x<bomb2.x + 240 && y>bomb2.y&& y < bomb2.y + 160) {
		return 2;
	}
	else if (x > bomb3.x&& x<bomb3.x + 240 && y>bomb3.y&& y < bomb3.y + 160) {
		return 3;
	}
	else {
		return 0;
	}
}

bool judgeTorF(int choice, Bomb& bomb1, Bomb& bomb2, Bomb& bomb3) {
	int rightchoice = 0;
	if (bomb1.isRight == true) {
		rightchoice = 1;
	}
	else if (bomb2.isRight == true) {
		rightchoice = 2;
	}
	else if (bomb3.isRight == true) {
		rightchoice = 3;
	}
	if (rightchoice == choice) {
		return true;
	}
	else {
		return false;
	}

}

void gameover(int problemNum,int problemSum,USER&user) {
	char s[10];
	if(problemNum > problemSum) {

		clearrectangle(WIDTH - 350, HEIGHT / 2 - 325, WIDTH - 150, HEIGHT / 2 - 125);
		putimage(WIDTH - 349, HEIGHT / 2 - 325, &refresh3);
		outtextxy(WIDTH - 300, HEIGHT / 2 - 350, "昵称: ");
		outtextxy(WIDTH - 200, HEIGHT / 2 - 350, user.name);

		sprintf_s(s, "%d", user.score);
		outtextxy(WIDTH - 300, HEIGHT / 2 - 300, "分数: ");
		outtextxy(WIDTH - 200, HEIGHT / 2 - 300, s);

		sprintf_s(s, "%d", problemSum - problemNum + 1);
		outtextxy(WIDTH - 300, HEIGHT / 2 - 250, "剩余：");
		outtextxy(WIDTH - 200, HEIGHT / 2 - 250, s);
	wsprintf(s, "%d", user.score);
	MessageBox(NULL, "已经没有炸弹了", "提示", MB_OK | MB_SYSTEMMODAL);
	MessageBox(NULL, s, "您的得分：", MB_SYSTEMMODAL);

	exit(0);
	}
}

Problem* hint(Problem* head, int problemNum) {
	Problem* p, * q, * h;
	char s[10];
	q = head;
	while (q->num != problemNum) {
		p = q;
		q = q->next;
	}
	h = q->next;
	wsprintf(s, "%d", q->result);
	MessageBox(NULL, s, "正确答案", MB_SYSTEMMODAL);
	while (h != NULL) {
		h->num--;
		h = h->next;
	}
	if (q == head) {
		head = q->next;
		free(q);
	}
	else if (q->next == NULL) {
		p->next = NULL;
		free(q);
	}
	else {
		p->next = q->next;
		free(q);
	}
	return head;
}

Problem* insert(Problem* head, int& problemSum) {
	int num = 1;
	Problem* p, * q, * r = NULL, * h = NULL;
	p = head;
	for (num = 1; num <= 3; num++) {
		q = (PROBLEM*)malloc(sizeof(PROBLEM));
		q->number1 = rand() % 101;
		q->number2 = rand() % 101;
		q->operation = rand() % 3 + 1;
		q->num = problemSum + num;
		switch (q->operation) {
		case 1:
			q->operation = '+';
			q->result = q->number1 + q->number2;
			break;
		case 2:
			q->operation = '-';
			q->result = q->number1 - q->number2;
			break;
		case 3:
			q->operation = '*';
			q->result = q->number1 * q->number2;
			break;
		}
		if (h == NULL) {
			h = q;
			r = q;
		}
		else {
			r->next = q;
			r = q;
			r->next = NULL;
		}
	}
	//找到当前题库链表的尾
	while (p->next != NULL) {
		p = p->next;
	}
	p->next = h;
	problemSum += 3;
	return head;
}

void save(Problem* head, USER& user, int& problemNum, int& problemSum) {
	FILE* fp;
	Problem* p;
	p = head;
	fp = fopen("game.txt", "w");
	fprintf(fp, "%d\n%d\n%d\n", user.score, problemSum, problemNum);
	while (p != NULL) {
		fprintf(fp, "%d\t%d%c%d %d\n", p->num, p->number1, p->operation, p->number2, p->result);
		p = p->next;
	}
	fclose(fp);
	MessageBox(NULL, "保存完毕", "提示", MB_OK | MB_SYSTEMMODAL);
}

PROBLEM* read(USER& user, int& problemNum, int& problemSum) {
	FILE* fp;
	Problem* p,*h=NULL,*r=NULL;
	int num = 1;
	fp = fopen("game.txt", "r");
	fscanf(fp, "%d\n%d\n%d\n", &user.score, &problemSum, &problemNum);
	printf("%d %d %d",user.score,problemSum,problemNum);
	while (num <= problemSum) {
		p = (PROBLEM*)malloc(sizeof(PROBLEM));
		fscanf(fp, "%d\t%d%c%d %d\n", &p->num, &p->number1, &p->operation, &p->number2, &p->result);
		p->next = NULL;
		if (h == NULL) {
			h = p;
			r = p;
		}
		else {
			r->next = p;
			r = p;
		}
		num++;
	}
	fclose(fp);
	return h;
		
}