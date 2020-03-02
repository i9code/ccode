#include <stdlib.h>
#include <iostream>
#include <list>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstring> // strlen size


class Message {
public:
	const std::string& data() const { return m_data; }
	Message(std::string d) : m_data(std::move(d)) {}
private:
	std::string m_data;
};

int totalSize = 0;
std::list<Message> globalList;

void worker() {
	while (!globalList.empty()) {
		auto iter = globalList.begin();
		// do real work and erase it
		totalSize += strlen((*iter).data().c_str());
		globalList.erase(iter);
	}
}

int main() {
	for (int i = 0; i < 500000; ++i)
		globalList.push_back("this is a test" + std::to_string(i));

	worker();

	for (int i = 0; i < 300000; ++i) {
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		globalList.push_back(std::string("second"));
	}
	worker();
	std::cout << "total size is " << totalSize << std::endl;
}




