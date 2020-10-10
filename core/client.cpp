#include "client.hpp"

int Client::OpenConnection(const char *hostname, int port){   
	int sd;
	struct hostent *host;
	struct sockaddr_in addr;

	if ( (host = gethostbyname(hostname)) == NULL ){
		perror(hostname);
		abort();
	}

	sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = *(long*)(host->h_addr);

	if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 ){
		close(sd);
		perror(hostname);
		abort();
	}
	return sd;
}

void Client::Init(const char *hostName, int port, const char *certificate, const char *key, const char *caCertificate){
	SSL_library_init();
	ctx = InitClientCTX();				
	LoadCertificates(ctx, certificate, key, caCertificate);/* load certs */
	server = OpenConnection(hostName, port);
}	
void Client::Connect(void){
	ssl = SSL_new(ctx);      /* create new SSL connection state */
	SSL_set_fd(ssl, server);    /* attach the socket descriptor */		
	if(SSL_connect(ssl) == FAIL){
			ERR_print_errors_fp(stderr);
	}
	LOG(INFO) << "Connected with "<< ("%s encryption\n", SSL_get_cipher(ssl));
	ShowCerts(ssl);        /* get any certs */
}
void Client::Send(const char *message){
	char buf[1024];
	int bytes;			
	SSL_write(ssl, message, strlen(message));   /* encrypt & send message */
	bytes = SSL_read(ssl, buf, sizeof(buf)); /* get reply & decrypt */
	buf[bytes] = 0;
	LOG(INFO) << "Received: " <<  ("%s\n", buf);
}
void Client::Close(void){
	SSL_free(ssl);        /* release connection state */
	close(server);         /* close socket */
	SSL_CTX_free(ctx);        /* release context */			
}		

