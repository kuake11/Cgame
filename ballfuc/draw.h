#ifndef _draw_h
#define _draw_h
extern double anglepen;
void MoveForward(double Distance);
//��ת�����ƺ���
void TurnTo(double angle);
//��ת�����ƺ���
void Move(double Distance);
//��ת�����ƺ���
double Distance(double x1, double x2, double y1, double y2);
//������㺯��
void BackGround();
//����Ļ�� 
void LSD();
//�׽�����ƺ���
void Drawtable();
//�������ƺ���
void DrawSightLine(double angle, double GT_x, double GT_y, double XuXian);
//��׼�߻��ƺ���
//���ݴ���Ĳ�����ת���Ƕȣ���ͷ��λ�ã����ȴ�С��������׼��
void DrawPowerBar(double XuXian);
//���������ƺ���
//���ݴ�������ȴ�С����������������

#endif
