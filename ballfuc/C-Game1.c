#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winuser.h>
#include <mmsystem.h>
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include "rank.h"
#include <math.h>
#include "boolean.h"
#include "genlib.h"
#include "ball.h"
#include "draw.h"
#pragma comment(lib, "WINMM.LIB")
#define pi 3.1415926535
#define count 11
int is_in = 0;
double R_qiu = 0.25;
double force = 0.25;
double u = 0.985;	 //模拟摩擦系数
int ball_state = 10; /* 杆的状态：0 转动；1 拖动；2 发射;3 等待全球停止，杆消失;4 [1]球入洞 */
int flag_model2 = 2; //model2使用，如果flag=2,score1,如果flag=-2,score2;
double anglepen;
double BD_x, BD_y, BD_angle; //BUTTON_DOWN将state０变成state１的时候鼠标的位置和夹角
double GT_x, GT_y;			 //杆头
double B1_x, B1_y;			 //【1号白球】球心坐标
double XuXian;
double mouse_x, mouse_y, movex_1, movey_1, movex_2, movey_2, movex_4, movey_4;
char ballcolor[17][30] = {"White", "White", "Black", "Red", "Red", "Yellow", "Yellow", "Yellow", "Blue", "Blue", "Blue", "Blue", "Brown", "Brown", "Brown", "Brown", "Brown"};
int model = 0; //model1，单人游戏；model2，双人游戏；model3 单人顺序入球
int distate;  //distate1,主界面；distate2，模式选择;distate单人排行榜 ;distate4双人排行榜;
int initialize = 1;
int is_beginGame = 0;
int playscore_1;
int playscore_2;
int endtime;
char user_name[80];
int is_win;
int add;
int minus_Time = 0;

int cycle = 0;
double playscore_3;
typedef enum
{
	BallTimer,
	countdown
} MyTimer;
BALL ball[11] = {0};

link *p;

void DrawButton();

/****杆的控制函数****/
void DrawCue(double angle, double BD_move, double B1_x, double B1_y);
void RollCue();
void DrugCue();

/*COLOR BANK*/
/*    Black, Dark Gray, Gray, Light Gray, White,
 *    Red, Yellow, Green, Cyan, Blue, Magenta */
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI字符输入
	display();	   //刷新显示
}
/******注册键盘事件******/
void KeyboardEvent(int key, int event)
{
	uiGetKeyboard(key, event); // GUI获取键盘
	switch (event)
	{
	case KEY_UP:
		if (key == VK_ESCAPE)
			exit(-1);
		display();
		break;

	default:
		break;
	}
	display();				   // 刷新显示
}

/******注册鼠标事件******/
void MouseEvent(int x, int y, int button, int event)
{
	uiGetMouse(x, y, button, event);

	mouse_x = ScaleXInches(x);
	mouse_y = ScaleYInches(y);
	if (model != 0)
	{
		switch (event)
		{
		case MOUSEMOVE:
			if (ball_state == 0)
			{

				movex_1 = mouse_x;
				movey_1 = mouse_y;
			} //转换成状态2之后状态1的位置参数保持不变，即停止转动
			if (ball_state == 1)
			{
				movex_2 = mouse_x;
				movey_2 = mouse_y;
			}
			if (ball_state == 4)
			{
				movex_4 = mouse_x;
				movey_4 = mouse_y;
			}
			break;

		case BUTTON_DOWN:
			if (button == LEFT_BUTTON)
			{
				if (ball_state == 0)
				{
					BD_x = movex_1;
					BD_y = movey_1;
					ball_state = 1; //只需要标记下鼠标按下时的坐标、角度，后面移动之后的坐标就用movex_2,movey_2
				}
				//蓄力过程
			}

			break;

		case BUTTON_UP:
			if (button == LEFT_BUTTON && ball_state != 2)
			{
				if (ball_state == 4)
				{
					ball_state = 0;
					if (model == 2)
						flag_model2 = -flag_model2;
				}
				else if (ball_state == 1 && Distance(B1_x, GT_x, B1_y, GT_y) < R_qiu) /*球心到杆头的距离<=R_qiu*/
				{
					ball_state = 0; //放弃发射重新瞄准
					if (model == 2)
						flag_model2 = -flag_model2;
				}
				else
				{
					ball_state = 2;
					ball[1].dx = -(GT_x - B1_x) * force;
					ball[1].dy = -(GT_y - B1_y) * force;
					//标记下鼠标放开时的坐标、角度。
				}
				XuXian = 0;
			}
			break;

		default:
			break;
		}
	}
}

