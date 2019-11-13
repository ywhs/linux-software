#!/bin/bash

if test -e /ywh
then
  printf "ywh这个文件存在！\n"
else
  printf "ywh这个文件不存在！\n"
fi

num1=$[2*3]
num2=$[1+5]

if test ${num1} -eq ${num2}
then
  printf "num1 和 num2 相等 \n"
else
  printf "num1 和 num2 不相等 \n"
fi
