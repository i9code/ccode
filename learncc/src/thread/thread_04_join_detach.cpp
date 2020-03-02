#include <iostream>
#include <thread>
#include <chrono>
// 线程创建好如果不用join加入 或者detach则会导致 程序崩溃，并且不会调用 析构函数
void joinWorker() {

}

class Obj {
public:
	Obj() { std::cout << "hello "; }
	~Obj() { std::cout << "world\n"; }
};

void detachWorker() {
	Obj obj;
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

class ThreadGuard {
public:
	ThreadGuard(std::thread& t) : m_thread(t) {}
	~ThreadGuard() { if (m_thread.joinable()) m_thread.join(); }
private:
	std::thread& m_thread;
};


int main() {
	std::thread j(joinWorker);
	ThreadGuard guard(j);
	std::thread w(detachWorker);
	w.detach();
	//abort(errorcode);
	return 0;
}