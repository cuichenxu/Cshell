#include "ps.h"
void record_pwd(){
    buffer_pwd = (char *)malloc(sizeof(char)*bufsize);
    if (!buffer_pwd){
        printf("allocation error1\n");
        exit(1);
    }
    while (1){
        if(getcwd(buffer_pwd, bufsize) == NULL){
            bufsize += bufsize;
            buffer_pwd = (char *)realloc(buffer_pwd, sizeof(char)*bufsize);
            }else{
            break;
        }
    }
}

void cd_pwd(){
    chdir(buffer_pwd);
    free(buffer_pwd);
}

int fun_ps(char** char_list)
{
    record_pwd();
    mps *head,*link;

    char dir_path[] = "/proc/";
    head=trav_dir(dir_path);
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

mps *trav_dir(char *dir)
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
    struct passwd *getpwuid(uid_t uid), *pw_ptr;
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