void DrugCue() //球杆拖动绘制函数
{
	if (ball_state == 2 || ball_state == 4)
	{
		return;
	}
	B1_x = ball[1].x - R_qiu;
	B1_y = ball[1].y;
	double BD_move;
	BD_move = Distance(movex_2, B1_x, movey_2, B1_y) - Distance(BD_x, B1_x, BD_y, B1_y);

	if (fabs(BD_x - B1_x))
	{
		BD_angle = atan2(movey_2 - B1_y, movex_2 - B1_x);
	}
	if (BD_move < 0)
	{
		BD_move = 0;
	}
	else if (BD_move > 5)
	{
		BD_move = sqrt(5);
	}
	else
	{
		BD_move = sqrt(BD_move);
	}
	XuXian = BD_move;
	if (ball_state == 1)
	{
		DrawCue(BD_angle, BD_move, B1_x, B1_y);
		DrawSightLine(BD_angle, GT_x, GT_y, XuXian);
	}
}

void RollCue() //球杆转动控制函数
{
	if (ball_state == 2 || ball_state == 4)
	{
		return;
	}
	B1_x = ball[1].x - R_qiu;
	B1_y = ball[1].y;
	double angle;
	if (fabs(B1_x - movex_1))
	{
		angle = atan2(movey_1 - B1_y, movex_1 - B1_x);
	}
	if (ball_state == 0)
	{
		DrawCue(angle, 0, B1_x, B1_y); //只有转动，偏移量为0
		DrawSightLine(angle, GT_x, GT_y, XuXian);
	}
}

void DrawCue(double angle, double BD_move, double B1_x, double B1_y) //球杆绘制函数
{	
	MovePen(B1_x, B1_y);
	TurnTo(angle + pi * 3 / 180);
	Move(R_qiu + BD_move); //ball_state 1:触发拖杆的偏移
	
	SetPenSize(11); 
	SetPenColor("Dark Gray");
	MoveForward(8);//杆的阴影 
	
	MovePen(B1_x, B1_y);
	TurnTo(angle);
	Move(R_qiu + BD_move); //ball_state 1:触发拖杆的偏移量	
	
	GT_x = GetCurrentX();
	GT_y = GetCurrentY();//为了画瞄准线，记录下杆头的位置坐标 
	


	DefineColor("GAN", 0.8, 0.457, 0.423); //180,132,108
	SetPenColor("GAN");
	SetPenSize(11);
	MoveForward(8);
	DrawSightLine(angle, GT_x, GT_y, XuXian);
}

