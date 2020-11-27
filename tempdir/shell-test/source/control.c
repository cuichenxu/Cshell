#include "./h/control.h"
#include "string.h"
/** system ... controller*/

char *cmder[] = {
    "ls",
    "ps",
    "cd",
    "rm",
    "mv",
    "pwd",
    "help",
    "exit",
    "echo",
    "tree",
    "touch",
    "mkdir",
    "remove_dir"
};

int long_cmd(){
    return sizeof(cmder)/sizeof(char*);
}

int (fun_help)(char** char_list){
    int space=40;
    int i;
    printf("-----------------崔晨旭、崔秦、李福连的shell-------------------------\n");
    printf("-----------------The following are support:-----------------------\n");
    for ( i = 0; i < long_cmd(); i++){
        printf("%*s\n",space, cmder[i]);
    }
    printf("%*s\n", space, "-----------history");
    printf("%*s\n", space, "------simple pipes");
    printf("%*s\n", space, "---------redirects");
    printf("------------------------------------------------------------------\n");
    return 1;  
}

int (*funcs[])(char**) = {
    &fun_ls,
    &fun_ps,
    &fun_cd,
    &fun_rm,
    &fun_copy,
    &fun_pwd,
    &fun_help,
    &fun_exit,
    &fun_echo,
    &fun_tree,
    &fun_touch,
    &fun_mkdir,
    &fun_remove_dir
};

