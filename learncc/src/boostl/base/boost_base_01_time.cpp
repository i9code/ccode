#include "iostream"
#include "boost/asio.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

void sync() {
	//同步
	boost::asio::io_service io;
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
	t.wait();
	std::cout << "Hello, world!" << std::endl;

}
void callback(const boost::system::error_code&) {
	std::cout << "hello world async " << std::endl;
}

void async() {
	// 异步
	boost::asio::io_service io;
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(2));
	t.async_wait(callback);
	std::cout << "start run " << std::endl;
	io.run(); // 相当于for循环，run退出条件 1.io中的stop函数   2.当消息池中的没有事件的时候也会推出
	std::cout << "end run " << std::endl;
}

int main() {
	async();
	return 0;
}


