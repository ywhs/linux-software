#include <stdio.h>
#include <string.h>


// 第一种方式定义结构体，标准格式
struct Books {

	char *title;
	char author[20];
	char subject[20];
	int book_id;

} b1, b2;

//第二种方式定义结构体
struct b3{
	char *title;
	char *author;
	char *subject;
	int book_id;
}; 

// 以后常用的方式吧，
typedef struct{
    char *title;
    char *author;
	char *subject;
	int book_id;
}Books1;

int main(){

	b1.title = "三体-黑暗森林";
	strcpy(b1.author, "刘慈欣");
	strcpy(b1.subject, "科幻小说,非常好看");
	b1.book_id = 1;

	b2.title = "剑来";
	strcpy(b2.author, "不知道");
	strcpy(b2.subject, "武侠小说");
	b2.book_id = 2;

	printf("author is : %s \n" , b1.author );

	printf("title is : %s \n" , b1.title );

	printf("subject is : %s \n" , b1.subject );

	printf("book_id is : %d \n" , b1.book_id );

	// 开始第二种方式定义结构体，常用
	structTest();
	// 开始定义第三种方式结构体
	structTest1();
	return 0;
}

structTest(){
	// 定义自定义类型变量
	Books1 b3;
	Books1 b4;
	printf("======================================= \n");

	b3.title = "雪中悍刀行";
	b3.author = "跟剑来一个作者";
	b3.subject = "带点武侠，带点玄幻";
	b3.book_id = 3;

	printf("b3.title is : %s \n", b3.title);
	printf("b3.author is : %s \n", b3.author);
	printf("b3.subject is : %s \n", b3.subject);
	printf("b3.book_id is : %d \n", b3.book_id);
}

structTest1(){
	struct b3 b5;

	printf("======================================= \n");
	b5.title = "东宫";

	printf("b5.title is : %s \n", b5.title);

}

