#include "iostream"
#include "array"

template<typename C>
void checkSize(C& c) {
	if (c.size() > 3) {
		c[3] = 10;
	}
	//c.at(3) = 10;
}



void arrayPart() {
	// array 实际上是对c/c++语言中原生数组进行的封装
	// namespace std {
	// template<typename T, size_t N>
	// class array;
	// }
	// 特点：内存分配在栈（stack），绝对不会重新分配，随机访问元素
	int abc[100];
	std::array<int, 100> a;
	std::array<int, 100> b = {};
	// 以上两者的区别

	std::array<int, 5> obj = { 1,2,3,4,5 };
	std::array<int, 5> obj2 = { 1,0 };
	// 接口
	a.empty(); // nerver be true if size > 0
	a.size();
	a.max_size();
	// operator == < != > <= >=
	auto aa = a;

	aa.swap(a);
	swap(aa, a);
	// 访问元素
	a[1];
	a.at(1);
	a.front();
	a.back();
	checkSize(a);

	// 迭代器相关
	a.begin();
	a.end();
	a.cbegin();
	a.cend();
	a.rbegin();
	a.rend();
	a.crbegin();
	a.crend();

	// 和C的接口互用
	std::array<char, 100> carr;
	strcpy_s(&carr[0],strlen("hello world\n")+1, "hello world\n"); // more better use carr.data();
	printf("%s", &carr[0]); // more better use carr.data()
	// 错误的用法
	printf("%s", carr.begin());

	// 特殊的地方
	auto info = std::get<1>(carr);
	carr.fill(0);

	// 异常exception
	// c.at(pos);
	// copy move swap
}

void main() {
	arrayPart();
}