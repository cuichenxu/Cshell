#include "./h/commands.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <libgen.h>

/** ls **/
char* get_mode(mode_t m,char* str)//显示文件读写权限函数
{
   if(S_ISREG(m))
   	strcpy(str,"-");
   else if(S_ISDIR(m))
   	strcpy(str,"d");
   else if(S_ISCHR(m))
   	strcpy(str,"c");
   else if(S_ISBLK(m))
   	strcpy(str,"b");
   else if(S_ISFIFO(m))
   	strcpy(str,"f");
   else if(S_ISLNK(m))
   	strcpy(str,"l");
   else if(S_ISSOCK(m))
   	strcpy(str,"n");

   // 属主权限
   strcat(str,m&S_IRUSR?"r":"-");
   strcat(str,m&S_IWUSR?"w":"-");
   strcat(str,m&S_IXUSR?"x":"-");

   // 同组权限
   strcat(str,m&S_IRGRP?"r":"-");
   strcat(str,m&S_IWGRP?"w":"-");
   strcat(str,m&S_IXGRP?"x":"-");

   // 其它权限
   strcat(str,m&S_IROTH?"r":"-");
   strcat(str,m&S_IWOTH?"w":"-");
   strcat(str,m&S_IXOTH?"x":"-");

   return str;
}

int _time(int year)//计算是否是闰年函数
{
   if(year%4==0 && year%100 !=0 || year%400 == 0)
   	return 29;
   return 28;
}

void time_ch(time_t num)//通过秒数来计算日期
{
   int year=1970;
   int month =1;
   int day =1;
   num = num + 8*3600;
   while(num >= 86400)
{
   	num-=86400;
   	day++;
   	if(month==1 && day == 32)
   	{
   		month++; 
   		day =1;
   	}
   	else if(month == 2 && day ==_time(year)+1)
   	{
   		month++;
   		day =1;
   	}
   	else if(month == 3 && day == 32)
   	{
   		month++;
   		day =1;
   	}
   	else if(month == 4 && day == 31)
   	{
   		month++;
   		day=1;
   	}
   	else if(month == 5 && day == 32)
   	{
   		month++;
   		day=1;
   	}
   	else if(month == 6 && day == 31)
   	{
   		month++;
   		day=1;
   	}
   	else if(month == 7 && day == 32)
   	{
   		month++;
   		day=1;
   	}
   	else if(month == 8 && day == 32)
   	{
   		month++;
   		day=1;
   	}
   	else if(month == 9 && day == 31)
   	{
   		month++;
   		day=1;
   	}
   	else if(month == 10 && day == 32)
   	{
   		month++;
   		day=1;
   	}
   	else if(month == 11 && day == 31)
   	{
   		month++;
   		day=1;
   	}
   	else if(month == 12 && day == 32)
   	{
   		month=1;
   		day=1;
   		year++;
   	}
   	
   }
   int hour = num/3600;
   int minute =num/60 -hour*60;
   printf("%2d月 %2d %2d:%2d ",month,day,hour,minute);
}

int fun_ls(char** char_list)//主函数
{
   char* dir_name=".";
   if(char_list[1]!=NULL)
   {
   	dir_name = char_list[1];
   }
   DIR* dp=opendir(dir_name);
   if(NULL == dp)
   {
   	perror("opendir");
   	return -1;
   }
   struct dirent* de=readdir(dp);
   for(;de;de=readdir(dp))
   {
   	if('.'==de->d_name[0]) continue;
   	//通过文件名获得文件信息
   	struct stat s;
   	int ret = lstat(de->d_name,&s);
   	if(0 > ret)
   	{
   		perror("stat");
   		return -1;
   	}
   	char str[11] = {};
   	printf("%s ",get_mode(s.st_mode,str));//类型
   	struct passwd *passwd;
   	passwd = getpwuid(s.st_uid);
   	printf ("%s ", passwd->pw_name);//主名
   	struct group *group;
   	group = getgrgid(passwd->pw_gid);
   	printf ("%s ", group->gr_name); //组名
   	printf("%5lu ",s.st_size);//大小
   	time_ch(s.st_mtime);//时间
   	printf("%s\t",de->d_name);//文件名
   	printf("\n");
   }
   closedir(dp);
   return 1;
}

