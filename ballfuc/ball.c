#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "boolean.h"
#include "ball.h"
#define R_qiu 0.25
int count = 11;
double e = 0.99;
double stop = 0.0001;
void SetTable()
{
	SetPenSize(2);
	int i, j;
	ball[1].x = 8.16;
	ball[1].y = 7.8;

	ball[2].x = 18;
	ball[2].y = 7.8;
	ball[2].score = 4; 

	ball[3].x = 18.5;
	ball[3].y = 8.1;
	ball[3].score = 3;

	ball[4].x = 18.5;
	ball[4].y = 7.5;
	ball[4].score = 3;

	ball[5].x = 19;
	ball[5].y = 7.8;
	ball[5].score = 2;

	ball[6].x = 19;
	ball[6].y = 7.2;
	ball[6].score = 2;

	ball[7].x = 19;
	ball[7].y = 8.4;
	ball[7].score = 2;


	ball[8].x = 19.5;
	ball[8].y = 7.5;
	ball[8].score = 1;

	ball[9].x = 19.5;
	ball[9].y = 6.9;
	ball[9].score = 1;

	ball[10].x = 19.5;
	ball[10].y = 8.1;
	ball[10].score = 1;

	ball[11].x = 19.5;
	ball[11].y = 8.7;
	ball[11].score = 1;

	ball[12].x = 20;
	ball[12].y = 9.0;
	ball[12].score = 1;

	ball[13].x = 20;
	ball[13].y = 8.4;

	ball[14].x = 20;
	ball[14].y = 7.8;

	ball[15].x = 20;
	ball[15].y = 7.2;

	ball[16].x = 20;
	ball[16].y = 6.6;

	for (i = 1; i <= count; i++)
	{
		ball[i].dx = 0;
		ball[i].dy = 0;
		ball[i].is_run = 0;
		ball[i].exist = 1;
	}
}
double l_(int b1, int b2)
{
	double Distance;
	Distance = sqrt((ball[b1].x - ball[b2].x) * (ball[b1].x - ball[b2].x) + (ball[b1].y - ball[b2].y) * (ball[b1].y - ball[b2].y));
	return Distance;
}
void Crush(int n, int m)
{

	double v1x, v1y, v2x, v2y; //dx,dy模拟速度vx,vy

	v1x = ball[n].dx;
	v1y = ball[n].dy;
	v2x = ball[m].dx;
	v2y = ball[m].dy;

	double v1, v2;
	v1 = sqrt(pow(v1x, 2) + pow(v1y, 2));
	v2 = sqrt(pow(v2x, 2) + pow(v2y, 2));

	double x, y; //球心距离向量
	x = ball[m].x - ball[n].x;
	y = ball[m].y - ball[n].y;

	double s, s1, s2; //球心连线弧度制角度，n、m球速度弧度制角度（都是和x轴的夹角）
	if (fabs(x) > 0 && fabs(y) > 0)
	{
		s = atan2(y, x);
	}
	if (v1 > stop)
	{
		s1 = atan2(v1y, v1x);
	}
	if (v2 > stop)
	{
		s2 = atan2(v2y, v2x);
	}
	double T_v1_1, T_v2_1, T_v1_2, T_v2_2;
	T_v1_1 = v1 * cos(s - s1);
	T_v2_1 = v2 * cos(s - s2);

	//根据考虑恢复系数的非完全弹性碰撞（可以最真实的模拟现实台球碰撞）
	T_v1_2 = (T_v1_1 + T_v2_1) / 2 - e * (T_v1_1 - T_v2_1) / 2;
	T_v2_2 = (T_v1_1 + T_v2_1) / 2 - e * (T_v2_1 - T_v1_1) / 2;
	ball[n].dx = T_v1_2 * cos(s) + v1 * sin(s - s1) * sin(s);
	ball[n].dy = T_v1_2 * sin(s) + v1 * sin(s - s1) * cos(s);
	ball[m].dx = T_v2_2 * cos(s) + v2 * sin(s - s2) * sin(s);
	ball[m].dy = T_v2_2 * sin(s) + v2 * sin(s - s2) * cos(s);
}

void BallToWall(double BX,double BY,int i){
	if ((BX + R_qiu >= 23.7375 || BX - R_qiu <= 5.0625) &&
		(BY >= 3.45 && BY < 12.15))
	{
		ball[i].dx *= -1;
	}
	if(BY + R_qiu >= 12.75 || BY - R_qiu <= 2.85 &&
	  ((BX >= 5.3125 && BX <= 13.55) ||
	  (BX >= 15.25 && BX <= 23.2375)))
	{
		ball[i].dy *= -1;
	}
			double ballvtemp;
			if (BX >= 13.55 + 0.7 * 0.28 && BX <= 13.85 + 0.7 * 0.28)
			{
				if (BY < -BX + 16.4 + 0.7 * 0.28 || BY > BX - 0.8 - 0.7 * 0.28)
				{
					ballvtemp = ball[i].dx;
					ball[i].dx = -ball[i].dy;
					ball[i].dy = -ballvtemp;
				}
			}
			else if (BX < 13.55 + 0.7 * 0.3 && BX > 13.55)
			{
				if (BY < 2.85 + 0.7 * 0.28 || BY > 12.75 - 0.7 * 0.28)
				{
					ball[i].dx = -ball[i].dx;
					ball[i].dy = -ball[i].dy;
				}
			}

			if (BX >= 14.95 - 0.7 * 0.28 && BX <= 15.25 - 0.7 * 0.28)
			{
				if (BY > -BX + 28 + 0.7 * 0.28 || BY < BX - 12.7 - 0.7 * 0.28)
				{
					ballvtemp = ball[i].dx;
					ball[i].dx = -ball[i].dy;
					ball[i].dy = -ballvtemp;
				}
			}
			else if (BX > 15.25 - 0.7 * 0.3 && BX < 15.25)
			{
				if (BY < 2.85 + 0.7 * 0.28 || BY > 12.75 - 0.7 * 0.28)
				{
					ball[i].dx = -ball[i].dx;
					ball[i].dy = -ball[i].dy;
				}
			}

			if (BX >= 5.2625 && BX <= 5.3125)
			{
				if (BY > -BX + 18.3125 + 0.7 * 0.3 || BY < BX - 2.7125 - 0.7 * 0.3)
				{
					ballvtemp = ball[i].dx;
					ball[i].dx = -ball[i].dy;
					ball[i].dy = -ballvtemp;
				}
			}

			if (BX >= 4.1725 && BX <= 5.0625)
			{
				if (BY < -BX + 16.6725 || BY > BX - 1.0725)
				{
					ballvtemp = ball[i].dx;
					ball[i].dx = -ball[i].dy;
					ball[i].dy = -ballvtemp;
				}
			}

			if (BX >= 23.2375 && BX <= 23.5375)
			{
				if (BY < -BX + 26.0875 || BY > BX - 10.4875)
				{
					ballvtemp = ball[i].dx;
					ball[i].dx = -ball[i].dy;
					ball[i].dy = -ballvtemp;
				}
			}

			if (BX >= 23.7375 && BX <= 24.0875)
			{
				if (BY > -BX + 27.1875 || BY < BX - 11.5875)
				{
					ballvtemp = ball[i].dx;
					ball[i].dx = -ball[i].dy;
					ball[i].dy = -ballvtemp;
				}
			}
} 
bool is_crush(int i)
{

	if ((l_(i, ball[i].cloball.n) <= 2 * 0.25 + 0.02))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
