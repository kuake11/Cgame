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
	double x, y;   //����
	double dx, dy; //����仯������ʱ�ñ仯������С����˶��ٶȣ�
	int exist;	   //���ڴ��ڱ�־��д������ʱ��Ҫ�õģ�
	struct cloestball cloball;
	int is_run; //�˶���־
	char color[20];
	int score;
} BALL;
extern BALL ball[11];
void BallToWall(double BX,double BY,int i);
//�������ıڣ��Լ����ǵķ��� 
void SetTable();
//��ʼ����ĸ������
//�����λ�ù̶��ڳ�ʼֵ�������ٶȣ����ڴ��ڱ�־���˶���־
//�������Ȳ�����ʼ��Ϊ��
double l_(int b1, int b2);
//�����������ľ���
//���ݴ�����������ţ���������������ľ��룬���ڶ���ײ��ʵ��
void Crush(int n, int m);
//��ײʵ�ֺ���
//���ݴ�����������ţ��ҵ���Ҫ��ײ������
//��ͨ����ѧģ�ͼ���������ײ֮����ٶȣ�λ�õȵķֲ���
//Ȼ��ֵ�ֱ𴫸������򣬴Ӷ�ʵ���������ײ
bool is_crush(int i);
//�ж�С���Ƿ���Է�����ײ
//���ݴ����С����ţ��ҵ������������С�򣬸�������֮��ľ���ֵ
//�Ƿ�С�ڰ뾶�Ķ����ж�С���Ƿ���Է�����ײ��������ľ�С�ڰ뾶����
//�򷵻�true��С������ײ����֮�򷵻�False��С�򲻷�����ײ
#endif