/** cd **/
int fun_cd(char** char_list){
    if(char_list[1] == NULL){
        printf("Please enter the correct directory\n");
    }else{
        if(chdir(char_list[1]) != 0){
            perror("myshell");
        }
    }
    return 1;
}

/** echo **/
int fun_echo(char** char_list){
    int i;
    if (char_list[1] == NULL){
        printf("Enter correct output.\n");
    }else{
        for ( i = 1; char_list[i] != NULL; i++)
        {
            printf("%s ", char_list[i] );
        }
        printf("\n");
    }
    return 1;
}

/** mkdir **/
int fun_mkdir(char** char_list){
    if(mkdir(char_list[1],0770)!=0) //数字代表含义，按位置：（文件所有者，用户组，其他用户）；1.可执行； 2.可读； 4.可读可写； 7可读可写可执行
    perror("mkdir"),exit(-1);

    printf("mkdir directory  success .\n");
    return 1;
}

/** exit **/
int fun_exit(char** char_list){

    printf("------------------------------goodbye-----------------------------\n");
    return 0;
}

void record_pwd(){
    buffer_pwd = (char*)malloc(sizeof(char)*bufsize);
    if (!buffer_pwd){
        printf("allocation error1\n");
        exit(1);
    }
    while (1){
        if(getcwd(buffer_pwd, bufsize) == NULL){
            bufsize += bufsize;
            buffer_pwd = realloc(buffer_pwd, sizeof(char)*bufsize);
            }else{
            break;
        }
    }
}
   
void cd_pwd(){
    chdir(buffer_pwd);
    free(buffer_pwd);
}

/** ps **/
int fun_ps(char** char_list)
{
    record_pwd();
    mps *head,*link;
     
    head=trav_dir("/proc/");
    if(head==NULL)
        printf("traverse dir error\n");
    print_ps(head);
 
    while(head!=NULL)        //释放链表
    {
        link=head;
        head=head->next;
        free(link);
    }
    cd_pwd();
    return 1;
}

mps *trav_dir(char dir[])                         
{
    DIR *dir_ptr;
    mps *head,*p1,*p2;
    struct dirent *direntp;
    struct stat infobuf;
 
    if((dir_ptr=opendir(dir))==NULL)
        fprintf(stderr,"dir error %s\n",dir);
    else
    {
        head=p1=p2=(struct ps_info *)malloc(sizeof(struct ps_info));    //建立链表
        while((direntp=readdir(dir_ptr)) != NULL)               //遍历/proc目录所有进程目录
        {
            if((is_num(direntp->d_name))==0)                   //判断目录名字是否为纯数字
            {
                if(p1==NULL)
                {
                    printf("malloc error!\n");
                    exit(0);
                }
 
                if(read_info(direntp->d_name,p1)!=0)         //获取进程信息
                {
                    printf("read_info error\n");
                    exit(0);
                }
                p2->next=p1;                        //插入新节点
                p2=p1;
                p1=(struct ps_info *)malloc(sizeof(struct ps_info));
            }
        }
    }
    p2->next=NULL;
    return head;
}
 
int read_info(char d_name[],struct ps_info *p1)
{
    FILE *fd;
    char dir[20];
    struct stat infobuf;
 
    sprintf(dir,"%s/%s","/proc/",d_name);
    chdir("/proc");                        //切换至/proc目录，不然stat返回-1
    if(stat(d_name,&infobuf)==-1)                     //get process USER
        fprintf(stderr,"stat error %s\n",d_name);
    else
        //p1->user=uid_to_name(infobuf.st_uid);
        uid_to_name(infobuf.st_uid, p1);
 
    chdir(dir);                         //切换到/proc/pid目录
    if((fd=fopen("stat","r"))<0)
    {
        printf("open the file is error!\n");
        exit(0);       
    }
    while(4==fscanf(fd,"%d %s %c %d\n",&(p1->pid),p1->pname,&(p1->state),&(p1->ppid)))     //读文件的前四个字段内容，并存放在相关的链表成员中
    {
        break;                                    
    }
    fclose(fd);
    return 0;
}
 
