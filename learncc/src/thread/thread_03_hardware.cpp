#include "iostream"
#include "thread"
#include "mutex"
#include "vector"

class Counter {
public:
	void addCountAndResouce(int r) {
		std::lock_guard<std::mutex> lock(m_mutex);
		addCount();
		addResource(r);
	}
	int count() const {
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_count;
	}
	int aveResource() {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_count == 0) {
			return 1;
		}
		return m_totalResource / m_count;
	}
	Counter() : m_count{ 0 }, m_totalResource{ 0 } {}

private:
	int m_count;
	int m_totalResource;
	mutable std::mutex m_mutex;
	void addResource(int r) { m_totalResource++; }
	void addCount() { m_count++; } // Ð´Èë¼Ä´æÆ÷ ¼Ä´æÆ÷¼Ó1 Ð´ÈëÄÚ´æ
	  // ++ -- + - * /
	  // expert fetch_add fetch_sub
	  //int m_count;
};

int work(int a) {
	// do some thing
	return a + a;
}


template <class Iter>
void realWork(Counter& c, double& totalValue, Iter b, Iter e) {
	for (; b != e; ++b) {
		try {
			totalValue += work(*b);
			// print some vaule
		//	debugPrintInfo(c);
			c.addCountAndResouce(1);
		}
		catch (...) {
		}
	}
}

void debugPrintInfo(Counter& c) {
	std::cout << c.aveResource() << std::endl;
}

bool printStep(Counter& c, int maxCount) {
	auto count = c.count();
	if (count == maxCount) {
		std::cout << " ok finished \n";
		return true;
	}
	return false;
}


int main() {
	auto n = std::thread::hardware_concurrency();
	std::cout << n << " concurrent thread are supported\n";

	std::vector<int> vec;
	double totalValue = 0;
	for (int i = 0; i < 10000000; ++i) {
		vec.push_back(rand() % 100);
	}

	totalValue = 0;
	Counter counter;
	realWork(counter, totalValue, vec.begin(), vec.end());
	// do work
	std::cout << "total times: " << counter.count() << " " << totalValue << std::endl;

	totalValue = 0;
	Counter counter2;
	std::thread printCount([&counter2] {
		while (!printStep(counter2, 10000000))
			;
		});

	auto iter = vec.begin() + (vec.size() / 3);
	auto iter2 = vec.begin() + (vec.size() / 3 * 2);
	std::thread b([&counter2, &totalValue, iter, iter2] {
		realWork(counter2, totalValue, iter, iter2);
		});
	auto end = vec.end();
	double totalC = 0;
	std::thread c([&counter2, &totalC, iter2, end] {
		realWork(counter2, totalC, iter2, end);
		});

	double totalM = 0;
	realWork(counter2, totalM, vec.begin(), iter);
	b.join();
	c.join();
	auto realTotalCount = counter2.count();
	totalValue += totalC + totalM;
	std::cout << "total times use multithread: " << realTotalCount << " " << totalValue << std::endl;
	printCount.join();
}