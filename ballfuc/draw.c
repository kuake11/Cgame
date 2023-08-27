#include "graphics.h"
#include "extgraph.h"
#include "draw.h"
#include "imgui.h"
void MoveForward(double Distance)
{
	DrawLine(Distance * cos(anglepen), Distance * sin(anglepen));
}

void TurnTo(double angle)
{
	anglepen = angle;
}

double Distance(double x1, double x2, double y1, double y2)
{
	double length;
	length = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	return length;
}
void Move(double Distance)
{
	MovePen(GetCurrentX() + Distance * cos(anglepen), GetCurrentY() + Distance * sin(anglepen));
}
void BackGround(){
	DefineColor("background",0.980,0.921,0.843);//250,235,215
	SetPenColor("background");
	drawRectangle(0,0,GetFullScreenWidth(),GetFullScreenHeight(),1);
}
void LSD()
{
	char *a;
	DefineColor("table_color1", 0.337, 0.46, 0.18);
	SetPenSize(3);
	DefineColor("table_color2", 0.545, 0.271, 0.075);
	SetPenColor("table_color2");
	drawRectangle(3.9, 1.8, 21, 11.9, 1);
	SetPenColor("table_color1");
	drawRectangle(4.7125, 2.55, 19.375, 10.5, 1);
	
	a = GetFont();
	SetFont("ËÎÌå"); 
	SetPointSize(45);
	SetPenColor("black");
	drawLabel(12.6 ,11.8 , "×ÀÇòÓÎÏ·");
	
	SetPenColor("white");
	StartFilledRegion(1); 
	MovePen(12.0,12);
	DrawArc(0.8, 0, 360);
	EndFilledRegion();
	SetPenColor("Black");
	StartFilledRegion(1); 
	MovePen(12.0,12);
	DrawArc(0.8, 0, 360);
	EndFilledRegion();
	SetPenColor("white");
	StartFilledRegion(1); 
	MovePen(11.8,12.1);
	DrawArc(0.5, 0, 360);
	EndFilledRegion();
	SetFont(a);
	SetPointSize(45);
	SetPenColor("black"); 
	drawLabel(11.1,11.9,"8");
	SetPointSize(3);

}
void Drawtable()
{
	//Çò×À
	DefineColor("table_color1", 0.337, 0.46, 0.18);
	SetPenSize(3);
	DefineColor("table_color2", 0.545, 0.271, 0.075);
	SetPenColor("table_color2");
	drawRectangle(3.9, 1.8, 21, 12, 1);
	SetPenColor("table_color1");
	drawRectangle(4.7125, 2.55, 19.375, 10.5, 1);
	//±ß¿ò
	MovePen(5.3125, 2.55);
	DefineColor("table_color3", 0.207, 0.407, 0.00784);
	SetPenColor("table_color3");
	StartFilledRegion(1);
	DrawLine(8.5875, 0);
	DrawLine(-0.3, 0.3);
	DrawLine(-7.9875, 0);
	DrawLine(-0.3, -0.3);
	EndFilledRegion();
	MovePen(15, 2.55);
	StartFilledRegion(1);
	DrawLine(8.5875, 0);
	DrawLine(-0.3, 0.3);
	DrawLine(-7.9875, 0);
	DrawLine(-0.3, -0.3);
	EndFilledRegion();
	MovePen(4.7625, 3.1);
	StartFilledRegion(1);
	DrawLine(0, 9.4);
	DrawLine(0.35, -0.35);
	DrawLine(0, -8.7);
	DrawLine(-0.35, -0.35);
	EndFilledRegion();
	MovePen(24.1375, 3.1);
	StartFilledRegion(1);
	DrawLine(0, 9.4);
	DrawLine(-0.35, -0.35);
	DrawLine(0, -8.7);
	DrawLine(0.35, -0.35);
	EndFilledRegion();
	MovePen(5.3125, 13.05);
	StartFilledRegion(1);
	DrawLine(8.5875, 0);
	DrawLine(-0.3, -0.3);
	DrawLine(-7.9875, 0);
	DrawLine(-0.3, 0.3);
	EndFilledRegion();
	MovePen(15, 13.05);
	StartFilledRegion(1);
	DrawLine(8.5875, 0);
	DrawLine(-0.3, -0.3);
	DrawLine(-7.9875, 0);
	DrawLine(-0.3, 0.3);
	EndFilledRegion();

	SetPenSize(2);
	SetPenColor("white");
	MovePen(8.160, 2.85);
	DrawLine(0, 12.75 - 2.85);
	//Çò¶´
	DefineColor("lightgray",0.625,0.625,0.625);
	SetPenColor("lightgray");
	StartFilledRegion(0.7);
	MovePen(5.2625, 2.55);
	DrawArc(0.55, 0, 360);
	EndFilledRegion(); //×ó±ßÁ½¶´
	StartFilledRegion(0.7);
	MovePen(5.2625, 13.05);
	DrawArc(0.55, 0, 360);
	EndFilledRegion();

	StartFilledRegion(0.7);
	MovePen(14.95, 2.35);
	DrawArc(0.55, 0, 360);
	EndFilledRegion();
	StartFilledRegion(0.7);
	MovePen(14.95, 13.25); //ÓÒ±ßÁ½¶´
	DrawArc(0.55, 0, 360);
	EndFilledRegion();

	StartFilledRegion(0.7);
	MovePen(24.6375, 2.55);
	DrawArc(0.55, 0, 360);
	EndFilledRegion();
	StartFilledRegion(0.7);
	MovePen(24.6375, 13.05);
	DrawArc(0.55, 0, 360);
	EndFilledRegion();
}

void DrawSightLine(double angle, double GT_x, double GT_y, double XuXian)
{
	double pi = 3.1415926535;
	int i;
	if (angle < 0)
	{
		angle += pi;
	}
	else
	{
		angle -= pi;
	}
	MovePen(GT_x, GT_y);
	TurnTo(angle);
	Move(0.25 * 2 + 2);

	for (i = 1; i <= 10; i++)
	{
		SetPenSize(4);
		SetPenColor("Gray");
		Move(0.5);
		MoveForward(XuXian * 0.2);
	}
}

void DrawPowerBar(double XuXian)
{
	SetPenSize(3);
	SetPenColor("Red");
	drawRectangle(2, 4, 1, 8, 0);
	drawRectangle(2, 4, 1, 8 * (XuXian) / 2.2360, 1);
}
