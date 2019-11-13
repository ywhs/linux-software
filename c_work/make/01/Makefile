# Makefile的书写格式为
# targets(目标): prequisites(依赖)   也不知道单词写没写对
# 	command(命令)

# create first cmd
# main: main.c function1.c function2.c
	# cc main.c function1.c function2.c -o main
# 定义用户自定义变量

MOBJ = main.o function1.o function2.o
CC = cc

# 为了试验 := 和 = 的区别
ANIMAL = Dog
ANIMAL = Dogs
VAR := "$(ANIMAL) is dog"
ANIMAL = Cat
ANIMAL = Mok
BIANYI = ""

# 上面是直接把.c源文件编译链接成可执行文件，下面我们可以先把.c文件生成目标文件后再生成可执行文件，
# $^ 代表的是所有不重复的依赖文件
# $@ 代表的是目标的名字
# 使用判断语句进行编译
main: $(MOBJ)

# 命令,一般都是要缩进的，但是要是写ifeq的话缩进就不用写了。
ifeq ($(CC),$(BIANYI))
	$(CC) $^ -o $@
else
	gcc $^ -o $@
endif

# 创建第一条目标所需要的依赖,根据make的隐示规则，依赖不需要写目标，但必须需要有与依赖同名的文件,所以把下面的注释掉了

#function1.o: function1.c
#	$(CC) -c function1.c

#function2.o: function2.c
#	$(CC) -c function2.c

#main.o: main.c
#	$(CC) -c main.c


# 定义清除生成的文件 完整写法要写.PHONY
# @代表的是不输出这一行
.PHONY: clean
clean:
	@rm -rf main *.o
	@echo "clean success!"

# 定义安装伪目标
.PHONY: install
install:
	@cp main /usr/
	@echo "install success!"

# 取消安装
.PHONY: uninstall
uninstall:
	@rm -rf /usr/main
	@echo "uninstall success!"

# @代表不输出这一行
.PHONY: echo
echo:
	@echo ${VAR}

ANIMAL = shizi

# 测试判断语句
.PHONY: if
if:
	ifeq ("a","a")
		@echo "aaa"
	else
		@echo "bbb"
	endif

ifeq ("a","a")
	@echo "aaa"
else
	@echo "bbb"
endif


