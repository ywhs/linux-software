CC		= g++
TARGET		= 

# main为此Makefile的主要目标
main: libclm.so
	$(CC) test.c -lclm -o a

.PHONY: libclm.so

libclm.so:
	@echo "======= compile libclm.so start ========"
	# -fPIC 和 -shared 两个参数是编译动态库所必需的的两个参数
	$(CC) -fPIC -shared clm-copy.cpp -o libclm.so
	@cp ./libclm.so /usr/lib/
	@echo "====== compile libclm.so end ========="

clean:
	@echo "=========== clean start ==============="
	rm -rf *.so *.o a
	@echo "=========== clean end ================="

