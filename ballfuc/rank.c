#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rank.h"

void InitLink()
{
	p = (link *)malloc(sizeof(link));
	link *temp = p;
	int i;
	for (i = 1; i <= 5; i++)
	{
		link *a = (link *)malloc(sizeof(link));
		a->score = 0;
		strcpy(a->name, "nobody");
		a->next = NULL;
		temp->next = a;
		temp = temp->next;
	}
}
void InsertElem()
{
	int flag_model2;
	int i = 1;
	link *temp = p;
	link *Temp;
	while (i < 10)
	{
		Temp = temp;
		temp = temp->next;
		if (temp == NULL)
		{
			flag_model2 = 0;
			break;
		}
		else
		{

			flag_model2 = 1;
			if (endtime < temp->score)
				temp = temp->next;
			else
			{
				break;
			}
		}
	}
	link *c = (link *)malloc(sizeof(link));
	c->score = endtime;
	strcpy(c->name, user_name);
	if (flag_model2 == 1)
	{
		c->next = Temp->next;
		Temp->next = c;
	}
	else if (flag_model2 == 0)
		c->next = NULL;
	Temp->next = c;
}
void DelElem()
{
	int i = 1;
	link *temp = p;
	while (i < 10)
	{

		if (temp->next->next == NULL)
		{
			break;
		}
		temp = temp->next;
		i++;
	}
	link *del = temp->next;
	temp->next = temp->next->next;
	free(del);
}
void Fsave(link *node, char *filename)
{
	link *p = node;
	FILE *fp;
	if ((fp = fopen(filename, "wb")) == NULL)
	{
		return;
	}
	else
	{
		while (p != NULL)
		{
			fwrite(p, sizeof(link), 1, fp);
			p = p->next;
		}
	}
	fclose(fp);
}