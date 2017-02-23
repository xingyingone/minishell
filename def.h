#ifndef _DEF_H_
#define _DEF_H_
#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(m) \
  do \
  { \
    perror(m); \
	exit(EXIT_FAILURE); \
  } \
  while (0)

#define MAXLINE 1024	/* �����е���󳤶� */
#define MAXARG 20		/* ÿ��������Ĳ��������� */
#define PIPELINE 5		/* һ���ܵ����м������������ */
#define MAXNAME 100		/* IO�ض����ļ�������󳤶� */
#define OPEN_MAX 1024

typedef struct command
{
	char *args[MAXARG+1];	/* ����������������б� */
	int infd;
	int outfd;
} COMMAND;

#endif /* _DEF_H_ */
