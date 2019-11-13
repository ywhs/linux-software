#!/bin/bash

demoFun(){
	echo "这是我第一个shell脚本函数"
}

echo "------函数开始执行--------"
demoFun
echo "------函数执行结束--------"

funWithReturn(){
	echo "这个函数会对输入的两个数字进行相加运算"
	echo "第一个数字"
	read aNum
	echo "第二个数字"
	read bNum
	echo "两个数字分别是 ${aNum} 和 ${bNum}"
	return $((${aNum} + ${bNum}))
}

funWithReturn
echo "输入的两个数字之和为：$? !"
