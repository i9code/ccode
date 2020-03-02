#include <iostream>
#include <thread>
#include <future>
#include <cmath>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <atomic>
#include <mutex>
 //enum memory_order {
 //
 //  memory_order_relaxed,
 //  memory_order_consume,
 //  memory_order_acquire,
 //  memory_order_release,
 //  memory_order_acq_rel,
 //  memory_order_seq_cst
 //};


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
	std::thread t([=] {printAll(a, b, c); });
	t.join();
	std::thread t2(printAll, a, b, c);
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
	std::thread t6(printString, abc, def);
	t6.join();
	std::thread t7(printString, std::cref(abc), std::cref(def));
	t7.join();


}



struct BankAccount {
	BankAccount(int b) : Balance(b) {}
	int Balance;
	std::mutex Mutex;
};

template<typename T>
class Lock {
public:
	Lock(T& m) : m_mutex(m) { m.lock(); }
	~Lock() { m_mutex.unlock(); }
private:
	T& m_mutex;
};

void transferMoney(BankAccount& a, BankAccount& b, int money) {
	std::lock(a.Mutex, b.Mutex /* ..... */);
	std::lock_guard<std::mutex> lockA(a.Mutex, std::adopt_lock);
	std::lock_guard<std::mutex> lockB(b.Mutex, std::adopt_lock);

	if (a.Balance <= money)
		return;
	a.Balance -= money;
	b.Balance += money;
	//  if (&a == &b)
	//    return;
	//  if (&(a.Mutex) < &(b.Mutex)) {
	//    std::lock_guard<std::mutex> lockA(a.Mutex);
	//    std::lock_guard<std::mutex> lockB(b.Mutex);
	//
	//    if (a.Balance <= money)
	//      return;
	//    a.Balance -= money;
	//    b.Balance += money;
	//  } else {
	//    std::lock_guard<std::mutex> lockA(b.Mutex);
	//    std::lock_guard<std::mutex> lockB(a.Mutex);
	//    if (a.Balance <= money)
	//      return;
	//    a.Balance -= money;
	//    b.Balance += money;
	//  }
}

// BankAccount alice(100);
// BankAccount bob(100);
// threa1
// transferMoney(alice, bob, 10); // when call lockB
// thread2
// transferMoney(bob, alice, 10); // when call lockB



int main() {
	//testThreadInit();
	unsigned int n = std::thread::hardware_concurrency();
	std::cout << n << " concurrent threads are supported.\n";
	
	// (1) 如果没有必要的话，线程间不要共享资源
}