void uid_to_name(uid_t uid, struct ps_info *p1)         //由进程uid得到进程的所有者user
{
    struct passwd *getpwuid(), *pw_ptr;
    static char numstr[10];
 
    if((pw_ptr=getpwuid(uid)) == NULL)
    {
        sprintf(numstr,"%d",uid);
        strcpy(p1->user, numstr);
    }
    else
        strcpy(p1->user, pw_ptr->pw_name);
}
 
int is_num(char p_name[])
{
    int i,len;
    len=strlen(p_name);
    if(len==0) return -1;
    for(i=0;i<len;i++)
        if(p_name[i]<'0' || p_name[i]>'9')
            return -1;
    return 0;
}

void print_ps(struct ps_info *head)
{
    mps *list;
    printf("USER\t\tPID\tPPID\tSTATE\tPNAME\n");
    for(list=head;list!=NULL;list=list->next)
    {
        printf("%s\t\t%d\t%d\t%c\t%s\n",list->user,list->pid,list->ppid,list->state,list->pname);
    }
}

/** pwd **/
int fun_pwd(char** char_list){
    char *buffer = (char*)malloc(sizeof(char)*bufsize);
    if (!buffer){
        printf("allocation error1\n");
        exit(1);
    }
    while (1){
        if(getcwd(buffer, bufsize) == NULL){
            bufsize += bufsize;
            buffer = (char*)realloc(buffer, sizeof(char)*bufsize);
            if (!buffer){
                printf("allocation error2\n");
                exit(1);
                }
        }else{
            printf("current working directory : %s\n", buffer);
            free(buffer);
            return 1;
        }
    }
}

/** remove dir **/
int fun_remove_dir(char** char_list)
{
    if (char_list[1] == NULL){
        printf("Enter correct dir_name.\n");
    }else{
    const char *dir = char_list[1];
    char cur_dir[] = ".";
    char up_dir[] = "..";
    char dir_name[1024];
    DIR *dirp;
    struct dirent *dp;
    struct stat dir_stat;

    // 参数传递进来的目录不存在，直接返回
    if ( 0 != access(dir, F_OK) ) {
        return 0;
    }

    // 获取目录属性失败，返回错误
    if ( 0 > stat(dir, &dir_stat) ) {
        perror("get directory stat error");
        return -1;
    }

    if ( S_ISREG(dir_stat.st_mode) ) {  // 普通文件直接删除
        remove(dir);
    } else if ( S_ISDIR(dir_stat.st_mode) ) {   // 目录文件，递归删除目录中内容
        dirp = opendir(dir);
        while ( (dp=readdir(dirp)) != NULL ) {
            // 忽略 . 和 ..
            if ( (0 == strcmp(cur_dir, dp->d_name)) || (0 == strcmp(up_dir, dp->d_name)) ) {
                continue;
            }
            
            sprintf(dir_name, "%s/%s", dir, dp->d_name);
            char_list[1] = dir_name;
            fun_remove_dir(char_list);   // 递归调用
        }
        closedir(dirp);

        rmdir(dir);     // 删除空目录
    } else {
        perror("unknow file type!");    
    }

    }
    return 1;   
}

/** touch **/
int fun_touch(char** char_list){
    FILE *file = NULL;

    file=fopen(char_list[1],"w");
    fclose(file);
    return 1;
}

int fun_copy(char **argv) {
	int from_fd,to_fd;
	int bytes_read,bytes_write;

	char buffer[BUFFER_SIZE];
	char*ptr;

	if(argv[2]==NULL)//如果不够2个参数
	{
		fprintf(stderr,"Usage:%s fromfile tofile \n\a",argv[0]);
		exit(1);
	}

	//open original file
	if((from_fd=open(argv[1],O_RDONLY))==-1)
	{
		fprintf(stderr,"Open %sError: %s\n",argv[1],strerror(errno));
		exit(1);
	}

	printf("openning %s...\n",argv[1]);

	//create target file
	if((to_fd=open(argv[2],O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))==-1)
	{
		fprintf(stderr,"Open %sError: %s\n",argv[2],strerror(errno));
		exit(1);
	}

	printf("creating %s...\n",argv[2]);
	
	//classical codes of copy file
	while(bytes_read=read(from_fd,buffer,BUFFER_SIZE))
	{
		//error
		if((bytes_read==-1)&&(errno!=EINTR)) break;
		else if(bytes_read>0)
		{
			ptr=buffer;
			while(bytes_write=write(to_fd,ptr,bytes_read))
			{
				//error
				if((bytes_write==-1)&&(errno!=EINTR)) break;
				//all read btyes
				else if(bytes_write==bytes_read) break;
				//continue writing
				else if(bytes_write>0)
				{
					ptr+=bytes_write;
					bytes_read-=bytes_write;
				}
			}
			//erro when write
			if(bytes_write==-1) break;
		}
	}
	close(from_fd);
	close(to_fd);

	printf("%s\n","finished");

	return 1;
}

