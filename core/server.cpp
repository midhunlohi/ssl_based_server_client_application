#include "server.hpp"

using namespace std;

int Server::OpenListener(int port){   
	int sd;
	int option = 1;
	struct sockaddr_in addr;

	sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if ( bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
	{
		LOG(ERROR) << "can't bind port";
		abort();
	}
	LOG(INFO) << "Listening for new connection at port - " << ( "%d \n", port );
	if ( listen(sd, 5) != 0 )
	{
		LOG(ERROR) << "Can't configure listening port";
		abort();
	}
	return sd;
}
void Server::Init( int listenPort, const char *certificate, const char *key, const char *caCert ){
	port = listenPort;
	SSL_library_init();	
	SSL_load_error_strings();
	ctx = InitServerCTX(); // Initialize SSL
	LoadCertificates(ctx, certificate, key, caCert);/* load certs */
	// TODO : Validate return values.
}

/* Serve the connection -- threadable */
void Server::Servlet(SSL* ssl){   
	char buf[1024];
	char reply[1024];
	int sd, bytes;
	const char* HTMLecho="Hello Friend !!";
	if ( SSL_accept(ssl) == FAIL ){     /* do SSL-protocol accept */
		ERR_print_errors_fp(stderr);
	}
	else{	
		ShowCerts(ssl);        /* get any certificates */
		while(1){
			bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */
			if ( bytes > 0 ){
				buf[bytes] = 0;
				LOG(INFO) << "@server#" << ("%s", buf);
				sprintf(reply, HTMLecho, buf);   /* construct reply */
				SSL_write(ssl, reply, strlen(reply)); /* send reply */
			}
			else{
				ERR_print_errors_fp(stderr);
			}
		}
	}
	sd = SSL_get_fd(ssl);       /* get socket connection */
	SSL_free(ssl);         /* release SSL state */
	close(sd);          /* close connection */
}		
void Server::Task(int clientSocket, SSL* ssl){
	SSL_set_fd(ssl, clientSocket);      /* set connection socket to SSL state */
	Servlet(ssl);         /* service connection */
}

void Server::Connect(void){
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);						
	SSL *ssl;
	server =  OpenListener(port);/* create server socket */
	while(1){
		int client = accept(server, (struct sockaddr*)&addr, &len);  /* accept connection as usual */
		LOG(INFO) << "New Connection: " << ("%s:%d:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), client);
		ssl = SSL_new(ctx);              /* get new SSL state with context */
		thread t1(&Server::Task, this, client, ssl);
		t1.detach();
	}
}
void Server::Close(void){
	close(server); /* close server socket */
	SSL_CTX_free(ctx); /* release context */
}
