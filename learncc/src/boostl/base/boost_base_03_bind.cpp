#include "iostream"
#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
// boost bind ����һ�� boost function<>  function Ч��û��lambled��
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
	// �첽
	boost::asio::io_service io;
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(2));
	int count = 0;
	// bind��ʽ
	//t.async_wait(boost::bind(printBind,boost::asio::placeholders::error,&t,&count));
	count = 0;

	// lambda��ʽ
	t.async_wait([&t, &count](const auto& err) {printLambda(err, &t, &count); });


	std::cout << "start run " << std::endl;


	io.run(); // �൱��forѭ����run�˳����� 1.io�е�stop����   2.����Ϣ���е�û���¼���ʱ��Ҳ���Ƴ�
	std::cout << "end run " << std::endl;
	return 0;
}