int fun_rm(char** argv) {
    if(argv[3]==NULL)
    {
        printf("Usage:%s filename\n",basename(argv[0]));
        exit(0);
    }

    if(unlink(argv[1])!=0)
    {
        perror("ulink");
        exit(-1);
    }

    printf("remove %s success!\n",basename(argv[1]));
    return 1;
}


/** tree **/
//tree
char *father_vline = "│   ";
char *block="   ";  //边界空格
char *child_level="├── ";  //中间文件c
char *last_file="└── ";  //最后一个文件
char *level_sum;
void tree_dirs(){
    int total_num = 0;
    int cur_num = 0;
    DIR *cur_dir;
    struct dirent *cur_dirent;
    cur_dir = opendir("./");  //打开当前目录

    while ((cur_dirent = readdir(cur_dir)) != NULL)   //计数
    {
        if ((strcmp(cur_dirent->d_name, ".")==0) || (strcmp(cur_dirent->d_name, "..")==0))  //不考虑 . 和 ..
        {
            continue;
        }
        if (cur_dirent->d_type == 4)
        {
            total_num++;
        }else if (cur_dirent->d_type == 10)
        {
            total_num++;
        }
    }
    closedir(cur_dir);

    cur_dir = opendir("./");  //再次打开当前目录
    while ((cur_dirent = readdir(cur_dir)) != NULL) 
    {
        if ((strcmp(cur_dirent->d_name, ".")==0) || (strcmp(cur_dirent->d_name, "..")==0))  //不考虑 . 和 ..
        {
            continue;
        }
        if (cur_dirent->d_type == 4)      //文件夹处理
        {
            cur_num++;
            if (cur_num == total_num)
            {   
                printf("%s%s\033[46;30m %s \033[0m\n", level_sum, last_file, cur_dirent->d_name);
                int len1 = strlen(level_sum);
                int len2 = strlen(block);

                strcat(level_sum, block);  //字符串拼接

                chdir(cur_dirent->d_name);  //cd name
                tree_dirs(cur_dirent->d_name);

                //删除block
                int i = len1;
                for (; i < len1+len2; i++)
                {
                    level_sum[i] = 0;
                }
                level_sum[i] = '\0';

                chdir("../");  // cd ../

            }else  //不是最后一项
            {
                printf("%s%s\033[46;30m %s \033[0m\n", level_sum, child_level, cur_dirent->d_name);
                int len1 = strlen(level_sum);
                int len2 = strlen(father_vline);

                strcat(level_sum, father_vline);
                chdir(cur_dirent->d_name);   // cd name
                tree_dirs(cur_dirent->d_name); 
                int i = len1;
                for (; i < len1+len2; i++)
                {
                    level_sum[i] = 0;
                }
                level_sum[i] = '\0';

                chdir("../");    //cd ../
            }   
              
        }else if (cur_dirent->d_type == 10)   //符号链接处理
        {
            cur_num++;
            if (cur_num == total_num)
            {
                char* buf = (char*)malloc(1024);
                readlink(cur_dirent->d_name,buf,1023);

                struct stat s_buf;
                stat(buf, &s_buf);
                if (S_ISDIR(s_buf.st_mode))
                {
                    printf("%s%s\033[46;30m %s \033[0m -> \033[46;30m %s \033[0m\n",level_sum, last_file, cur_dirent->d_name, buf);
                    free(buf);
                    int len1 = strlen(level_sum);
                    int len2 = strlen(block);
                    strcat(level_sum, block);

                    chdir(cur_dirent->d_name);

                    tree_dirs(cur_dirent->d_name); 
                    int i = len1;
                    for (; i < len1+len2; i++)
                    {
                        level_sum[i] = 0;
                    }
                    level_sum[i] = '\0';

                    chdir("../");
                }

            }else
            {
                char* buf = (char*)malloc(1024);
                readlink(cur_dirent->d_name,buf,1023);
                struct stat s_buf;
                stat(buf, &s_buf);

                if (S_ISDIR(s_buf.st_mode))
                {                 
                    printf("%s%s\033[46;30m %s \033[0m -> \033[46;30m %s \033[0m\n", level_sum, child_level, cur_dirent->d_name, buf);
                    free(buf);
                    int len1 = strlen(level_sum);
                    int len2 = strlen(father_vline);

                    strcat(level_sum, father_vline);

                    chdir(cur_dirent->d_name);
                    tree_dirs(cur_dirent->d_name); 
                    int i = len1;
                    for (; i < len1+len2; i++)
                    {
                        level_sum[i] = 0;
                    }
                    level_sum[i] = '\0';  
                    chdir("../");
                }
            }     
        }
    }
    closedir(cur_dir);
}

