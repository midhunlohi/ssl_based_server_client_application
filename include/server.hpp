#ifndef SERVERHEADER_H

#define SERVERHEADER_H

#include "network.hpp"

class Server : public Network{
	private:
		SSL_CTX *ctx;
		int server;
		int port;
	public:
		int OpenListener(int port);
		void Init( int port, const char *certificate , const char *key, const char *caCert);
		void Task( int clientSocket, SSL* ssl);
		void Servlet(SSL* ssl);
		void Connect(void);
		void Close(void);
};

#endif
