#include "netcommon.h"
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <vector>

class sync_echo_client 
{
public:
	explicit sync_echo_client(boost::asio::io_service& io_service)
		:socket_(io_service) 
	{
		
	}
	void connect(const std::string &ipaddr, const int port)
	{
		TCP::endpoint hostname(boost::asio::ip::address::from_string(ipaddr), port);
		socket_.connect(hostname);
	}
	void start(const std::string &msg)
	{
		char buffer[max_size_];
		socket_.write_some(boost::asio::buffer(msg + "\n"));
		int bytes = boost::asio::read(socket_, boost::asio::buffer(buffer), 
			[&](boost::system::error_code /*ec*/, std::size_t bytes) ->size_t
			{
				return on_read(buffer, bytes);
			});
		// print : whether or not the sent and received message is same 
		std::string copy(buffer, bytes - 1);
		std::cout << "server echoed our " << msg<< ": "
		          << (copy == msg ? "OK" : "FAIL") << std::endl;
		socket_.close();
	}
private:
	size_t on_read(char *buffer, size_t bytes) 
	{
		bool found = std::find(buffer, buffer + bytes, '\n') < buffer + bytes;
		return found ? 0 : 1;
	}
private:
	TCP::socket socket_;
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
	// Launch a group of client threads
	std::vector<std::thread> workers;
	for(auto p : messages) {
		workers.push_back(std::thread( [&] {
			sync_echo_client client(service);
			client.connect("127.0.0.1", 4403);
			client.start(p);
		}));
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	// Join the threads with the main thread
	std::for_each(workers.begin(), workers.end(), [](std::thread &t) 
	{
		t.join();
	});
}
