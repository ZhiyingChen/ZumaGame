#include <conio.h>
#include <time.h>
#include<math.h>
#include "LinkList.h"
#include "ball.h"
#include<Windows.h>
#include<mmsystem.h>
#include <windows.h>
#include <crtdbg.h>//add this header file to detect memory leaks
#include<stack>
#include <time.h>
#include "MySort.h"
#pragma comment(lib,"winmm.lib")
using namespace std;


#define WINDOWWITH 600
#define WINDOWHEIGHT 600
#define MAXBALL 70
#define colornum 6
COLORREF ballColorTable[] = {
MAGENTA,RED, YELLOW, BLUE, GREEN,BROWN
};
IMAGE bgimg, startimg, startgameimg, exitgameimg;

const COLORREF mapMainColor = WHITE;
const COLORREF mapLineColor = RGB(225, 225, 225);

int score = 0; // 得分
int restart = 0;
U_Score users[100]; //记录用户和分数


void LIMG(void);
void startMenu();
//初始化球链表
void initBallList(Node* head, int n);
//更新球链中球的位置
void updateBallPos(Node* head);
void addBall(Node* head, int n);
void newStart(int* X, int* Y);
void updateChainPos(Node* head, int startX, int startY);
//球碰撞检测
//bool collisionDetection(Node* head, ball b,bool* sameColor,int* id)
bool collisionDetection(Node* head, ball b, int* id, int* n);
//检测消除后还有没有相同的碰在一起
bool aftercollision(Node* head, int id, int* newid, int* n);
//删除相同颜色的
void delesame(Node* head, int id, int n);
//绘制球表
void drawBallList(Node* head);
//绘制碰撞球
void drawColBall(ball* b, int x, int y);
void winMenu();
void DeleleLastChar(char* a);
void AddLastChar(char* a);


