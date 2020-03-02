#include "iostream"
#include "thread"

// 为什么用多线程来做服务器？
// （1）当前的机器的单核红利已经结束了
// process(linux fork windows CreateProcess, signal) thread (POSIX threads)
// （2）多线程拥有自身的优势
// （3）API日益成熟，操作系统和标准库都已经支持了多线程
// 要在数据IO和计算找到平衡点
// 多线程的编程，mutilthread goto
// 可能的问题包括：
// （1）死锁；
// （2）乱序
// （3）并发访问数据造成的问题；
// （4）低效率

// c++11给我们带来的新概念
// （1）高阶接口；(async, future)

// （2）低阶接口；(thread, mutex)
//
//

void helloWorld() {
	std::cout << "hello world" << std::endl;
}
int main() {
	// 开启一个线程
	std::thread t(helloWorld);
	std::cout << "hello main thread" << std::endl;
	// 线程的终结
	t.join();
}