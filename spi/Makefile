# 本机编译器，如果需要可以改成交叉编译器 -Wall -werror -g 都是在编译的时候有打印调试信息的参数，最好在正式编译的时候都带上
GCC 	= gcc -Wall -Werror -g

# Makefile 内置规则，最前的为最高目标，后面的所有都是为其服务的
main: clean spi

# 相当于伪指令，但也有可能理解错了
.PHONY: clean spi

# $@ 符号是Makefile中内置变量，代表目标文件
# $^ 符号是Makefile中内置变量，代表依赖文件，即源文件
spi: myself-spi.o
	@echo "================ compile start ================"
	$(GCC) $^ -o $@
	@rm -rf myself-spi.o
	@echo "================ compile end  ================"


clean:
	@echo "=============== clean start ================="
	@rm -rf myself-spi.o spi
	@echo "=============== clean end ================="

