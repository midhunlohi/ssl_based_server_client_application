#include "network.hpp"

SSL_CTX* Network::InitServerCTX(void){ 
	const SSL_METHOD *method;
	SSL_CTX *ctx;

	OpenSSL_add_all_algorithms();  /* load & register all cryptos, etc. */
	SSL_load_error_strings();   /* load all error messages */
//	method = TLSv1_2_server_method();  /* create new server-method instance */
	method = SSLv23_method(); 
	ctx = SSL_CTX_new(method);   /* create new context from method */
	if ( ctx == NULL )
	{
	ERR_print_errors_fp(stderr);
	abort();
	}
	return ctx;
}

SSL_CTX* Network::InitClientCTX(void){   
	const SSL_METHOD *method;
	SSL_CTX *ctx;

	OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
	SSL_load_error_strings();   /* Bring in and register error messages */
	//method = TLSv1_2_client_method();  /* Create new client-method instance */
	method = SSLv23_method();
	ctx = SSL_CTX_new(method);   /* Create new context */
	if ( ctx == NULL )
	{
	ERR_print_errors_fp(stderr);
	abort();
	}
	return ctx;
}

void Network::ShowCerts(SSL* ssl){   
	X509 *cert;
	char *line;

	cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
	if ( cert != NULL ){
		LOG(INFO) << "other party certificates:";
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		LOG(INFO) << "Subject: "<< ("%s\n", line);
		free(line);
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		LOG(INFO) << "Issuer: " << ("%s\n", line);
		free(line);
		X509_free(cert);
	}
	else{
		LOG(ERROR) << ("other party certificates not available\n");
	}
}	

void Network::LoadCertificates(SSL_CTX* ctx, const char* CertFile, const char* KeyFile, const char *CaCertificate){
	/* set the local certificate from CertFile */
	if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
	{
		ERR_print_errors_fp(stderr);
		abort();
	}
	/* set the private key from KeyFile (may be the same as CertFile) */
	if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
	{
		ERR_print_errors_fp(stderr);
		abort();
	}
	/* verify private key */
	if ( !SSL_CTX_check_private_key(ctx) )
	{
		LOG(ERROR) << "Private key does not match the public certificate\n";
		abort();
	}
	/* Load the RSA CA certificate into the SSL_CTX structure */
	if (!SSL_CTX_load_verify_locations(ctx, CaCertificate, NULL)) {
        	ERR_print_errors_fp(stderr);
        	exit(1);
	}

	/* Set to require peer (client) certificate verification */
	SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER,NULL);

	/* Set the verification depth to 1 */
	SSL_CTX_set_verify_depth(ctx,1);
}