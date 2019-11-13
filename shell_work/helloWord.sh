#!/bin/bash

yangwh="ywh"

str="I know you \"$yangwh\" ! \n"
echo -e "输出组装语句：" ${str}
echo "输出长度：" ${#yangwh}
echo "输出截取后的字符：" ${str:2:5}
echo "输出查找特定的字符最后一个位置：" `expr index "${str}" o`
echo "执行的文件名: ${0}"
if [ -n "${1}" ]
then
    echo "包含第一个参数：${1}"
else
    echo "没有包含第一参数"
fi
array_name=(1 2 3 4)
echo "输出数组元素[1]：${array_name[0]}"
echo "输出数组元素[2]：${array_name[1]}"

echo '$yangwh\"'