void mytimer(int timerID)
{
	int i, j, k;
	int closestN = 0;
	int is_running = 0;

	double v;
	switch (timerID)
	{
	case BallTimer: //球的运动
		for (i = 1; i <= count; i++)
		{

			//要是变化量不为零，那就沿变化量的方向渐渐移动
			ball[i].y += ball[i].dy;
			ball[i].dy *= u;

			ball[i].x += ball[i].dx;
			ball[i].dx *= u; //每一帧的 dx 都乘于一个接近 1 的数，用这样的数学模型实现减速

			v = Distance(ball[i].dx, 0, ball[i].dy, 0);
			if (fabs(v) <= 0.001 && ball_state == 2)
			{
				ball[i].dy = 0;
				ball[i].dx = 0;
				ball[i].is_run = 0;
			} //绝对值小于阈值直接停下

			//小球进洞
			for (k = 1; k <= count; k++)//关于小球进洞还有一个函数专门处理绘制问题 
			{
				if ((Distance(ball[k].x - R_qiu, 4.7125, ball[k].y, 2.55) <= 0.55 ||
					 Distance(ball[k].x - R_qiu, 4.7125, ball[k].y, 12.75) <= 0.55 ||
					 Distance(ball[k].x - R_qiu, 24.0875, ball[k].y, 2.55) <= 0.55 ||
					 Distance(ball[k].x - R_qiu, 24.0875, ball[k].y, 12.75) <= 0.55 || //4个桌角的口袋
					 Distance(ball[k].x - R_qiu, 14.40, ball[k].y, 2.35) <= 0.55 ||
					 Distance(ball[k].x - R_qiu, 14.40, ball[k].y, 12.95) <= 0.55) &&
					ball_state != 4) /*中间上下的两个桌袋*/
				{
					if (k == 1)//特别处理1号球入洞 
					{

						if (model == 1 && ball_state != 4)
							playscore_1--;
						if (model == 2 && ball_state != 4)
						{
							if (flag_model2 == 2)
								playscore_1--;
							if (flag_model2 == -2)
								playscore_2--;
						}
						if(model == 3 && ball_state != 4){
							if(playscore_3 >= 0){
								playscore_3 = playscore_3 * 0.75;
							}
							else{
								playscore_3 += playscore_3 * 0.25;
							}//白球入洞减去当前分数的四分之一 
						}	 
						ball_state = 4;
					}
					else
					{
						add++;
						if (model == 1)
							playscore_1 += ball[k].score;
						if (model == 2)
						{
							if (flag_model2 == 2){
								playscore_1 += ball[k].score;
								is_in = 1;
							}
								
							if (flag_model2 == -2){
								playscore_2 += ball[k].score;
								is_in = 1; 
							}//白球进了等于没进所以不加 is_in 
								
						}
						if (model == 3){
							switch(cycle){
								case 0:
									playscore_3 += k;
									break;
								case 1:
									playscore_3 -= k;
									break;
								case 2:
									playscore_3 *= k;
									break;
								case 3:
									playscore_3 /= k;
									break;
							}
							cycle = (++cycle) % 4;
						}
					}
					ball[k].exist = 0;
					ball[k].dx = 0;
					ball[k].dy = 0;
					ball[k].x = 1;
					ball[k].y = k; //移开桌面避免产生幽灵球现象
				}
			}

			//小球碰壁
			double BX, BY ;
			BX = ball[i].x - R_qiu;
			BY = ball[i].y;
			BallToWall(BX ,BY ,i);

			//小球互碰，先找出谁是距离 i 最近的小球，在此基础上再去讨论碰撞
			closestN = 0;
			for (j = 1; j <= count && j != i; j++)
			{
				if (l_(i, j) < l_(i, closestN))
				{
					ball[i].cloball.n = closestN = j;
				}
			}
			//选择排序找出距离最近的小球
			ball[i].cloball.Distance = l_(i, j);
			if (is_crush(i))
			{
				Crush(i, ball[i].cloball.n);
			}
			
			for(j = 1; j <= count && j != i; j++){//解决小球粘滞问题 
				double d;
				d=Distance(ball[i].x,ball[j].x,ball[i].y,ball[j].y);
				if( d < 2*R_qiu && ball_state != 4){
					double x, y; 
					x = ball[j].x - ball[i].x;
					y = ball[j].y - ball[i].y;
					double s; //球心连线弧度制角度
					if (fabs(x) > 0 && fabs(y) > 0)
					{
						s = atan2(y, x);
					}
					ball[i].x -= (2 * R_qiu - d) / 2 * cos(s); 
					ball[i].y -= (2 * R_qiu - d) / 2 * sin(s);
					ball[j].x += (2 * R_qiu - d) / 2 * cos(s);
					ball[j].y += (2 * R_qiu - d) / 2 * sin(s); 
				}
			}
			if (fabs(v) > 0)
			{
				ball[i].is_run = 1;
			}
			else
			{
				ball[i].is_run = 0;
			}
		}

		is_running = 0;
		for (k = 1; k <= count; k++)//检测小球的运动状态 
		{
			if (ball[k].is_run)
			{
				is_running = 1;
			}
		}
		if (is_running == 0 && ball_state == 2)
		{
			ball_state = 0;//回到杆转动的状态 
			if (model == 2 && !is_in){
				flag_model2 = -flag_model2;//没进才换人 
				 //最后都清零（都不用放括号里） 
			}
			is_in = 0;
		}
		display();
		break;

	case countdown: //倒计时
		if (model == 1 && minus_Time >= 0 && initialize != 1)
			minus_Time--;
		display();
		break;
	}
}
void BootScreen() //菜单界面绘制函数
{
	double fH = GetFontHeight();
	double h = fH * 2;
	double x = 4.8;
	double y = 2;
	double w = TextStringWidth("夸克") * 4;
	double dx = w + TextStringWidth("夸");
	double dy = h * 2;
	setButtonColors("Black", "GRAY", "Light Gray", "WHITE", 1);
	if (distate == 0)
	{
		if (button(GenUIID(0), x + 2.7 * dx, y + 6.0 * dy, w, h, "开始"))
		{
			distate = 1;
		}
		if (button(GenUIID(1), x + 2.7 * dx, y + 4.8 * dy, w, h, "帮助")){
			distate = 6;
		};
		if (button(GenUIID(2), x + 2.7 * dx, y + 3.6 * dy, w, h, "排行榜"))
		{
			distate = 2;
		}
	}

	if (button(GenUIID(3), x + 2.7 * dx, y + 2.4 * dy, w, h, "退出"))
	{
		exit(-1);
	}

	if (distate == 1)
	{
		if (button(GenUIID(0), x + 2.7 * dx, y + 6 * dy, w, h, "单人游戏"))
		{
			model = 1;
		}
		if (button(GenUIID(0), x + 2.7 * dx, y + 4.8 * dy, w, h, "双人游戏"))
		{
			model = 2;
		}
		if (button(GenUIID(0), x + 2.7 * dx, y + 3.6 * dy, w, h, "计算击球"))
		{
			model = 3;
		}
		if (button(GenUIID(0), x + 2.7 * dx, y + 2.4 * dy, w, h, "返回主菜单"))
		{
			distate = 0;
		}
	}

	if (distate == 2)
	{
		if (button(GenUIID(0), x + 2.7 * dx, y + 4.8 * dy, w, h, "单人排行榜"))
		{
			distate = 3;
		}
		if (button(GenUIID(0), x + 2.7 * dx, y + 3.6 * dy, w, h, "返回主界面"))
		{
			distate = 0;
		}
	}
}

