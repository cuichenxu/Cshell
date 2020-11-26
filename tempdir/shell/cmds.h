#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "commands.h"
#include <readline/readline.h>
#include <readline/history.h> //gcc main.c -lreadline

#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
#define BLOD  "\001\033[1m\002"                 // 强调、加粗、高亮
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景

char *current_pwd_buffer;

/** system ... controller*/
int long_cmd();
int process(char** char_list);
char *shell_readline();
char **cut_line(char *line);
int execute(char **char_list);
char *cut_str(int left,int right,char *line);
int commandwithpipe(char *line);
int commandWithRedi(char* line);
int execute_line(char *line);
void fresh_pwd();
int loop();