void tree_normal(){
    int total_num = 0;
    int cur_num = 0;
    DIR *cur_dir;
    struct dirent *cur_dirent;
    cur_dir = opendir("./");  //打开当前目录

    while ((cur_dirent = readdir(cur_dir)) != NULL)   //计数
    {
        if ((cur_dirent->d_name)[0] == '.')  //不考虑 . 和 .. 和隐藏文件
        {
            continue;
        }
        if ((cur_dirent->d_type == 4) || (cur_dirent->d_type == 10) || (cur_dirent->d_type == 8))
        {
            total_num++;
        }
    }
    closedir(cur_dir);
    
    cur_dir = opendir("./");  //再次打开当前目录
    while ((cur_dirent = readdir(cur_dir)) != NULL) 
    {
        if ((cur_dirent->d_name)[0] == '.')  //不考虑 . 和 .. 和隐藏文件
        {
            continue;
        }
        if (cur_dirent->d_type == 4)      //文件夹处理
        {
            cur_num++;
            if (cur_num == total_num)
            {   
                printf("%s%s\033[46;30m %s \033[0m\n", level_sum, last_file, cur_dirent->d_name);
                int len1 = strlen(level_sum);
                int len2 = strlen(block);

                strcat(level_sum, block);  //字符串拼接

                chdir(cur_dirent->d_name);  //cd name
                tree_normal();

                //删除block
                int i = len1;
                for (; i < len1+len2; i++)
                {
                    level_sum[i] = 0;
                }
                level_sum[i] = '\0';

                chdir("../");  // cd ../

            }else  //不是最后一项
            {
                printf("%s%s\033[46;30m %s \033[0m\n", level_sum, child_level, cur_dirent->d_name);
                int len1 = strlen(level_sum);
                int len2 = strlen(father_vline);

                strcat(level_sum, father_vline);
                chdir(cur_dirent->d_name);   // cd name
                tree_normal();
                int i = len1;
                for (; i < len1+len2; i++)
                {
                    level_sum[i] = 0;
                }
                level_sum[i] = '\0';

                chdir("../");    //cd ../
            }   
              
        }else if (cur_dirent->d_type == 10)   //符号链接处理
        {
            cur_num++;
            if (cur_num == total_num)
            {
                char* buf = (char*)malloc(1024);
                readlink(cur_dirent->d_name,buf,1023);

                struct stat s_buf;
                stat(buf, &s_buf);
                if (S_ISDIR(s_buf.st_mode))
                {
                    printf("%s%s\033[46;30m %s \033[0m -> \033[46;30m %s \033[0m\n",level_sum, last_file, cur_dirent->d_name, buf);
                    free(buf);
                    int len1 = strlen(level_sum);
                    int len2 = strlen(block);
                    strcat(level_sum, block);

                    chdir(cur_dirent->d_name);

                    tree_normal(); 
                    int i = len1;
                    for (; i < len1+len2; i++)
                    {
                        level_sum[i] = 0;
                    }
                    level_sum[i] = '\0';

                    chdir("../");
                }else
                {
                    printf("%s%s\033[34m%s\033[0m -> %s\n",level_sum, last_file, cur_dirent->d_name, buf);
                    free(buf);
                }
            }else
            {
                char* buf = (char*)malloc(1024);
                readlink(cur_dirent->d_name,buf,1023);
                struct stat s_buf;
                stat(buf, &s_buf);

                if (S_ISDIR(s_buf.st_mode))
                {                 
                    printf("%s%s\033[34m%s\033[0m -> \033[46;30m%s\033[0m\n", level_sum, child_level, cur_dirent->d_name, buf);
                    free(buf);
                    int len1 = strlen(level_sum);
                    int len2 = strlen(father_vline);

                    strcat(level_sum, father_vline);

                    chdir(cur_dirent->d_name);
                    tree_normal(); 
                    int i = len1;
                    for (; i < len1+len2; i++)
                    {
                        level_sum[i] = 0;
                    }
                    level_sum[i] = '\0';  
                    chdir("../");
                }else
                {
                    printf("%s%s\033[34m%s\033[0m -> %s\n",level_sum, child_level, cur_dirent->d_name, buf);
                    free(buf);
                }
            }     
        }else if (cur_dirent->d_type == 8)
        {
            cur_num++;
            if (cur_num == total_num)
            {
                printf("%s%s%s\n", level_sum, last_file, cur_dirent->d_name);
            }else
            {
                printf("%s%s%s\n", level_sum, child_level, cur_dirent->d_name);
            }
            
        }
        
    }
    closedir(cur_dir);
}

