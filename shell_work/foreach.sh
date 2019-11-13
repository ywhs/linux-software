#!/bin/bash
# author: ywh
# time:2019-07-24


val=`expr 2 + 2`
echo "2 + 2 = " ${val}

a=10
b=20

if [ ${a} == ${b}  ]
then
  echo "a 等于 b"
else
  echo "a 不等于 b"
fi

for i in 1 2 3 4 5
do
  echo $i
done

int=1
while (( ${int}<=5 )) 
do
  echo "${int} <= 5"
  let int++
done

echo '按下 <CTRL-D> 退出'
echo -n '输入你最喜欢的网站名: '

while read FILM
do
	echo "是的！${FILM}是一个好网站"
done
