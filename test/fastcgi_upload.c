/*
 * echo.c --
 *
 *	Produce a page containing all FastCGI inputs
 *
 *
 * Copyright (c) 1996 Open Market, Inc.
 *
 * See the file "LICENSE.TERMS" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */
#ifndef lint
static const char rcsid[] = "$Id: echo.c,v 1.5 1999/07/28 00:29:37 roberts Exp $";
#endif /* not lint */

#include "fcgi_config.h"

#include <stdlib.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#include <sys/wait.h>
#endif

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

#include "fcgi_stdio.h"
#include "make_log.h"
#include"fdfs_client.h"

#define FASTCGI_UPLOAD "file_data and name"
#define FASTCGI "fileid"

#include "../include/util_cgi.h"




/*
 * static void PrintEnv(char *label, char **envp)
 * {
 *     printf("%s:<br>\n<pre>\n", label);
 *     for ( ; *envp != NULL; envp++) {
 *         printf("%s\n", *envp);
 *     }
 *     printf("</pre><p>\n");
 * }
 * 
 */
int main ()
{
    /*
     * char **initialEnv = environ;
     * int count = 0;
     */

    while (FCGI_Accept() >= 0) 
    {
        char *contentLength = getenv("CONTENT_LENGTH");
        int len;

        printf("Content-type: text/html\r\n"
               "\r\n");

        if (contentLength != NULL) 
        {
            len = strtol(contentLength, NULL, 10);
        }
        else 
        {
            len = 0;
        }

        if (len <= 0) 
        {
            printf("No data from standard input.<p>\n");
        }
        else 
        {
            int i, ch;

            char*buf = (char *)malloc(len);
            char *temp_p = buf;

            /* printf("Standard input:<br>\n<pre>\n"); */
            for (i = 0; i < len; i++) 
            {
                if ((ch = getchar()) < 0) 
                {
                    printf("Error: Not enough bytes received on standard input<p>\n");
                    break;
                }
                /* putchar(ch); */
                *temp_p = ch;
                temp_p++;
            }
            /* printf("\n</pre><p>\n"); */
            char *left = memstr(buf,len,"filename");
            char *temp_name = strtok(left,"\r\n");
            char *name = strtok(temp_name+9,"\"");

            char *type = memstr(buf,len,"Content-Type");
            char *con = strtok(type,"\r");
            type+=strlen(con)+4;

            int temp_len = len - (type - buf);
            char *end = memstr(type,temp_len,"-------------");

            int ret;
            char* file_id = (char *)malloc(1024);
            char* torage_ip = (char *)malloc(1024);


            ret = dfs_init(getpid(),"/etc/fdfs/client.conf");
            if(ret != 0)
            {
                LOG(FASTCGI,FASTCGI_UPLOAD,"dfs_init error");
            }
            ret = upload_file(type,end-type-2,file_id,torage_ip);
            if (ret != 0) 
            {
                LOG(FASTCGI,FASTCGI_UPLOAD,"upload_file error");
                exit(1);
            }

            FILE* fp = fopen(name,"w");
            if (fp == NULL)
            {
                perror("fopen error");
                exit(1);
            }
            fwrite(type,1,end-type-2,fp);
            fclose(fp);
            free(buf);

            printf("%s:%s\n",torage_ip,file_id); 
            
            free(file_id);
            free(torage_ip);
            /* PrintEnv("Request environment", environ); */
            /* PrintEnv("Initial environment", initialEnv); */
        } /* while */
    }
    return 0;
    
}
