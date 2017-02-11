CC=gcc
CPPFLAGS= -I./include -I /usr/local/include/hiredis
CFLAGS=-Wall 
LIBS=-lhiredis -lfcgi

#找到当前目录下所有的.c文件
src = $(wildcard *.c)

#将当前目录下所有的.c  转换成.o给obj
obj = $(patsubst %.c, %.o, $(src))


#可执行程序的名字的路径
fdfs_upload_test=./test/fdfs_test
redis_api_test=./test/redis_api_test
redis_op_test=./test/redis_op_test
fastcgi_upload=./test/fastcgi_upload


target=$(fdfs_upload_test) $(redis_api_test) $(redis_op_test) $(fastcgi_upload)



ALL:$(target)


#生成所有的.o文件
$(obj):%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS) 


#fdfs_upload_test程序
$(fdfs_upload_test):./test/fdfs_test.o $(obj)
	$(CC) $^ -o $@ $(LIBS)

$(redis_api_test):./test/redis_api_test.o $(obj)
	$(CC) $^ -o $@ $(LIBS)

$(redis_op_test):./test/redis_op_test.o $(obj)
	$(CC) $^ -o $@ $(LIBS)

$(fastcgi_upload):./test/fastcgi_upload.o $(obj)
	$(CC) $^ -o $@ $(LIBS)



#clean指令
clean:
	-rm -rf $(obj) $(target) ./test/*.o
distclean:
	-rm -rf $(obj) $(target) ./test/*.o
#将clean目标 改成一个虚拟符号
.PHONY: clean ALL distclean
