#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "redis_op.h"

int main(int argc, char *argv[])
{

    //建立连接
    redisContext* conn = NULL;
    redisReply* reply = NULL;

    conn = rop_connectdb_nopwd("127.0.0.1", "6379");
    if (conn == NULL) 
    {
        //连接失败
        printf("connect error\n");
        exit(0);
    }
    //set key
    char* key = "likang";
    char* value = "is great";
    int ret = rop_string_set(conn,key,value);
    if(ret != 0) 
    {
        printf("出错啦\n");
    }
    //get key
    key = "likang";
    char values[128] = {0};
    ret  = rop_string_get(conn,key,values);
    if(ret == 0)
    {
        printf("get %s =  %s\n",key,values);
    }
    //断开连接
    rop_disconnect(conn);

	return 0;
}
