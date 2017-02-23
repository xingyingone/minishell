#include "parse.h"
#include "externs.h"
#include "init.h"
#include "execute.h"
#include "builtin.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <fcntl.h>

void get_command(int i);
void getname(char *name);
void print_command();
/*
 * shell��ѭ��
 */
void shell_loop(void)
{
	while (1)
	{
		/* ��ʼ������ */
		init();
		/* ��ȡ���� */
		if (read_command() == -1)
			break;
		/* �������� */
		parse_command();
		/*print_command();*/
		/* ִ������ */
		execute_command();
	}
	
	printf("\nexit\n");
}

/*
 * ��ȡ����
 * �ɹ�����0��ʧ�ܻ��߶�ȡ���ļ�������(EOF)����-1
 */
int read_command(void)
{
	/* ���ж�ȡ���cmdline�а���\n�ַ� */
	if (fgets(cmdline, MAXLINE, stdin) == NULL)
		return -1;
	return 0;
}

/*
 * ��������
 * �ɹ����ؽ����������������ʧ�ܷ���-1
 */
int parse_command(void)
{
	/* cat < test.txt | grep -n public > test2.txt & */
	if (check("\n"))
		return 0;

	/* �ж��Ƿ��ڲ����ִ���� */
	if (builtin())
		return 0;


	/* 1��������һ�������� */
	get_command(0);
	/* 2���ж��Ƿ��������ض���� */
	if (check("<"))
		getname(infile);
	/* 3���ж��Ƿ��йܵ� */
	int i;
	for (i=1; i<PIPELINE; ++i)
	{
		if (check("|"))
			get_command(i);
		else
			break;
	}
	/* 4���ж��Ƿ�������ض���� */
	if (check(">"))
	{
		if (check(">"))
			append = 1;
		getname(outfile);
	}
	/* 5���ж��Ƿ��̨��ҵ */
	if (check("&"))
		backgnd = 1;
	/* 6���ж����������\n��*/
	if (check("\n"))
	{
		cmd_count = i;
		return cmd_count;
	}
	else
	{
		fprintf(stderr, "Command line syntax error\n");
		return -1;
	}
}

/*
 * ִ������
 * �ɹ�����0��ʧ�ܷ���-1
 */
int execute_command(void)
{
	execute_disk_command();
	return 0;
}

void print_command()
{
	int i;
	int j;
	printf("cmd_count = %d\n", cmd_count);
	if (infile[0] != '\0')
		printf("infile=[%s]\n", infile);
	if (outfile[0] != '\0')
		printf("outfile=[%s]\n", outfile);

	for (i=0; i<cmd_count; ++i)
	{
		j = 0;
		while (cmd[i].args[j] != NULL)
		{
			printf("[%s] ", cmd[i].args[j]);
			j++;
		}
		printf("\n");
	}
}

/*
 * ������������cmd[i]
 * ��ȡcmdline�е����������avline�����У�
 * ���ҽ�COMMAND�ṹ�е�args[]�е�ÿ��ָ��ָ����Щ�ַ���
 */
void get_command(int i)
{
	/*   cat < test.txt | grep -n public > test2.txt & */

	int j = 0;
	int inword;
	while (*lineptr != '\0')
	{
		/* ȥ���ո� */
		while (*lineptr == ' ' || *lineptr == '\t')
			lineptr++;

		/* ����i�������j������ָ��avptr */
		cmd[i].args[j] = avptr;
		/* ��ȡ���� */
		while (*lineptr != '\0'
			&& *lineptr != ' '
			&& *lineptr != '\t'
			&& *lineptr != '>'
			&& *lineptr != '<'
			&& *lineptr != '|'
			&& *lineptr != '&'
			&& *lineptr != '\n')
		{
				/* ������ȡ��avptrָ�����������avline */
				*avptr++ = *lineptr++;
				inword = 1;
		}
		*avptr++ = '\0';
		switch (*lineptr)
		{
		case ' ':
		case '\t':
			inword = 0;
			j++;
			break;
		case '<':
		case '>':
		case '|':
		case '&':
		case '\n':
			if (inword == 0)
				cmd[i].args[j] = NULL;
			return;
		default: /* for '\0' */
			return;
		}
	}
}

/*
 * ��lineptr�е��ַ�����str����ƥ��
 * �ɹ�����1��lineptr�ƹ���ƥ����ַ���
 * ʧ�ܷ���0��lineptr���ֲ���
 */
int check(const char *str)
{
	char *p;
	while (*lineptr == ' ' || *lineptr == '\t')
		lineptr++;

	p = lineptr;
	while (*str != '\0' && *str == *p)
	{
		str++;
		p++;
	}

	if (*str == '\0')
	{
		lineptr = p;	/* lineptr�ƹ���ƥ����ַ��� */
		return 1;
	}

	/* lineptr���ֲ��� */
	return 0;
}

void getname(char *name)
{
	while (*lineptr == ' ' || *lineptr == '\t')
		lineptr++;

	while (*lineptr != '\0'
			&& *lineptr != ' '
			&& *lineptr != '\t'
			&& *lineptr != '>'
			&& *lineptr != '<'
			&& *lineptr != '|'
			&& *lineptr != '&'
			&& *lineptr != '\n')
	{
			*name++ = *lineptr++;
	}
	*name = '\0';
}

