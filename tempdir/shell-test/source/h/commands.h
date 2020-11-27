#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <readline/readline.h>
#include <readline/history.h> //gcc main.c -lreadline
#include "global.h"
#define MAX_LEN 20
#define BUFFER_SIZE 1024

//pwd
void record_pwd();
void cd_pwd();
int fun_ps(char** char_list);
struct ps_info *trav_dir(char dir[]);
int read_info(char d_name[],struct ps_info *p1);
void print_ps(struct ps_info *head);
void uid_to_name(uid_t uid, struct ps_info *p1);
int is_num(char *);
void print_ps(struct ps_info *head);

//define new structure
typedef struct ps_info
{
    char pname[MAX_LEN];
    char user[MAX_LEN];
    int  pid;
    int  ppid;
    char state;
    struct ps_info *next;
}mps;

//others
char *buffer_pwd;

char* get_mode(mode_t m,char* str);//显示文件读写权限函数
int _time(int year);
void time_ch(time_t num);
int fun_ls(char** char_list);
int fun_cd(char** char_list);
int fun_echo(char** char_list);
int fun_exit(char** char_list);
int fun_mkdir(char** char_list);
int fun_pwd(char** char_list);
int fun_remove_dir(char** char_list);
int fun_touch(char** char_list);
int fun_copy(char **argv);
int fun_rm(char** argv);
int fun_tree(char **argv);