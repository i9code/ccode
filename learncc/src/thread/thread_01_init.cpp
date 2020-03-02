#include "iostream"
#include "thread"

void printAll(int a, int b, int c) {
	std::cout << a << " " << b << " " << c << std::endl;
}

void add(int a, int b, int& c) {
	c = a + b;
}

void printString(const std::string& info, const std::string& info2) {
	std::cout << "hello " << info << " " << info2 << "\n";
}

void testThreadInit() {
	int a = 3;
	int b = 4;
	int c = 5;
	std::thread t([=] {printAll(a, b, c); }); // 等号 按值拷贝
	t.join();
	std::thread t2(printAll, a, b, c);// 效率更高     参数按值拷贝
	t2.join();

	std::thread t3([=, &c] {add(a, b, c); });
	t3.join();
	std::cout << "after add " << c << std::endl;
	std::thread t4(add, a, b, std::ref(c));
	t4.join();

	std::string abc("abc");
	std::string def("def");

	std::thread t5([&] {printString(abc, def); });
	t5.join();
	std::thread t6(printString, abc, def); //按值传入，效率低与t7
	t6.join();
	std::thread t7(printString, std::cref(abc), std::cref(def));
	t7.join();

}

int main() {
	testThreadInit();
}