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
double u = 0.985;	 //ģ��Ħ��ϵ��
int ball_state = 10; /* �˵�״̬��0 ת����1 �϶���2 ����;3 �ȴ�ȫ��ֹͣ������ʧ;4 [1]���붴 */
int flag_model2 = 2; //model2ʹ�ã����flag=2,score1,���flag=-2,score2;
double anglepen;
double BD_x, BD_y, BD_angle; //BUTTON_DOWN��state�����state����ʱ������λ�úͼн�
double GT_x, GT_y;			 //��ͷ
double B1_x, B1_y;			 //��1�Ű�����������
double XuXian;
double mouse_x, mouse_y, movex_1, movey_1, movex_2, movey_2, movex_4, movey_4;
char ballcolor[17][30] = {"White", "White", "Black", "Red", "Red", "Yellow", "Yellow", "Yellow", "Blue", "Blue", "Blue", "Blue", "Brown", "Brown", "Brown", "Brown", "Brown"};
int model = 0; //model1��������Ϸ��model2��˫����Ϸ��model3 ����˳������
int distate;  //distate1,�����棻distate2��ģʽѡ��;distate�������а� ;distate4˫�����а�;
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

/****�˵Ŀ��ƺ���****/
void DrawCue(double angle, double BD_move, double B1_x, double B1_y);
void RollCue();
void DrugCue();

/*COLOR BANK*/
/*    Black, Dark Gray, Gray, Light Gray, White,
 *    Red, Yellow, Green, Cyan, Blue, Magenta */
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI�ַ�����
	display();	   //ˢ����ʾ
}
/******ע������¼�******/
void KeyboardEvent(int key, int event)
{
	uiGetKeyboard(key, event); // GUI��ȡ����
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
	display();				   // ˢ����ʾ
}

/******ע������¼�******/
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
			} //ת����״̬2֮��״̬1��λ�ò������ֲ��䣬��ֹͣת��
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
					ball_state = 1; //ֻ��Ҫ�������갴��ʱ�����ꡢ�Ƕȣ������ƶ�֮����������movex_2,movey_2
				}
				//��������
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
				else if (ball_state == 1 && Distance(B1_x, GT_x, B1_y, GT_y) < R_qiu) /*���ĵ���ͷ�ľ���<=R_qiu*/
				{
					ball_state = 0; //��������������׼
					if (model == 2)
						flag_model2 = -flag_model2;
				}
				else
				{
					ball_state = 2;
					ball[1].dx = -(GT_x - B1_x) * force;
					ball[1].dy = -(GT_y - B1_y) * force;
					//��������ſ�ʱ�����ꡢ�Ƕȡ�
				}
				XuXian = 0;
			}
			break;

		default:
			break;
		}
	}
}

void DrugCue() //����϶����ƺ���
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

void RollCue() //���ת�����ƺ���
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
		DrawCue(angle, 0, B1_x, B1_y); //ֻ��ת����ƫ����Ϊ0
		DrawSightLine(angle, GT_x, GT_y, XuXian);
	}
}

