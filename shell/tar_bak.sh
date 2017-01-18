#!/bin/bash

#tar --exclude /nfsroot/dev_38/rootfs_2.7.0-rc8/root  --exclude  /nfsroot/dev_38/rootfs_2.7.0-rc8_slave/root  -zcvf /nfsroot/dev_38_2014xx.tgz /nfsroot/dev_38
# 该脚本用于压缩gzip格式，其中--exclude排除不压缩的内容
tar -zcvf /nfsroot/dev_26_20140324.tgz /nfsroot/dev_26