int main()
{
	//定义文件操作的变量
	FILE* fp = NULL;
	FILE* fp2 = NULL;
	errno_t err = 0;
	errno_t err2 = 0;
	int n_user;
	int i;
	int user_s_int;

	char user_num[20];
	char new_user_num[20];
	char user_name[20];
	char temp_name[20];
	char user_s[20];
	char temp_s[20];
	int temp_user_s;
	

	/*用来读取GameRanking文件放入users中*/
	err = fopen_s(&fp, "GameRanking.txt", "r");

	if (err != 0) {
		printf("No such file.");
		return 0;
	}

	fgets(user_num, 20, fp);

	DeleleLastChar(user_num);
	sscanf_s(user_num, "%d", &n_user);

	for (i = 0; i < n_user; i++) {
		fgets(user_name, 20, fp);
		if (feof(fp))break;
		strcpy_s(users[i].name, user_name);

		fgets(user_s, 20, fp);
		if (feof(fp))break;
		sscanf_s(user_s, "%d", &user_s_int);
		users[i].score = user_s_int;
	}
	fclose(fp);
	/*************************************************************/

	//给users排序
	Mysort(users, n_user);


	/*将users放入GameRanking中*/
	err2 = fopen_s(&fp2, "GameRanking.txt", "w");

	if (err2 != 0) {
		printf("No such file.");
		return 0;
	}

	_itoa_s(n_user, new_user_num, 20, 10);
	AddLastChar(new_user_num);
	fputs(new_user_num, fp2);

	for (i = 0; i < n_user; i++) {
		strcpy_s(temp_name, users[i].name);
		fputs(temp_name, fp2);

		temp_user_s = users[i].score;
		_itoa_s(temp_user_s, temp_s, 20, 10);
		AddLastChar(temp_s);
		fputs(temp_s, fp2);

		//printf("%s%d\n", users[i].name, users[i].score);
	}
	fclose(fp2);
	/********************************************************/


	// 初始化绘图窗口
	LIMG();
	initgraph(WINDOWWITH, WINDOWHEIGHT);
	restart = 0;
	startMenu();
	
	label: 
	// 定义字符串缓冲区，并接收用户输入
	char name[20];
	InputBox(name, 20, "请输入用户名：");
	score = 0; // 得分

	while(restart==1) {
		//初始球链中球的个数
		int origin_n = 1, total_n = origin_n;
		int bigSleep = 1500;            //大循环沉睡的秒数
		int smallSleep = 10;
		int smallLoop = bigSleep / smallSleep;

		

		TCHAR str[] = _T("Your Score:");


		srand(time(0));
		

		//setbkcolor(BLACK);
		cleardevice();
		putimage(0, 0, &bgimg);
		setbkmode(TRANSPARENT);
		settextcolor(WHITE);
		settextstyle(20, 0, _T("黑体"));
		outtextxy(5, 5, str);
		char s[5];
		sprintf_s(s, _T("%d"), score);
		outtextxy(120, 5, s);
		//初始化球链
		Node* head = NULL;
		head = CreateEmptyList();
		initBallList(head, origin_n);
		updateChainPos(head, 100, 200);
		//print(head);
		drawBallList(head);

		//画碰撞球
		ball cball;
		cball.c = rand() % colornum;
		float ballDirction = -1;
		float speed = 10;
		drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
		mciSendString(_T("open zuma/background.mp3 alias back"), nullptr, 0, nullptr);	//文件路径采用相对路径	alias 取别名
		mciSendString(_T("play back repeat"), 0, 0, 0);		//repeat循环播放

		//事件循环，处理鼠标、时间事件
		MOUSEMSG m;
		bool ballMoving = FALSE;

		while (true)
		{
			//无用户操作的更新,更新球链，让球链运动起来
			Node* p;
			int X, Y, flag, i = 0;

			BeginBatchDraw();



			if (total_n <= MAXBALL) {          //如果没达到最大值，就不断补上新的球(相当于游戏进行了一轮)
				addBall(head, 1);
				total_n++;

				p = head->prior;

				if (p->data.x == 500 && p->data.y == 380) {
					ListDeleteLast(head);
				}


				updateChainPos(head, 100, 200);
				flag = 0;
			}
			else {

				ListDeleteLast(head);
				p = head->next;

				if (p == head) {   //如果只剩一个头结点，就重置
					users[n_user].score = score;
					AddLastChar(name);
					strcpy_s(users[n_user].name, name);
					n_user++;

					//给users排序
					Mysort(users, n_user);


					/*将users放入GameRanking中*/
					err2 = fopen_s(&fp2, "GameRanking.txt", "w");

					if (err2 != 0) {
						printf("No such file.");
						return 0;
					}

					_itoa_s(n_user, new_user_num, 20, 10);
					AddLastChar(new_user_num);
					fputs(new_user_num, fp2);

					for (i = 0; i < n_user; i++) {
						strcpy_s(temp_name, users[i].name);
						fputs(temp_name, fp2);

						temp_user_s = users[i].score;
						_itoa_s(temp_user_s, temp_s, 20, 10);
						AddLastChar(temp_s);
						fputs(temp_s, fp2);

						printf("%s%d\n", users[i].name, users[i].score);
					}
					fclose(fp2);
					/********************************************************/

					winMenu();
				
					goto label;
				}
				else
					X = p->data.x, Y = p->data.y;
				newStart(&X, &Y);
				updateChainPos(head, X, Y);
				flag = 1;
			}



			for (i = 0; i < smallLoop; i++) {
				putimage(0, 0, &bgimg);
				setbkmode(TRANSPARENT);
				settextcolor(WHITE);
				settextstyle(20, 0, _T("黑体"));
				outtextxy(5, 5, str);
				char s[5];
				sprintf_s(s, _T("%d"), score);
				outtextxy(120, 5, s);
				//有用户操作的更新
				// 获取一条鼠标消息
				if (MouseHit())
				{
					m = GetMouseMsg();

					switch (m.uMsg)
					{
					case WM_MOUSEMOVE:
						break;

					case WM_LBUTTONDOWN:

						//更新球的运动方向，球开始运动
						if (!ballMoving)
						{
							ballDirction = float(m.x - WINDOWWITH / 2) / float(WINDOWHEIGHT - m.y);
							ballMoving = TRUE;
							mciSendString(_T("open zuma/Jump.mp3 alias jump"), nullptr, 0, nullptr);	//文件路径采用相对路径	alias 取别名
							mciSendString(_T("play jump wait"), 0, 0, 0);
							mciSendString(_T("close jump"), 0, 0, 0);

						}
						break;

					case WM_RBUTTONUP:
						//closegraph();
						//DestroyList(head);
						//return 0;	// 按鼠标右键退出程序

						users[n_user].score = score;
						AddLastChar(name);
						strcpy_s(users[n_user].name, name);
						n_user++;

						//给users排序
						Mysort(users, n_user);


						/*将users放入GameRanking中*/
						err2 = fopen_s(&fp2, "GameRanking.txt", "w");

						if (err2 != 0) {
							printf("No such file.");
							return 0;
						}

						_itoa_s(n_user, new_user_num, 20, 10);
						AddLastChar(new_user_num);
						fputs(new_user_num, fp2);

						for (i = 0; i < n_user; i++) {
							strcpy_s(temp_name, users[i].name);
							fputs(temp_name, fp2);

							temp_user_s = users[i].score;
							_itoa_s(temp_user_s, temp_s, 20, 10);
							AddLastChar(temp_s);
							fputs(temp_s, fp2);

							printf("%s%d\n", users[i].name, users[i].score);
						}
						fclose(fp2);
						/********************************************************/

						winMenu();

						goto label;
						
						
					}
				}



				int id, n = 0, k = 0;
				ball a;

				bool collision = collisionDetection(head, cball, &id, &n);
				if (collision)
				{
					if (n > 2) {
						score += (n - 2);
						delesame(head, id, n);
						if (flag) updateChainPos(head, X, Y);
						else updateChainPos(head, 100, 200);

						FlushBatchDraw();
						cleardevice();
						putimage(0, 0, &bgimg);
						setbkmode(TRANSPARENT);
						settextcolor(WHITE);
						settextstyle(20, 0, _T("黑体"));
						outtextxy(5, 5, str);
						char s[5];
						sprintf_s(s, _T("%d"), score);
						outtextxy(120, 5, s);
						drawBallList(head);

						int newid = -1;                      //消除之后还重复的
						bool afsameornot;
						while (1) {
							n = 0;
							afsameornot = aftercollision(head, id, &newid, &n);
							if (n > 2) {
								score += 2 * (n - 2);    //分数加倍
								delesame(head, newid, n);
								id = newid;

								if (flag) updateChainPos(head, X, Y);
								else updateChainPos(head, 100, 200);

								FlushBatchDraw();
								Sleep(600);

								cleardevice();
								putimage(0, 0, &bgimg);
								setbkmode(TRANSPARENT);
								settextcolor(WHITE);
								settextstyle(20, 0, _T("黑体"));
								outtextxy(5, 5, str);
								char s[5];
								sprintf_s(s, _T("%d"), score);
								outtextxy(120, 5, s);
								drawBallList(head);
							}
							else
								break;
						}


					}
					else
						if (flag) updateChainPos(head, X, Y);
						else updateChainPos(head, 100, 200);

					cleardevice();
					putimage(0, 0, &bgimg);
					setbkmode(TRANSPARENT);
					settextcolor(WHITE);
					settextstyle(20, 0, _T("黑体"));
					outtextxy(5, 5, str);
					char s[5];
					sprintf_s(s, _T("%d"), score);
					outtextxy(120, 5, s);
					drawBallList(head);


					cball.c = rand() % colornum;
					drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
					ballMoving = FALSE;

				}
				else {
					cleardevice();
					putimage(0, 0, &bgimg);
					setbkmode(TRANSPARENT);
					settextcolor(WHITE);
					settextstyle(20, 0, _T("黑体"));
					outtextxy(5, 5, str);
					char s[5];
					sprintf_s(s, _T("%d"), score);
					outtextxy(120, 5, s);
					drawBallList(head);
				}


				//检测碰撞球是否超出范围
				if (cball.x > WINDOWWITH || cball.x <0 || cball.y > WINDOWHEIGHT || cball.y < 0)
				{
					cball.c = rand() % colornum;
					drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
					ballMoving = FALSE;
				}

				//移动、绘制碰撞球
				if (ballMoving == TRUE)
				{
					drawColBall(&cball, cball.x += (speed * ballDirction), cball.y -= speed);
				}
				else
				{
					drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
				}

				FlushBatchDraw();
				Sleep(smallSleep);

			}
			EndBatchDraw();
		}
	

	}

	closegraph();
	

}

