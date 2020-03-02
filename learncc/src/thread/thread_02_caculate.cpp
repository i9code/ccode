#include "iostream"
#include "thread"
#include "cmath"
#include "vector"
#include "chrono"


double caculate(const double& v) {
	if (v <= 0) {
		return v;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	return std::sqrt((v * v + std::sqrt((v - 5) * (v + 2.5)) / 2.0) / v);
}

template<typename Iter,typename Fun>
double visitRange(std::thread::id id,Iter iterBegin, Iter iterEnd, Fun func) {
	auto curId = std::this_thread::get_id();
	if (id == curId) {
		std::cout << curId << "hello main thread" << std::endl;
	}
	else {
		std::cout << curId << "hello work thread" << std::endl;
	}
	double v = 0;
	for (auto iter = iterBegin; iter != iterEnd; ++iter) {
		v += func(*iter);
	}
	return v;
}

int main() {
	auto mainThreadId = std::this_thread::get_id();
	std::vector<double> v;
	for (int i = 0; i < 1000; ++i) {
		v.push_back(rand());
	}
	std::cout << v.size() << std::endl;

	double value = 0.0;
	for (auto& info : v) {
		value += caculate(info);
	}
	std::cout << value << std::endl;

	auto iter = v.begin() + (v.size() / 2);
	
	double anotherv = 0.0;
	auto iterEnd = v.end();

	std::thread s([&anotherv, mainThreadId, iter, iterEnd]() {
		anotherv = visitRange(mainThreadId,iter, iterEnd, caculate);
		});
	
	auto halfv = visitRange(mainThreadId,v.begin(), iter, caculate);

	s.join();
	std::cout << (halfv + anotherv) << std::endl;
}