int process(char** char_list){
    pid_t pid = fork(),wpid;
    int status;
    if (pid == 0){
        if (execvp(char_list[0], char_list) == -1){
            perror("myshell ");
            exit(1);//子进程报错后销毁，返回父进程
        }
    }else if (pid < 0){
        perror("myshell ");
        exit(1);
    }else{
        do{
            wpid = waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

char *shell_readline() {
    int i = 0;
    char *buffer = malloc(sizeof(char)*bufsize);
    int c;

    if(!buffer){
        printf("allocation error\n");
        exit(1);
    }
    while (1)
    {
        c = getchar();
        if(c == EOF || c == '\n'){
            buffer[i] = '\0';
            return buffer;
        }else{
            buffer[i] = c;
            i++;
        }
        if(i >= bufsize){
            bufsize += bufsize;
            buffer = realloc(buffer, sizeof(char)*bufsize);
            if(!buffer){
                printf("allocation error\n");
                exit(1);
            }
        }
    }
}

char **cut_line(char *line){
    int i = 0;
    char **tokens = malloc(bufsize*sizeof(char*));
    char *token;

    if(!tokens){
        printf("allocation error5\n");
        exit(1);
    } 
    token = strtok(line, " ");
    while (token != NULL)
    {
        tokens[i] = token;
        i++;
        if(i >= bufsize){
            bufsize += bufsize;
            tokens = realloc(tokens, bufsize*sizeof(char*));
            if(!tokens){
                printf("allocation error6\n");
                exit(1);
            }
        }
        token = strtok(NULL, " ");
    }
    tokens[i] = NULL;
    return tokens;
}

int execute(char **char_list){
    int i;
    if(char_list[0] == NULL){
        return 1;
    }
    for ( i = 0; i < long_cmd(); i++){
        if (strcmp(char_list[0], cmder[i]) == 0){
            return (*funcs[i])(char_list); 
        }
    }
    return process(char_list);//调用进程
}

char *cut_str(int left,int right,char *line){
    char *buffer = malloc(sizeof(char)*bufsize);
    int j = 0;
    if(!buffer){
        printf("allocation error7\n");
        exit(1);
    }
    for (size_t i = left; i <= right; i++){
        buffer[j] = line[i];
        j++;
    }
    buffer[j] = '\0';
    return buffer;
}

int commandwithpipe(char *line){
    int pipeIdx;
    for (int i = 0; i < strlen(line); ++i) {
        if (line[i] == '|' && line[i+1] == ' ' && line[i-1] == ' ') {
            pipeIdx = i;
            break;
        }
    }
    if (pipeIdx+2 == strlen(line)) { // 管道命令' | '后续没有指令，参数缺失
        printf("Parameters are missing\n");
        return 1;
    }

    int fds[2];
    if (pipe(fds) == -1) {
        return 0;
    }
    int result = 0;
    pid_t pid = fork();
    if (pid == -1) {
        result = 0;
    } else if (pid == 0) { // 子进程执行单个命令
        close(fds[0]);
        dup2(fds[1], STDOUT_FILENO); // 将标准输出重定向到fds[1]
        close(fds[1]);
        char *new_str = cut_str(0,pipeIdx-2,line);
        char **simple_line = cut_line(new_str);
        if (execute(simple_line) != 1){
            result = 1;
        }
        free(new_str);
        free(simple_line);
        exit(result);
    } else { // 父进程递归执行后续命令
        int status;
        waitpid(pid, &status, 0);

        close(fds[1]);
        dup2(fds[0], STDIN_FILENO); // 将标准输入重定向到fds[0]
        close(fds[0]);
        char *new_str = cut_str(pipeIdx + 2,strlen(line),line);
        char **simple_line = cut_line(new_str);
        result = execute(simple_line);
        free(new_str);
        free(simple_line);

    }

    return result;

}

int commandWithRedi(char* line) { //可能含有重定向
    int outNum = 0;
    char *outFile = NULL;
    int endIdx = strlen(line); // 指令在重定向前的终止下标

    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == '>') { // 输出重定向
            outNum++;
            if (i+1 < strlen(line))
                outFile = &line[i+1];
            else{
                printf("Parameters are missing\n");
            }
            endIdx = i;
        }
    }
    /* 处理重定向 */
    if (outNum > 1) { // 输出重定向符超过一个
        printf("Output redirection more than one\n");
        return 1;
    }

    int result = 1;
    pid_t pid = fork();
    if (pid == -1) {
        result = 0;
    } else if (pid == 0) {
        /* 输入输出重定向 */
        if (outNum == 1){
            freopen(outFile, "w", stdout);
        }
        /* 执行命令 */
        line[endIdx] = '\0';
        char** char_list = cut_line(line);
        int stute = execvp(char_list[0], char_list);
        free(char_list);
        if (stute == -1){
            exit(1);//子进程报错后销毁，返回父进程
        };
        exit(0);
    } else {
        int status;
        waitpid(pid, &status, 0);
        int err = WEXITSTATUS(status); // 读取子进程的返回码

        if (err) { 
            printf("Error: %s\n", strerror(err));
        }
    }
    return result;
}

int execute_line(char *line){
    int sample;
    for (int i = 0; i < strlen(line); i++){
        if (line[i] == '|' && line[i+1] == ' ' && line[i-1] == ' '){
            sample = commandwithpipe(line);
            return sample;
        }
    }
    for (int j = 0; j < strlen(line); j++){
        if (line[j] == '>'){
            sample = commandWithRedi(line);
            return sample;
        }
    }
    char **simple_line = cut_line(line);
    sample = execute(simple_line);
    free(simple_line);
    return sample;
}

void fresh_pwd(){
    current_pwd_buffer = malloc(sizeof(char)*bufsize);
    if (!current_pwd_buffer){
        printf("allocation error1\n");
        exit(1);
    }
    while (1){
        if(getcwd(current_pwd_buffer, bufsize) == NULL){
            bufsize += bufsize;
           current_pwd_buffer = realloc(current_pwd_buffer, sizeof(char)*bufsize);
            }else{
            break;
        }
    }
}

int loop(int argc, char **argv) {
    // if(argc > 1 && string(argv[1])=="-d") {
    //     rl_bind_key('\t',rl_insert);//tab controller
    // }
    // rl_attempted_completion_function = autoCommand;

    char *line;
    int state = 1;
    do{
        fresh_pwd();//每次开始刷新一下当前工作路径
        //dup()用来复制参数oldfd 所指的文件描述词, 并将它返回. 此新的文件描述词和参数oldfd 指的是同一个文件, 共享所有的锁定、读写位置和各项权限或旗标.
        int s_fd_out = dup(STDOUT_FILENO); 
        int s_fd_in = dup(STDIN_FILENO);
        printf("\033[49;36mOurShell: %s\033[0m",current_pwd_buffer);//打印提示：路径
        line = readline(BEGIN(49,36)"$v$"CLOSE);  //readline是一个动态库，编译的时候需要加上　-lreadline
        if (!line){
            printf("allocation error\n");
            exit(1);
        }
        add_history(line);        

        state = execute_line(line);
        
        free(line);
        //int dup2(int odlfd, int newfd);用来复制参数oldfd 所指的文件描述词, 并将它拷贝至参数newfd 后一块返回. 
        //若参数newfd为一已打开的文件描述词, 则newfd 所指的文件会先被关闭. dup2()所复制的文件描述词, 与原来的文件描述词共享各种文件状态, 详情可参考dup().
        int n_fd_out = dup2(s_fd_out , STDOUT_FILENO);
        int n_fd_in = dup2(s_fd_in,STDIN_FILENO);
    }while(state);
    return 1;
}