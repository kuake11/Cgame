#ifndef _draw_h
#define _draw_h
extern double anglepen;
void MoveForward(double Distance);
//杆转动控制函数
void TurnTo(double angle);
//杆转动控制函数
void Move(double Distance);
//杆转动控制函数
double Distance(double x1, double x2, double y1, double y2);
//距离计算函数
void BackGround();
//背景幕布 
void LSD();
//首界面绘制函数
void Drawtable();
//球桌绘制函数
void DrawSightLine(double angle, double GT_x, double GT_y, double XuXian);
//瞄准线绘制函数
//根据传入的参量（转动角度，杆头的位置，力度大小）绘制瞄准线
void DrawPowerBar(double XuXian);
//力度条绘制函数
//根据传入的力度大小参数，绘制力度条

#endif