void LIMG(void) {
	loadimage(&bgimg, _T("zuma/background.jpg"), WINDOWWITH, WINDOWHEIGHT);
	loadimage(&startimg, _T("zuma/zumastart.jpg"), WINDOWWITH, WINDOWHEIGHT);
	loadimage(&startgameimg, _T("zuma/text1.jpg"), WINDOWWITH*0.15, WINDOWHEIGHT*0.1);
	loadimage(&exitgameimg, _T("zuma/text3.jpg"), WINDOWWITH * 0.15, WINDOWHEIGHT * 0.1);
}


//初始化球链表
void initBallList(Node* head, int n)
{
	int i;
	ball b;

	for (i = 0; i < n; ++i) {
		//b.c = rand() % 6;
		b.c = i % colornum;
		ListInsert(head, 0, b);

	}
}

//更新球链中球的位置
void updateBallPos(Node* head)
{
	Node* p = head;
	int x = 10;
	int y = WINDOWHEIGHT / 2;
	while (p->next != head)
	{
		p = p->next;

		p->data.x = x;
		x = x + BALLRADIUS * 2;
		p->data.y = y;
	}

}

void addBall(Node* head, int n) {
	int i;
	ball b;
	for (i = 0; i < n; i++) {
		b.c = rand() % colornum;
		//b.c = i % 6;
		ListInsert(head, 0, b);
	}
}

