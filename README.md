# Boost.Asio.Exercise
##Environment
1. Ubuntu 14.04.5 LTS
  * Command to check version : *lsb_release -a*
2. C++11
3. Boost.Asio 1.59.0 
  * Dependencies: 
    * Boost.System
    * Boost.Regex
    * OpenSSL

##Content
1. A TCP Echo Client and Server

|   Server      	      	      |       synchronous      	|         asynchronous         	|
|:--------------------------:	|:----------------------:	|:----------------------------:	|
| How may clients   <br /> at a time 	| one client connection  	| multiple client connections  	|
| Accept Function<br /> (basic_socket_acceptor) 	| [accept](https://goo.gl/06oFIU) |  [async_accept](https://goo.gl/slWbX0)|
|      UML Class Diagram     	| ![sync server class diagram](https://github.com/CarolCheng/Boost.Asio.Exercise/blob/master/UML%20Diagram/sync_tcp_echo_server.png?raw=true) |![async server class diagram](https://github.com/CarolCheng/Boost.Asio.Exercise/blob/master/UML%20Diagram/async_tcp_echo_server.png?raw=true)|

2. A synchronous TCP Client and Server
3. A asynchronous TCP Client and Server

##Reference
1. [Boost.Asio C++ Network Programming (Chinese version)] (https://www.gitbook.com/book/mmoaay/boost-asio-cpp-network-programming-chinese/details)
2. [Boost.Asio C++ Network Programming] (http://developers-club.com/posts/192284/)
3. [Boost.Asio - 1.59.0 - C++11 Example] (http://www.boost.org/doc/libs/1_59_0/doc/html/boost_asio/examples/cpp11_examples.html)
