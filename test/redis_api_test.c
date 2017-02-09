#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<dirent.h>
#include<unistd.h>

#include<hiredis.h>



int main(int argc,char* argv[])
{
    redisContext *c;
    redisReply *reply;

    //创建一个服务器句柄
    c = redisConnect("127.0.0.1",6379);
    if (c == NULL || c->err)
    {
        if(c)
        {
            printf("Connect error:%s\n",c->errstr);
            redisFree(c);
        }
        else
        {
            printf("connection error:can't allocate redis context\n");
        }
        exit(1);
    }

    //执行一个命令
    //SET
    reply = redisCommand(c,"set %s %s","iceforg","is great");
    if (reply == NULL) 
    {
        printf("redisCommand set error\n");
        exit(1);
    }
    if(strcmp(reply->str,"OK")==0)
    {
        printf("SET:%s\n",reply->str);  
    }
    freeReplyObject(reply);

    //GET
    reply = redisCommand(c,"GET %s","itcast");
    if (reply == NULL) 
    {
        printf("redisCommand get error\n");
        exit(1);
    }
    printf("get itcast = %s\n",reply->str);
    freeReplyObject(reply);

    


    return 0;
}
