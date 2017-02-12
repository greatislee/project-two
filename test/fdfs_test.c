#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>

#include"../include/make_log.h"

#define FDFS_TEST_MODULE "test"
#define FDFS_TEST_PROC "fdfs_test"

#define FILE_ID_LEN (256) 

int main(int argc, char *argv[])
{
    char fileid[FILE_ID_LEN] = {0};
    if(argc < 2)
    {
        printf("./a.out filename\n");
        exit(0);
    }
    //创建管道
    int fd[2];
    int ret = pipe(fd);
    if (ret == -1) 
    {
       perror("pipe error");
       exit(1);
    }
    //创建子进程
    pid_t pid = fork();
    if (pid==-1)
    {
        perror("fork error");
        exit(1);
    }     
    //子进程关闭读端 
    if(pid == 0)
    {
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        ret = execlp("fdfs_upload_file","sdsd","/etc/fdfs/client.conf",argv[1],NULL);
        if (ret==-1)
        {
            LOG(FDFS_TEST_MODULE,FDFS_TEST_PROC,"exec fdfs_upload_file error");
            exit(1);
        }
    }
    //父进程关闭写端 重定向标准输出 连接管道的读端
    if(pid > 0) 
    {
        close(fd[1]);
        //dup2(fd[0],STDOUT_FILENO);
        
        wait(NULL);

        ret = read(fd[0],fileid,FILE_ID_LEN);
        if (ret == -1)
        {
            perror("read error");
            exit(1);
        }
        int len = strlen(fileid);
        if(fileid[len-1] == '\n')
        {
            fileid[len-1] = '\0';
        }
        LOG(FDFS_TEST_MODULE,FDFS_TEST_PROC,"fileid = [%s]",fileid);
    }
    return 0;
}
