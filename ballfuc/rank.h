#ifndef _rank_h
#define _rank_h
typedef struct Link
{
	int score;
	char name[80]; //Ӣ��
	struct Link *next;
} link;
extern link *p;
extern int endtime;
extern char user_name[80];
void InitLink();
//??????
//???????????????????????????
void InsertElem();
//??????
//??????????????????????????????????????????????????
void DelElem();
//???????
void Fsave(link *node, char *filename);
//??????????????
#endif