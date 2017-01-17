#include "netcommon.h"
#include <iostream>
#include <string.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <deque>

class async_tcp_client
{
public:
	explicit async_tcp_client(boost::asio::io_service& io_service)
	:io_service_(io_service), socket_(io_service) 
	{
	}
public:
	void start(const std::string &ipaddr, const int port)
	{
		TCP::endpoint hostname(boost::asio::ip::address::from_string(ipaddr), port);
		do_connect(hostname);
	}
	void write(const std::string &msg)
	{
		io_service_.post( [this, msg]() {
			bool write_in_progress = !write_msgs_.empty();
			write_msgs_.push_back(msg);
			if (!write_in_progress) {
				do_write(msg + "\n");
			}
		});
	}
	void stop()
	{
		io_service_.post([this]() { socket_.close(); });
	}
private:
	// connect to server
	void do_connect(const TCP::endpoint& address) 
	{
		socket_.async_connect(address,
			[this](boost::system::error_code ec) {
				if (!ec) {
					std::cout << "connected" << std::endl;
				}
			});
	}
	void do_write(const std::string &msg)
	{
		socket_.async_write_some(boost::asio::buffer(msg, msg.length()),
		[this](boost::system::error_code ec, std::size_t bytes)
		{
			if (ec) socket_.close(); 
			else do_read();
		});
	}
	void do_read()
	{
		boost::asio::async_read(socket_, boost::asio::buffer(read_buffer_), 
			[this](const boost::system::error_code &ec, std::size_t bytes) -> size_t {
				if (ec) socket_.close();
				return on_read(bytes);
			},
			[this](const boost::system::error_code &ec, std::size_t bytes) {
				// print : whether or not the sent and received message is same 
				std::cout << "server echoed our " << write_msgs_.front().data() << ": "  
					<< (strncmp(read_buffer_, write_msgs_.front().data(), bytes - 1) == 0 ? "OK" : "FAIL") 
					<< std::endl;
				write_msgs_.pop_front();
				if (!write_msgs_.empty()) {
					std::string msg = write_msgs_.front().data();
					do_write(msg + "\n");
				}
			});
	}
	 /**
       * CompletionCondition : read until the character '\n' 
       * @param bytes the number of bytes read
       * @return if the message are read completely
       */
	size_t on_read(size_t bytes) 
	{
		bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
		return found ? 0 : 1;
	}
private:    
	boost::asio::io_service& io_service_;
	TCP::socket socket_;
	char read_buffer_[max_size_];
	std::deque<std::string> write_msgs_;
};
int main(int argc, char* argv[]) 
{
	boost::asio::io_service service;
	
	char const * messages[] = {
		"John says hi",
		"so does James",
		"Lucy just got home",
		"Boost.Asio is Fun!"
	};
	
	async_tcp_client client(service);
	client.start("127.0.0.1", 4003);
	std::thread thrd_io ( [&service](){ service.run(); });
	
	for(auto p : messages) {
		client.write(p);
	}

	thrd_io.join();
	client.stop();
}