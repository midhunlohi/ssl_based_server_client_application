all:
	g++ apps/demo_server.cpp core/server.cpp core/network.cpp core/logger.cpp -I./include -o demo_server -std=c++11 -lssl -lcrypto -pthread
	g++ apps/demo_client.cpp core/client.cpp core/network.cpp core/logger.cpp -I./include -o demo_client -std=c++11 -lssl -lcrypto -pthread
clean:
	rm demo_server demo_client
