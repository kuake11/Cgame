#ifndef _ball_h
#define _ball_h
extern int count;
struct cloestball
{
	int n;
	double x;
	double y;
	double Distance;
};
typedef struct balltype
{
	double x, y;   //坐标
	double dx, dy; //坐标变化量（暂时用变化量代替小球的运动速度）
	int exist;	   //桌内存在标志（写进洞的时候要用的）
	struct cloestball cloball;
	int is_run; //运动标志
	char color[20];
	int score;
} BALL;
extern BALL ball[11];
void BallToWall(double BX,double BY,int i);
//球碰到四壁，以及导角的反弹 
void SetTable();
//初始化球的各项参数
//将球的位置固定于初始值，并将速度，桌内存在标志，运动标志
//最近的球等参数初始化为零
double l_(int b1, int b2);
//计算两球球心距离
//根据传入的两球的序号，计算两个球的球心距离，用于对碰撞的实现
void Crush(int n, int m);
//碰撞实现函数
//根据传入的两球的序号，找到将要碰撞的两球
//并通过数学模型计算两球碰撞之后的速度，位置等的分布，
//然后将值分别传给两个球，从而实现两球的碰撞
bool is_crush(int i);
//判断小球是否可以发生碰撞
//根据传入的小球序号，找到距离他最近的小球，根据两者之间的距离值
//是否小于半径的二倍判断小球是否可以发生碰撞，如果球心距小于半径二倍
//则返回true，小球发生碰撞，反之则返回False，小球不发生碰撞
#endif