void newStart(int* X, int* Y) {
	switch (*Y) {
	case 200: {
		if (*X == 500) {
			*Y += 20;

		}
		else {
			*X += 20;
		}
		break;
	}

	case 220: {
		*Y += 20;

		break;
	}
	case 240: {
		*Y += 20;

		break;
	}

	case 260: {
		if (*X == 100) {
			*Y += 20;

		}
		else {
			*X -= 20;
		}
		break;

	}

	case 280: {
		*Y += 20;
		break;
	}
	case 300: {
		*Y += 20;
		break;
	}
	case 320: {
		if (*X == 500) {
			*Y += 20;

		}
		else {
			*X += 20;

		}
		break;
	}

	case 340: {
		*Y += 20;
		break;
	}

	case 360: {
		*Y += 20;
		break;
	}

	}
}

void updateChainPos(Node* head, int startX, int startY) {
	Node* p = head->next;
	int X = startX, Y = startY;

	while (p != head) {
		p->data.x = X;
		p->data.y = Y;
		p = p->next;
		newStart(&X, &Y);
	}


}
//球碰撞检测
//bool collisionDetection(Node* head, ball b,bool* sameColor,int* id)
bool collisionDetection(Node* head, ball b, int* id, int* n)
{
	Node* p = head->next, * q1, * q2;
	int dist = 1000, distaf = 1000;
	int index = 0;
	int cbind = -1;
	int i1 = 0, i2 = i1;

	while (p != head)
	{
		dist = (p->data.x - b.x) * (p->data.x - b.x) + (p->data.y - b.y) * (p->data.y - b.y);
		distaf = (p->next->data.x - b.x) * (p->next->data.x - b.x) + (p->next->data.y - b.y) * (p->next->data.y - b.y);
		if (dist <= pow(2 * BALLRADIUS, 2))
		{
			if (distaf < 500) {  //如果和后面的球距离近，就插入后面
				cbind = index + 1;
				ListInsert(head, cbind, b);

				q1 = q2 = p->next;   //q1,q2 均指向新插入的球
				i1 = i2 = cbind;	//i1,i2记录新插入的球的坐标
			}
			else {
				cbind = index;
				ListInsert(head, cbind, b);

				q1 = q2 = p->prior;   //q1,q2 均指向新插入的球
				i1 = i2 = cbind;	//i1,i2记录新插入的球的坐标
			}

			while (q1 != head) {
				if (q1->data.c == b.c) {
					q1 = q1->prior;
					i1--;
				}
				else
					break;
			}
			i1++;
			*id = i1;
			while (q2 != head) {
				if (q2->data.c == b.c) {
					q2 = q2->next;
					i2++;
				}
				else
					break;
			}
			i2--;
			*n = i2 - i1 + 1;
			return TRUE;
		}
		p = p->next;
		index++;
	}



	return FALSE;

}

//检测消除后还有没有相同的碰在一起
bool aftercollision(Node* head, int id, int* newid, int* n)
{
	Node* p = head->next, * q1, * q2;
	int dist = 0;
	int index = 0;
	int cbind = -1;
	int i1 = 0, i2 = i1;
	int k = 0;

	if (id) id--;
	while (k < id) {
		p = p->next;    //将p指向指标为id的球
		k++;
	}

	q1 = q2 = p;   //q1,q2 均指向指标为id的球
	i1 = i2 = id;	//i1,i2记录id的坐标

	while (q1 != head) {
		if (q1->data.c == p->data.c) {
			q1 = q1->prior;
			i1--;
		}
		else
			break;
	}
	i1++;
	*newid = i1;

	while (q2 != head) {
		if (q2->data.c == p->data.c) {
			q2 = q2->next;
			i2++;
		}
		else
			break;
	}
	i2--;
	*n = i2 - i1 + 1;

	if (i2 - i1 + 1 > 1)
		return TRUE;
	else
		return FALSE;
}

