#include "iostream"
#include "boost/asio.hpp"
#include "boost/asio/steady_timer.hpp"



void callback(const boost::system::error_code&) {
	std::cout << "hello world async " << std::endl;
}

int main() {
	boost::asio::io_context io;
	boost::asio::steady_timer st(io);
	st.expires_from_now(std::chrono::seconds(5));
	st.wait();
	std::cout << "end main";
	return 0;
}