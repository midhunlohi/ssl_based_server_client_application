#ifndef NETWORKHEADER_H
#define NETWORKHEADER_H

#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <logger.hpp>
#include <thread>

#define FAIL    -1

class Network{
public:
  SSL_CTX* InitServerCTX(void); 
  SSL_CTX* InitClientCTX(void);   
  void ShowCerts(SSL* ssl);
  void LoadCertificates(SSL_CTX* ctx, const char* CertFile, const char* KeyFile, const char *CaCertificate);
};

#endif
