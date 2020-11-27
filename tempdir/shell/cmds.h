#ifdef __cplusplus
#include <algorithm>
#include <sstream>
#include <vector>
#include <iostream>
using namespace std;
/** tab commands **/
vector<string> vocabulory{"help","cp","mv","mkdir","ls","ps","pwd","exit","echo","touch","remove_dir"};
#endif
#include "commands.h"

#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
#define BLOD  "\001\033[1m\002"                 // 强调、加粗、高亮
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景

char *current_pwd_buffer;

/** tab commands **/
// char *autoCommand(const char *text, int state);
// char **autoCommand(const char* text, int start, int end);

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
extern void add_history PARAMS((const char *));
extern char *readline PARAMS((const char *));
int loop();