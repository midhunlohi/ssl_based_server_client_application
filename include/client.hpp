#ifndef CLIENTHEADER_H

#define CLIENTHEADER_H

#include "network.hpp"

class Client: public Network{
    private:
		SSL_CTX *ctx;
		int server;
		SSL *ssl;
    public:
   		int OpenConnection(const char *hostname, int port);
		void Init(const char *hostName, int port, const char *certificate, const char *key, const char *caCert);
		void Connect(void);
		void Send(const char *message);
		void Close(void);
};

#endif
