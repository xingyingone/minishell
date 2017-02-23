#ifndef _PARSE_H_
#define _PARSE_H_

void shell_loop(void);
int read_command(void);
int parse_command(void);
int execute_command(void);
int check(const char *str);

#endif /* _PARSE_H_ */
