#!/bin/bash

#total_size=`xz --robot -l $1 | grep 'totals' | awk '{print $5}'`
total_size=`gzip -l $1 |awk 'END{print $2}'`

echo "total_size:$total_size"

block_size=`expr $total_size / 51200`
block_size=`expr $block_size + 1`
#let block_size=total_size/51200
#let block_size=block_size+1
echo "block_size:$block_size"
tar --blocking-factor=$block_size --checkpoint=1 --checkpoint-action='ttyout= %u    \r' -zxf $1 -C $2

#显示效果：
# root@lalala:~# ./unpackage.sh ./root.tar.gz temp/
# total_size:471726080
# block_size:921
# unpack 54%
