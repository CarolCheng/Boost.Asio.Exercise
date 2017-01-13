#include "netcommon.h"
#include <iostream>


class async_tcp_connection 
	: public std::enable_shared_from_this<async_tcp_connection>
{
public:
	async_tcp_connection(TCP::socket socket)
		:socket_(std::move(socket)), started_(false) 
	{
	}
	// start reading buffer message
	void start()
	{
	    started_ = true;
		do_read();
	}
	void stop()
	{
		if(!started_)
			return;
		socket_.close();
		started_ = false;
	}
private:
	void do_read()
	{
	    auto self(shared_from_this());
		boost::asio::async_read(socket_, boost::asio::buffer(data_), 
			[this, self](const boost::system::error_code &ec, std::size_t bytes) ->size_t
			{
			    if (ec) return 0;
				return on_read(bytes);
			},
			[this, self](const boost::system::error_code &ec, std::size_t bytes) 
			{
			    if (!ec)
			    do_write(data_,bytes);
				stop();   
			});
	}
	void do_write(const char *buffer, size_t bytes)
	{
	    auto self(shared_from_this());
		socket_.async_write_some(boost::asio::buffer(buffer, bytes),
			[this, self](boost::system::error_code ec, std::size_t bytes)
			{
				if(ec) return; 
				do_read();
			});
	}
	// terminated reading by EOL '\n' 
	size_t on_read(size_t bytes) 
	{
		bool found = std::find(data_, data_ + bytes, '\n') < data_ + bytes;
		return found ? 0 : 1;
	}
private:
	TCP::socket socket_;
    char data_[max_size_];
	bool started_;
};
class async_tcp_server 
{
public:
	async_tcp_server(boost::asio::io_service& io_service, const TCP::endpoint& endpoint)
		:acceptor_(io_service, endpoint), socket_(io_service) 
	{
		on_accept();
	}
private:
	// accept the new connection
	void on_accept() 
	{
		acceptor_.async_accept(socket_, 
			[this] (boost::system::error_code ec) 
			{
				if (!ec)
				{
					std::make_shared<async_tcp_connection>(std::move(socket_))->start();
				}
				on_accept();
			});
	}
private:
	TCP::acceptor acceptor_;
	TCP::socket  socket_;
};
int main(int argc, char* argv[]) 
{
	try
	{
		boost::asio::ip::tcp::endpoint endpoint(
			boost::asio::ip::address::from_string("127.0.0.1"), 4403);

		boost::asio::io_service io_service;

		async_tcp_server server(io_service, endpoint);
		io_service.run();
		std::cout<<"server is running"<<std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}