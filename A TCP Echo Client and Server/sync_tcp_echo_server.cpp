#include "netcommon.h"
#include <iostream>

class sync_echo_server
{
public:
	explicit sync_echo_server(boost::asio::io_service& io_service, const TCP::endpoint& endpoint)
		:acceptor_(io_service, endpoint), socket_(io_service)  
	{
		
	}
	void start()
	{
		while (true) 
		{
			acceptor_.accept(socket_);
			char buffer[max_size];
			int bytes = boost::asio::read(socket_, boost::asio::buffer(buffer), 
			     [&](boost::system::error_code ec, std::size_t bytes) ->size_t
			     {
				    return on_read(buffer, bytes);
    			});
			socket_.write_some(boost::asio::buffer(buffer, bytes));
			socket_.close();
		}
	}
private:
	// terminated reading by EOL '\n' 
	size_t on_read(char *buffer, size_t bytes) 
	{
		bool found = std::find(buffer, buffer + bytes, '\n') < buffer + bytes;
		return found ? 0 : 1;
	}
private:
	TCP::acceptor acceptor_;
	TCP::socket socket_;
};
int main(int argc, char* argv[]) 
{
	try
	{
		boost::asio::ip::tcp::endpoint endpoint(
			boost::asio::ip::address::from_string("127.0.0.1"), 4403);
		boost::asio::io_service io_service;

		sync_echo_server server(io_service, endpoint);
		server.start();
		std::cout<<"server is running"<<std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}
