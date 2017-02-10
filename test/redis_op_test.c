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
    /* redisReply* reply = NULL; */

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
    int arr_num = 0;
    int count = 3;
    int i = 0;
    RVALUES array = NULL;


    ret  = rop_string_get(conn,key,values);
    if(ret == 0)
    {
        printf("get %s =  %s\n",key,values);
    }


    //lpush
    ret = rop_list_push(conn,"MY_LIST","dota");
    if (ret < 0) 
    {
       printf("lpush error\n");
    }
    ret = rop_list_push(conn,"MY_LIST","SF");
    ret = rop_list_push(conn,"MY_LIST","LINA");

    //lrange
    array = malloc(count* VALUES_ID_SIZE);
    ret = rop_range_list(conn,"MY_LIST",0,2,array,&arr_num);
    if(ret != -1)
    {
        for (i = 0; i < arr_num; ++i) 
        {
           printf("%d:%s\n",i,array[i]);
        }
    }
    if(array != NULL)
    {
        free(array);
        printf("free array succ\n");
    }
    //set
    ret = rop_hash_set(conn,"sf","Hight","180");





    //断开连接
    rop_disconnect(conn);

	return 0;
}
