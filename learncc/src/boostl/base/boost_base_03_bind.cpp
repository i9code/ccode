#include "iostream"
#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
// boost bind 返回一个 boost function<>  function 效率没有lambled高
void printBind(const boost::system::error_code&, boost::asio::deadline_timer* t, int* count) {
	if (*count < 5) {
		std::cout << *count << std::endl;
		++(*count);
		t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
		t->async_wait(
			boost::bind(printBind, boost::asio::placeholders::error, t, count));
	}
}

void callback(const boost::system::error_code&) {
	std::cout << "hello world async " << std::endl;
}

void printLambda(const boost::system::error_code&, boost::asio::deadline_timer* t, int *count) {
	if (*count < 5) {
		std::cout << *count << std::endl;
		++(*count);
		t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
		t->async_wait([t, count](const auto& err) {printLambda(err, t, count); });
	}
}


int main() {
	// 异步
	boost::asio::io_service io;
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(2));
	int count = 0;
	// bind形式
	//t.async_wait(boost::bind(printBind,boost::asio::placeholders::error,&t,&count));
	count = 0;

	// lambda形式
	t.async_wait([&t, &count](const auto& err) {printLambda(err, &t, &count); });


	std::cout << "start run " << std::endl;


	io.run(); // 相当于for循环，run退出条件 1.io中的stop函数   2.当消息池中的没有事件的时候也会推出
	std::cout << "end run " << std::endl;
	return 0;
}


