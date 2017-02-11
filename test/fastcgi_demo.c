#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "fcgi_stdio.h"

int main(int argc, char *argv[])
{
    int count = 0;

    //阻塞
    while (FCGI_Accept() >= 0) 
    {

        //将标准的输出 重定向 ngxin已经accept成功的cfd
        printf("Content-type: text/html\r\n");
        printf("\r\n");

        printf("<title>Fast CGI Hello!</title>");
        printf("<h1>Fast CGI Hello!</h1>h1>");                                                     
        printf("Request number %d running on host <i>%s</i>i>, remote_addr=%s, remote_port = %s\n",++count, getenv("SERVER_NAME"), getenv("REMOTE_ADDR"), getenv("REMOTE_PORT"));


    }

    return 0;
}