void InitGame() //各项数据初始化函数
{
	SetTable();
	is_in = 0;
	ball_state = 0;
	if (model == 2)
		flag_model2 = 2;
	flag_model2 == 2;
	minus_Time = 180;
	playscore_1 = 0;
	if (model == 2)
	{
		playscore_1 = 0;
		playscore_2 = 0;
	}
	if(model == 3){
		playscore_3 = 0;
	} 
}


void DrawButton() //按钮及显示绘制函数
{

	double fH = GetFontHeight();
	double h = fH * 2; // 控件高度
	double x = 2;
	double y = 0;
	double w = TextStringWidth("夸克") * 3;
	double dx = w + TextStringWidth("夸") * 2;
	double dy = h;
	char a[50], b[50], c[50], d[50];
	
	setButtonColors("Black", "GRAY", "Light Gray", "WHITE", 1);

	itoa((minus_Time < 0 ? 0 : minus_Time), a, 10);
	itoa((playscore_1 < 0 ? 0 : playscore_1), b, 10);
	itoa((playscore_2 < 0 ? 0 : playscore_2), c, 10);
	sprintf(d,"%lf",playscore_3); 
	SetPenColor("RED");
	if (model == 1)
	{
		drawLabel(x, y + dy, "倒计时");
		drawLabel(x + dx, y + dy, a);
		drawLabel(x + 3 * dx, y + dy, "得分");
		drawLabel(x + 4 * dx, y + dy, b);
	}
	if (model == 2)
	{
		drawLabel(x, y + dy, "玩家1得分");
		drawLabel(x + dx, y + dy, b);
		drawLabel(x + 4 * dx, y + dy, "玩家2得分");
		drawLabel(x + 5 * dx, y + dy, c);
		if (flag_model2 == 2 && is_beginGame == 1)
		{
			SetPenColor("Blue"); 
			drawLabel(x , y + dy, "玩家1得分");
			
			SetPenColor("red");
			drawLabel(x + 4 * dx, y + dy, "玩家2得分");
		}
		if (flag_model2 == -2 && is_beginGame == 1)
		{
			SetPenColor("red");
			drawLabel(x , y + dy, "玩家1得分");
			
			SetPenColor("Blue");
			drawLabel(x + 4 * dx, y + dy, "玩家2得分");
		}//蓝色表示现在正在操作的玩家 
	}
	if(model == 3){
		SetPenColor("Blue");
		drawLabel(x, y + dy, "+");
		drawLabel(x + dx, y + dy, "-");
		drawLabel(x + 2 * dx, y + dy, "*");
		drawLabel(x + 3 * dx, y + dy, "/");
		SetPenColor("red");
		switch(cycle){
			case 0:
				drawLabel(x, y + dy, "+");
				break;
			case 1:
				drawLabel(x + dx, y + dy, "-");
				break;
			case 2:
				drawLabel(x + 2 * dx, y + dy, "*");
				break;
			case 3:
				drawLabel(x + 3 * dx, y + dy, "/");
				break;
		}
		drawLabel(x + 4 * dx, y + dy, "玩家得分");
		drawLabel(x + 5 * dx, y + dy, d);
	} 
	if (model == 2 || model == 1 || model ==3)
	{
		if (button(GenUIID(0), x + 7 * dx, y + dy, w, h, initialize ? "开始" : "重新开始"))
		{
			initialize = 0;
			is_beginGame = 1;
			InitGame();
		}
		SetPenColor("BLACK");
		if (button(GenUIID(0), x + 8 * dx, y + dy, w, h, "结束游戏"))
		{
			distate = 0;
			model = 0;
			initialize = 1;
			is_beginGame = 0;
			InitGame();
			ball_state = 10;
		}
	}
	if (distate == 3)
	{
		{
			int i = 1;
			FILE *fp;
			if ((fp = fopen("RANK1", "rb")) == NULL)
			{
				return;
			}
			else
			{
				for (; i <= 5; i++)
				{
					link *temp = (link *)malloc(sizeof(link));
					fread(temp, sizeof(link), 1, fp);
					temp = temp->next;
					if (temp == NULL)
						break;
					itoa(temp->score, d, 10);
					drawLabel(x + 4 * dx, y + (16 - i) * dy, temp->name);
					drawLabel(x + 5 * dx, y + (16 - i) * dy, d);
				}
			}
		}
	}
	if (distate == 3 || distate == 5 || distate == 6)
	{
		if (button(GenUIID(0), x + 6 * dx, y + dy, w, h, "返回主菜单"))
		{
			distate = 0;
			model = 0 ;
			BootScreen();
		}
	}
	if (distate == 5)
	{
		static char name[80] = "yourname";
		drawLabel(x + 4 * dx, y + 6 * dy, "请输入");
		if (textbox(GenUIID(0), x + 5 * dx, y + 6 * dy, w, h, name, sizeof(name)))
		{
			strcpy(user_name, name);
			endtime = 180 - minus_Time;
		}
		if (button(GenUIID(0), x + 6 * dx, y + 6 * dy, w, h, "确定"))
		{
			InsertElem();
			DelElem();
			Fsave(p, "RANK1");
		}
	}
	if(distate == 6){
		SetPenColor("table_color1");
		drawRectangle(4.7125, 2.55, 19.375, 10.5, 1);
		SetPenColor("black");
		SetPointSize(15);
		drawLabel(x + 1.5 * dx,y + 18 * dy,"单人模式：180秒倒计时，彩球进洞积一分，白球进洞倒扣一分，时间减少10s。倒计时结束，游戏停止。");	
		
		drawLabel(x + 1.5 * dx,y + 16 * dy,"双人模式：黑球进洞积4分，红球进洞积3分，黄球进洞积2分，蓝球进洞积1分，球全部进洞后。游戏停止,得分高的");
		drawLabel(x + 1.2 * dx,y + 15 * dy,"玩家获胜。player1进行开球，若一位玩家击球球未进洞，则更换为另一位玩家继续击球；若击球进洞，则可以一直击球。");
		drawLabel(x + 1.2 * dx,y + 14 * dy,"若白球进洞，该玩家扣除一分，并更换为另一位玩家击球。");
		
		drawLabel(x + 1.5 * dx,y + 12 * dy,"计算击球模式：按照加减乘除的次序依次对进洞球上的数字进行计算。即第一杆进洞球的分数减去球的分数，再乘以");
		drawLabel(x + 1.2 * dx,y + 11 * dy,"第三杆进洞球的分数，最后除以第四杆进洞球的分数，计算得到玩家的总分。若白球进洞，则扣除当前总分绝对值的25%。");
		drawLabel(x + 1.2 * dx,y + 10 * dy,"反复进行四个循环，直至所有球进洞后，游戏结束。（该模式没有时间限制）");
	}
}

