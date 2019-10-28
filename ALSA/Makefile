# 本机编译器，如果需要可以改成交叉编译器 -Wall -werror -g 都是在编译的时候有打印调试信息的参数，最好在正式编译的时候都带上
GCC	= gcc -Wall -Werror -g

# Makefile的内置规则，文件最前的为最高目标，执行make命令会自动编译此目标
main: clean alsa

.PHONY: clean alsa
# $@ 符号是Makefile中内置变量，代表目标文件
# $^ 符号是Makefile中内置变量，代表依赖文件，即源文件
alsa: alsa-myself-rw-wav-file-paly-music.o
	@echo "=== compile start! ==="
	$(GCC) $^ -o $@ -lasound
	@echo "=== compile edn! ==="


clean:
	@rm -rf *.o alsa
	@echo "clean success!"
