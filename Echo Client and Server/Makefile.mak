CXX := g++
CXXFLAGS := -Wall -std=c++11
LIBPATH := -L/usr/local/lib
LIBS  := -lboost_system

all: sync_client sync_server 

sync_server: sync_tcp_echo_server.cpp
	$(CXX) $(CXXFLAGS) sync_tcp_echo_server.cpp -o sync_server $(LIBPATH) $(LIBS)

sync_client: sync_tcp_echo_client.cpp
	$(CXX) $(CXXFLAGS) sync_tcp_echo_client.o -o sync_client $(LIBPATH) $(LIBS)

clean:
	rm -f sync_server sync_client