//删除相同颜色的
void delesame(Node* head, int id, int n) {
	//n++;
	int k;
	for (k = 0; k < n; k++) {
		ListDelete(head, id);
	}

}
//绘制球表
void drawBallList(Node* head)
{
	Node* p;
	ball b;

	p = head;

	while (p->next != head)
	{
		p = p->next;
		b = p->data;

		setcolor(WHITE); // 小球的线条为白色
		setfillcolor(ballColorTable[b.c]); // 小球内部位颜色填充
		fillcircle(b.x, b.y, BALLRADIUS);


	
	}
}

//绘制碰撞球
void drawColBall(ball* b, int x, int y)
{
	b->x = x;
	b->y = y;

	setcolor(WHITE);
	setfillcolor(ballColorTable[b->c]);
	fillcircle(b->x, b->y, BALLRADIUS);


}

void startMenu(void) {
	putimage(0, 0, &startimg);

	putimage(WINDOWWITH * 0.45, WINDOWHEIGHT * 0.6, &startgameimg);
	putimage(WINDOWWITH * 0.45, WINDOWHEIGHT * 0.7, &exitgameimg);
	
	MOUSEMSG m;//定义鼠标信息
	while (1)//这个函数用于检测当前是否有鼠标信息
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= WINDOWWITH * 0.45 && m.x <= WINDOWWITH * 0.6 && m.y >= WINDOWHEIGHT * 0.6 && m.y < WINDOWHEIGHT * 0.7)
		{

			restart = 1;

			break;
		}
	
		else if (m.uMsg == WM_LBUTTONDOWN && m.x >= WINDOWWITH * 0.45 && m.x <= WINDOWWITH * 0.6 && m.y >= WINDOWHEIGHT * 0.7 && m.y < WINDOWHEIGHT * 0.8)
		{
			restart = 0;
			exit(0);
			closegraph();


		}
		
	}
}

void winMenu()
{
	putimage(0, 0, &bgimg);	// 显示背景
	setbkmode(TRANSPARENT);

	settextcolor(WHITE);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(WINDOWWITH * 0.1, WINDOWHEIGHT * 0.2, "Congratulations！Your Score：");
	char s[5];
	sprintf_s(s, "%d", score);
	outtextxy(WINDOWWITH * 0.82, WINDOWHEIGHT * 0.2, s);

	settextcolor(BLUE);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(WINDOWWITH * 0.15, WINDOWHEIGHT * 0.4, "Do you want to play again?");


	settextcolor(RED);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(WINDOWWITH * 0.2, WINDOWHEIGHT * 0.5, "再玩一次");

	settextcolor(RED);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(WINDOWWITH * 0.6, WINDOWHEIGHT * 0.5, "不玩啦");

	FlushBatchDraw();
	Sleep(2);


	MOUSEMSG m;//定义鼠标信息
	while (1)//这个函数用于检测当前是否有鼠标信息
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= WINDOWWITH * 0.2 && m.x <= WINDOWWITH * 0.4 && m.y >= WINDOWHEIGHT * 0.5 && m.y <= WINDOWHEIGHT * 0.7)
		{

			restart = 1;
			break;
		}
		else if (m.uMsg == WM_LBUTTONDOWN && m.x >= WINDOWWITH * 0.6 && m.x <= WINDOWWITH * 0.8 && m.y >= WINDOWHEIGHT * 0.5 && m.y <= WINDOWHEIGHT * 0.7)
		{
			restart = 0;
			exit(0);
			closegraph();
			break;
			

			
		}
	}

	

}

void DeleleLastChar(char* a) {
	int k_loop = 0;
	while (a[k_loop] != '\n') k_loop++;
	a[k_loop] = '\0';
}

void AddLastChar(char* a) {
	int k_loop = 0;
	while (a[k_loop] != '\0') k_loop++;
	a[k_loop] = '\n';
	a[k_loop + 1] = '\0';
}