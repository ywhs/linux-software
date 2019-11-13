#include <iostream>

using namespace std;

class Box
{
	
	public:
		double length;	// 长度
		double breadth; // 宽度
		double height;	// 高度
	
};

int main()
{

	Box box1; // 声明变量1
	Box box2; // 声明变量2

	double volume = 0.0; // 用于存储体积

	// box1 详述
	box1.height = 5.0;
	box1.length = 6.0;
	box1.breadth = 7.0;

	// box2 详述
	box2.height = 10.0;
	box2.length = 12.0;
	box2.breadth = 13.0;

	// box1的体积
	volume = box1.height * box1.length * box1.breadth;
	cout << "Box1 的体积：" << volume <<endl;

	// box2的体积
	volume = box2.height * box2.length * box2.breadth;
	cout << "box2的体积：" << volume <<endl;

	return 0;

}