void DrawCue(double angle, double BD_move, double B1_x, double B1_y) //��˻��ƺ���
{	
	MovePen(B1_x, B1_y);
	TurnTo(angle + pi * 3 / 180);
	Move(R_qiu + BD_move); //ball_state 1:�����ϸ˵�ƫ��
	
	SetPenSize(11); 
	SetPenColor("Dark Gray");
	MoveForward(8);//�˵���Ӱ 
	
	MovePen(B1_x, B1_y);
	TurnTo(angle);
	Move(R_qiu + BD_move); //ball_state 1:�����ϸ˵�ƫ����	
	
	GT_x = GetCurrentX();
	GT_y = GetCurrentY();//Ϊ�˻���׼�ߣ���¼�¸�ͷ��λ������ 
	


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
	case BallTimer: //����˶�
		for (i = 1; i <= count; i++)
		{

			//Ҫ�Ǳ仯����Ϊ�㣬�Ǿ��ر仯���ķ��򽥽��ƶ�
			ball[i].y += ball[i].dy;
			ball[i].dy *= u;

			ball[i].x += ball[i].dx;
			ball[i].dx *= u; //ÿһ֡�� dx ������һ���ӽ� 1 ����������������ѧģ��ʵ�ּ���

			v = Distance(ball[i].dx, 0, ball[i].dy, 0);
			if (fabs(v) <= 0.001 && ball_state == 2)
			{
				ball[i].dy = 0;
				ball[i].dx = 0;
				ball[i].is_run = 0;
			} //����ֵС����ֱֵ��ͣ��

			//С�����
			for (k = 1; k <= count; k++)//����С���������һ������ר�Ŵ���������� 
			{
				if ((Distance(ball[k].x - R_qiu, 4.7125, ball[k].y, 2.55) <= 0.55 ||
					 Distance(ball[k].x - R_qiu, 4.7125, ball[k].y, 12.75) <= 0.55 ||
					 Distance(ball[k].x - R_qiu, 24.0875, ball[k].y, 2.55) <= 0.55 ||
					 Distance(ball[k].x - R_qiu, 24.0875, ball[k].y, 12.75) <= 0.55 || //4�����ǵĿڴ�
					 Distance(ball[k].x - R_qiu, 14.40, ball[k].y, 2.35) <= 0.55 ||
					 Distance(ball[k].x - R_qiu, 14.40, ball[k].y, 12.95) <= 0.55) &&
					ball_state != 4) /*�м����µ���������*/
				{
					if (k == 1)//�ر���1�����붴 
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
							}//�����붴��ȥ��ǰ�������ķ�֮һ 
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
							}//������˵���û�����Բ��� is_in 
								
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
					ball[k].y = k; //�ƿ���������������������
				}
			}

			//С������
			double BX, BY ;
			BX = ball[i].x - R_qiu;
			BY = ball[i].y;
			BallToWall(BX ,BY ,i);

			//С���������ҳ�˭�Ǿ��� i �����С���ڴ˻�������ȥ������ײ
			closestN = 0;
			for (j = 1; j <= count && j != i; j++)
			{
				if (l_(i, j) < l_(i, closestN))
				{
					ball[i].cloball.n = closestN = j;
				}
			}
			//ѡ�������ҳ����������С��
			ball[i].cloball.Distance = l_(i, j);
			if (is_crush(i))
			{
				Crush(i, ball[i].cloball.n);
			}
			
			for(j = 1; j <= count && j != i; j++){//���С��ճ������ 
				double d;
				d=Distance(ball[i].x,ball[j].x,ball[i].y,ball[j].y);
				if( d < 2*R_qiu && ball_state != 4){
					double x, y; 
					x = ball[j].x - ball[i].x;
					y = ball[j].y - ball[i].y;
					double s; //�������߻����ƽǶ�
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
		for (k = 1; k <= count; k++)//���С����˶�״̬ 
		{
			if (ball[k].is_run)
			{
				is_running = 1;
			}
		}
		if (is_running == 0 && ball_state == 2)
		{
			ball_state = 0;//�ص���ת����״̬ 
			if (model == 2 && !is_in){
				flag_model2 = -flag_model2;//û���Ż��� 
				 //������㣨�����÷������ 
			}
			is_in = 0;
		}
		display();
		break;

	case countdown: //����ʱ
		if (model == 1 && minus_Time >= 0 && initialize != 1)
			minus_Time--;
		display();
		break;
	}
}
void BootScreen() //�˵�������ƺ���
{
	double fH = GetFontHeight();
	double h = fH * 2;
	double x = 4.8;
	double y = 2;
	double w = TextStringWidth("���") * 4;
	double dx = w + TextStringWidth("��");
	double dy = h * 2;
	setButtonColors("Black", "GRAY", "Light Gray", "WHITE", 1);
	if (distate == 0)
	{
		if (button(GenUIID(0), x + 2.7 * dx, y + 6.0 * dy, w, h, "��ʼ"))
		{
			distate = 1;
		}
		if (button(GenUIID(1), x + 2.7 * dx, y + 4.8 * dy, w, h, "����")){
			distate = 6;
		};
		if (button(GenUIID(2), x + 2.7 * dx, y + 3.6 * dy, w, h, "���а�"))
		{
			distate = 2;
		}
	}

	if (button(GenUIID(3), x + 2.7 * dx, y + 2.4 * dy, w, h, "�˳�"))
	{
		exit(-1);
	}

	if (distate == 1)
	{
		if (button(GenUIID(0), x + 2.7 * dx, y + 6 * dy, w, h, "������Ϸ"))
		{
			model = 1;
		}
		if (button(GenUIID(0), x + 2.7 * dx, y + 4.8 * dy, w, h, "˫����Ϸ"))
		{
			model = 2;
		}
		if (button(GenUIID(0), x + 2.7 * dx, y + 3.6 * dy, w, h, "�������"))
		{
			model = 3;
		}
		if (button(GenUIID(0), x + 2.7 * dx, y + 2.4 * dy, w, h, "�������˵�"))
		{
			distate = 0;
		}
	}

	if (distate == 2)
	{
		if (button(GenUIID(0), x + 2.7 * dx, y + 4.8 * dy, w, h, "�������а�"))
		{
			distate = 3;
		}
		if (button(GenUIID(0), x + 2.7 * dx, y + 3.6 * dy, w, h, "����������"))
		{
			distate = 0;
		}
	}
}

