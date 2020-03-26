#include "iostream"
#include "boost/asio.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

void sync() {
	//ͬ��
	boost::asio::io_service io;
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
	t.wait();
	std::cout << "Hello, world!" << std::endl;

}
void callback(const boost::system::error_code&) {
	std::cout << "hello world async " << std::endl;
}

void async() {
	// �첽
	boost::asio::io_service io;
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(2));
	t.async_wait(callback);
	std::cout << "start run " << std::endl;
	io.run(); // �൱��forѭ����run�˳����� 1.io�е�stop����   2.����Ϣ���е�û���¼���ʱ��Ҳ���Ƴ�
	std::cout << "end run " << std::endl;
}

int main() {
	async();
	return 0;
}