void tree_all(){
    int total_num = 0;
    int cur_num = 0;
    DIR *cur_dir;
    struct dirent *cur_dirent;
    cur_dir = opendir("./");  //打开当前目录

    while ((cur_dirent = readdir(cur_dir)) != NULL)   //计数
    {
        if ((strcmp(cur_dirent->d_name, ".")==0) || (strcmp(cur_dirent->d_name, "..")==0))  //不考虑 . 和 ..
        {
            continue;
        }
        if ((cur_dirent->d_type == 4) || (cur_dirent->d_type == 10) || (cur_dirent->d_type == 8))
        {
            total_num++;
        }
    }
    closedir(cur_dir);
    
    cur_dir = opendir("./");  //再次打开当前目录
    while ((cur_dirent = readdir(cur_dir)) != NULL) 
    {
        if ((strcmp(cur_dirent->d_name, ".")==0) || (strcmp(cur_dirent->d_name, "..")==0))  //不考虑 . 和 ..
        {
            continue;
        }
        if (cur_dirent->d_type == 4)      //文件夹处理
        {
            cur_num++;
            if (cur_num == total_num)
            {   
                printf("%s%s\033[46;30m %s \033[0m", level_sum, last_file, cur_dirent->d_name);
                int len1 = strlen(level_sum);
                int len2 = strlen(block);

                strcat(level_sum, block);  //字符串拼接

                chdir(cur_dirent->d_name);  //cd name
                tree_all();

                //删除block
                int i = len1;
                for (; i < len1+len2; i++)
                {
                    level_sum[i] = 0;
                }
                level_sum[i] = '\0';

                chdir("../");  // cd ../

            }else  //不是最后一项
            {
                printf("%s%s\033[46;30m %s \033[0m", level_sum, child_level, cur_dirent->d_name);
                int len1 = strlen(level_sum);
                int len2 = strlen(father_vline);

                strcat(level_sum, father_vline);
                chdir(cur_dirent->d_name);   // cd name
                tree_all();
                int i = len1;
                for (; i < len1+len2; i++)
                {
                    level_sum[i] = 0;
                }
                level_sum[i] = '\0';

                chdir("../");    //cd ../
            }   
              
        }else if (cur_dirent->d_type == 10)   //符号链接处理
        {
            cur_num++;
            if (cur_num == total_num)
            {
                char* buf = (char*)malloc(1024);
                readlink(cur_dirent->d_name,buf,1023);

                struct stat s_buf;
                stat(buf, &s_buf);
                if (S_ISDIR(s_buf.st_mode))
                {
                    printf("%s%s\033[46;30m %s \033[0m -> \033[46;30m %s \033[0m\n",level_sum, last_file, cur_dirent->d_name, buf);
                    free(buf);
                    int len1 = strlen(level_sum);
                    int len2 = strlen(block);
                    strcat(level_sum, block);

                    chdir(cur_dirent->d_name);

                    tree_all(); 
                    int i = len1;
                    for (; i < len1+len2; i++)
                    {
                        level_sum[i] = 0;
                    }
                    level_sum[i] = '\0';

                    chdir("../");
                }else
                {
                    printf("%s%s\033[46;30m %s \033[0m -> %s \n",level_sum, last_file, cur_dirent->d_name, buf);
                    free(buf);
                }
            }else
            {
                char* buf = (char*)malloc(1024);
                readlink(cur_dirent->d_name,buf,1023);
                struct stat s_buf;
                stat(buf, &s_buf);

                if (S_ISDIR(s_buf.st_mode))
                {                 
                    printf("%s%s\033[46;30m %s \033[0m -> \033[46;30m %s \033[0m\n", level_sum, child_level, cur_dirent->d_name, buf);
                    free(buf);
                    int len1 = strlen(level_sum);
                    int len2 = strlen(father_vline);

                    strcat(level_sum, father_vline);

                    chdir(cur_dirent->d_name);
                    tree_all(); 
                    int i = len1;
                    for (; i < len1+len2; i++)
                    {
                        level_sum[i] = 0;
                    }
                    level_sum[i] = '\0';  
                    chdir("../");
                }else
                {
                    printf("%s%s\033[46;30m %s \033[0m -> %s\n",level_sum, child_level, cur_dirent->d_name, buf);
                    free(buf);
                }
            }     
        }else if (cur_dirent->d_type == 8)
        {
            cur_num++;
            if (cur_num == total_num)
            {
                printf("%s%s%s\n", level_sum, last_file, cur_dirent->d_name);
            }else
            {
                printf("%s%s%s\n", level_sum, child_level, cur_dirent->d_name);
            }
            
        }
        
    }
    closedir(cur_dir);
}