void InitGame() //�������ݳ�ʼ������
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


void DrawButton() //��ť����ʾ���ƺ���
{

	double fH = GetFontHeight();
	double h = fH * 2; // �ؼ��߶�
	double x = 2;
	double y = 0;
	double w = TextStringWidth("���") * 3;
	double dx = w + TextStringWidth("��") * 2;
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
		drawLabel(x, y + dy, "����ʱ");
		drawLabel(x + dx, y + dy, a);
		drawLabel(x + 3 * dx, y + dy, "�÷�");
		drawLabel(x + 4 * dx, y + dy, b);
	}
	if (model == 2)
	{
		drawLabel(x, y + dy, "���1�÷�");
		drawLabel(x + dx, y + dy, b);
		drawLabel(x + 4 * dx, y + dy, "���2�÷�");
		drawLabel(x + 5 * dx, y + dy, c);
		if (flag_model2 == 2 && is_beginGame == 1)
		{
			SetPenColor("Blue"); 
			drawLabel(x , y + dy, "���1�÷�");
			
			SetPenColor("red");
			drawLabel(x + 4 * dx, y + dy, "���2�÷�");
		}
		if (flag_model2 == -2 && is_beginGame == 1)
		{
			SetPenColor("red");
			drawLabel(x , y + dy, "���1�÷�");
			
			SetPenColor("Blue");
			drawLabel(x + 4 * dx, y + dy, "���2�÷�");
		}//��ɫ��ʾ�������ڲ�������� 
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
		drawLabel(x + 4 * dx, y + dy, "��ҵ÷�");
		drawLabel(x + 5 * dx, y + dy, d);
	} 
	if (model == 2 || model == 1 || model ==3)
	{
		if (button(GenUIID(0), x + 7 * dx, y + dy, w, h, initialize ? "��ʼ" : "���¿�ʼ"))
		{
			initialize = 0;
			is_beginGame = 1;
			InitGame();
		}
		SetPenColor("BLACK");
		if (button(GenUIID(0), x + 8 * dx, y + dy, w, h, "������Ϸ"))
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
		if (button(GenUIID(0), x + 6 * dx, y + dy, w, h, "�������˵�"))
		{
			distate = 0;
			model = 0 ;
			BootScreen();
		}
	}
	if (distate == 5)
	{
		static char name[80] = "yourname";
		drawLabel(x + 4 * dx, y + 6 * dy, "������");
		if (textbox(GenUIID(0), x + 5 * dx, y + 6 * dy, w, h, name, sizeof(name)))
		{
			strcpy(user_name, name);
			endtime = 180 - minus_Time;
		}
		if (button(GenUIID(0), x + 6 * dx, y + 6 * dy, w, h, "ȷ��"))
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
		drawLabel(x + 1.5 * dx,y + 18 * dy,"����ģʽ��180�뵹��ʱ�����������һ�֣������������һ�֣�ʱ�����10s������ʱ��������Ϸֹͣ��");	
		
		drawLabel(x + 1.5 * dx,y + 16 * dy,"˫��ģʽ�����������4�֣����������3�֣����������2�֣����������1�֣���ȫ����������Ϸֹͣ,�÷ָߵ�");
		drawLabel(x + 1.2 * dx,y + 15 * dy,"��һ�ʤ��player1���п�����һλ��һ�����δ�����������Ϊ��һλ��Ҽ�����������������������һֱ����");
		drawLabel(x + 1.2 * dx,y + 14 * dy,"���������������ҿ۳�һ�֣�������Ϊ��һλ��һ���");
		
		drawLabel(x + 1.5 * dx,y + 12 * dy,"�������ģʽ�����ռӼ��˳��Ĵ������ζԽ������ϵ����ֽ��м��㡣����һ�˽�����ķ�����ȥ��ķ������ٳ���");
		drawLabel(x + 1.2 * dx,y + 11 * dy,"�����˽�����ķ����������Ե��ĸ˽�����ķ���������õ���ҵ��ܷ֡��������������۳���ǰ�ܷ־���ֵ��25%��");
		drawLabel(x + 1.2 * dx,y + 10 * dy,"���������ĸ�ѭ����ֱ���������������Ϸ����������ģʽû��ʱ�����ƣ�");
	}
}

void EndGame() //��Ϸ����������ƺ���
{
	double fH = GetFontHeight();
	double h = fH * 2; // �ؼ��߶�
	double w = TextStringWidth("���") * 3;
	double dx = w + TextStringWidth("��");
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
			if (button(GenUIID(0), x + 1.3 + dx , y - dy, w, h, "����"))
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
		{ //����1�������������λ��
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
//������

void Main()
{
	SetWindowSize(28, 15);
	InitGraphics();
	SetWindowTitle("̨��");
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
