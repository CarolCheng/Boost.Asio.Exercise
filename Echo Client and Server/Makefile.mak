CXX := g++
CXXFLAGS := -Wall -std=c++11
LIBPATH := -L/usr/local/lib
LIBS  := -lboost_system

all: sync_client sync_server async_server async_client

sync_server: sync_tcp_echo_server.cpp
	$(CXX) $(CXXFLAGS) sync_tcp_echo_server.cpp -o sync_server $(LIBPATH) $(LIBS)

sync_client: sync_tcp_echo_client.cpp
	$(CXX) $(CXXFLAGS) sync_tcp_echo_client.cpp -o sync_client $(LIBPATH) $(LIBS)
	
async_server: async_tcp_echo_server.cpp
	$(CXX) $(CXXFLAGS) async_tcp_echo_server.cpp -o async_server $(LIBPATH) $(LIBS)
	
async_client: async_tcp_echo_client.cpp
	$(CXX) $(CXXFLAGS) async_tcp_echo_client.cpp -o async_client $(LIBPATH) $(LIBS)
	
clean:
	rm -f sync_server sync_client async_server async_client