void tip(){
    printf("    默认遍历展示所有非隐藏文件\n");
    printf("    可选项 -[option]:\n");
    printf("    -a -----------> 展示所有文件\n");
    printf("    -d -----------> 只展示文件夹\n");
}

int fun_tree(char** argv)
{
    level_sum = (char*)malloc(4096);   //linux路径的最大长度
    char dirName[255];  //linux文件名的最大长度
    if (argv[1] == NULL)  //不带参，当前目录
    {
        strcpy(dirName, ".");
        DIR *cur_dir;
        cur_dir = opendir(dirName);
        if (cur_dir != NULL)
        {
            printf("\033[46;30m %s \033[0m\n", dirName);
        }else
        {
            printf("\033[46;30m open dir error! \033[0m\n");
            exit(1);
        }
        closedir(cur_dir);
        chdir(dirName);  //切换到目标目录
        tree_normal();
        
    }else if (argv[2]==NULL) //一个参数
    {
        strcpy(dirName, argv[1]);
        DIR *cur_dir;
        cur_dir = opendir(dirName);
        if (cur_dir != NULL)
        {
            printf("\033[46;30m%s\033[0m\n", dirName);
        }else
        {
            printf("\033[46;30m open dir error! \033[0m\n");
            exit(1);

        }
        
        closedir(cur_dir);
        chdir(dirName);  //切换到目标目录
        tree_normal();
        
    }else if (argv[3]==NULL) //两个参数
    {
        if (strcmp(argv[1], "-a")==0)
        {
            strcpy(dirName, argv[2]);
            DIR *cur_dir;
            cur_dir = opendir(dirName);
            if (cur_dir != NULL)
            {
                printf("\033[46;30m%s\033[0m\n", dirName);
            }else
            {
                printf("\033[31mopen dir error! \033[0m\n");
                exit(1);
                
            }
            
            closedir(cur_dir);
            chdir(dirName);  //切换到目标目录
            tree_all();
            
        }else if (strcmp(argv[1], "-d")==0) 
        {
            strcpy(dirName, argv[2]);
            DIR *cur_dir;
            cur_dir = opendir(dirName);
            if (cur_dir != NULL)
            {
                printf("\033[46;30m%s\033[0m\n", dirName);
            }else
            {
                printf("\033[31mopen dir error! \033[0m\n");
                exit(1);

            }
            closedir(cur_dir);
            chdir(dirName);  //切换到目标目录
            tree_dirs();
        }else
        {
            tip();
        }
    }else //3个以上参数
    {
        printf("\033[31mtoo many arguments!\033[0m\n");
    }

    return 1;
}