void EndGame() //游戏结束界面绘制函数
{
	double fH = GetFontHeight();
	double h = fH * 2; // 控件高度
	double w = TextStringWidth("夸克") * 3;
	double dx = w + TextStringWidth("夸");
	double dy = h * 2;
	double x = 10;
	double y = 7.5;
	int a[50];
	
	int i;
	int ALL_OUT = 1;
	for( i = 2;i <= count ;i++){
		if(ball[i].exist){
			ALL_OUT = 0;
			break;
		}
	} 
	if(ALL_OUT){
		if(model == 1){
			minus_Time = 0;
		}
		if(model ==3 && is_beginGame == 1){
			drawLabel(x + dx, y, "YOUR FINAL GOAL IS");
			sprintf(a,"%lf",playscore_3);
			drawLabel(x + 2 * dx, y , a);
		}
	}
	if (minus_Time <= 0 && initialize != 1 && model == 1)
	{
		int i;
		for (i = 1; i <= count; i++)
		{
			ball[i].exist = 0;
		}
		ball_state = 11;
		if (playscore_1 < count && ball_state == 11)
		{
			SetPenColor("YELLOW");
			drawLabel(x + dx, y, "GAME OVER!YOU ARE A LOSER!");
			SetPenColor("BLACK");
			if (button(GenUIID(0), x + 1.3 + dx , y - dy, w, h, "继续"))
			{
				distate = 5;
			}
			is_win = 0;
		}
		else if (add >= count && ball_state == 11)
		{
			SetPenColor("BLUE");
			drawLabel(x + dx, y, "YOU ARE TEH WINNER!");
			SetPenColor("BLACK");
			is_win = 1;
		}
	}
	if (model == 2 && initialize != 1 && add >= count)
	{
		ball_state = 12;
		if (playscore_1 < playscore_2 && ball_state == 12)
		{
			SetPenColor("YELLOW");
			drawLabel(x + dx, y, "GAME OVER");
			drawLabel(x + dx, y + 1, "The Winner is Player2");
			SetPenColor("BLACK");
		}
		else if (playscore_1 > playscore_2 && ball_state == 12)
		{
			SetPenColor("YELLOW");
			drawLabel(x + dx, y, "GAME OVER!");
			drawLabel(x + dx, y + 1, "The Winner is Player2");
			SetPenColor("BLACK");
		}
		else if (playscore_1 == playscore_2 && ball_state == 12)
		{
			SetPenColor("YELLOW");
			drawLabel(x + dx, y, "GAME OVER!");
			drawLabel(x + dx, y + 1, "A Dead Heat");
			SetPenColor("BLACK");
		}
	}
}
void display()
{

	DisplayClear();
	BackGround();
	if (model == 0)
	{
		LSD(); 
		BootScreen();
	}
	if (model == 1 || model == 2 || model == 3)
	{
		DisplayClear();
		BackGround();
		DrawButton();
		Drawtable();
		DrawPowerBar(XuXian);
		RollCue();
		DrugCue();
		
		SetPenSize(2);
		SetPenColor("Blue");

		BallGoToHole();
		EndGame();
	}
	if (distate == 3 || distate == 5|| distate == 6)
	{
		DrawButton();
	}
}

