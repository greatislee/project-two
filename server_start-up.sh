#!/bin/bash
#启动fastDFS服务

#tracker
sudo fdfs_trackerd ./conf/tracker.conf
echo fdfs_trackerd 启动成功！

#storage
sudo fdfs_storaged ./conf/storage.conf
echo fdfs_storaged 启动成功！

#nginx
sudo nginx -s stop
sudo nginx
echo nginx 启动成功！

#spawn-fcgi
spawn-fcgi -a 127.0.0.1 -p 8082 ./test/fastcgi_upload
echo spawn-fcgi 启动成功！
