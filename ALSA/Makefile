# 本机编译器，如果需要可以改成交叉编译器
GCC	= gcc
# $@ 符号是Makefile中内置变量，代表目标文件
# $^ 符号是Makefile中内置变量，代表依赖文件，即源文件
alsa: alsa-myself-rw-wav-file-paly-music.o
	@echo "=== compile start! ==="
	$(GCC) $^ -o $@ -lasound
	@echo "=== compile edn! ==="

.PHONY: clean
clean:
	@rm -rf *.o alsa
	@echo "clean success!"