void BallGoToHole()
{
	int i;
	char a[50];
	for (i = 1; i <= count; i++)
	{
	
			if (ball[i].exist)
			{
				if (ball_state == 4)
				{
					ball[1].x = movex_4 + R_qiu;
					ball[1].y = movey_4;
				}
				SetPenColor("Dark Gray");
				MovePen(ball[i].x+0.08,ball[i].y);
				DrawArc(R_qiu ,0 ,360);
				SetPenColor(ballcolor[i]);
				StartFilledRegion(0.9);
				MovePen(ball[i].x, ball[i].y);
				DrawArc(R_qiu, 0, 360);
				EndFilledRegion();
						
			if(model == 3){
				if (i != 2)
					SetPenColor("Black");
				else
					SetPenColor("White");
				itoa(i, a, 10);
				drawLabel(ball[i].x - R_qiu - 0.1, ball[i].y - 0.1, a);
			}
		}

		if (ball[1].exist == 0 && ball_state == 4)
		{ //处理1号球进洞，重置位置
			ball[1].exist = 1; 
			ball[1].x = movex_4 + R_qiu;
			ball[1].y = movey_4;
			SetPenColor("Dark Gray");
			MovePen(ball[i].x+0.08,ball[i].y);
			DrawArc(R_qiu ,0 ,360);
			SetPenColor(ballcolor[i]);
			StartFilledRegion(0.9);
			MovePen(ball[1].x, ball[1].y);
			DrawArc(R_qiu, 0, 360);
			EndFilledRegion();
			if(model == 1){
				minus_Time -= 10;
			} 
			if(model == 3){
				SetPenColor("Black");
				itoa(1, a, 10);
				drawLabel(ball[i].x - R_qiu - 0.1, ball[i].y - 0.1, a);
			}
		}
	}
}
//链表部分

void Main()
{
	SetWindowSize(28, 15);
	InitGraphics();
	SetWindowTitle("台球");
	PlaySound(TEXT("Mark Petrie - Go Time.wav"), NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	registerTimerEvent(mytimer);
	startTimer(BallTimer, 1);
	startTimer(countdown, 1000);
	registerKeyboardEvent(KeyboardEvent);
	registerCharEvent(CharEventProcess);
	registerMouseEvent(MouseEvent);
	InitLink();
	Fsave(p, "RANK1");
	display();
}
