// from: https://ubuntuforums.org/showthread.php?t=2217101

// gcc -o https_get https_get.c -lssl -lcrypto
//http://talkera.org.cp-in-1.webhostbox.net/ascii/ascii.php
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

int main(int argc, char *argv[]){
  int sd;
  struct hostent *host;
  struct sockaddr_in addr;
  BIO *outbio = NULL;
  SSL_METHOD *method;
  SSL_CTX *ctx;
  SSL *ssl;
  char *req;
  int req_len;
  char hostname[] = "www.google.com";
  char certs[] = "/etc/ssl/certs/ca-certificates.crt";
  int port = 443;
  int bytes;
  char buf[128];

  // added this to test
  char           dest_url[] = "https://www.google.com";
  BIO              *certbio = NULL;
  X509                *cert = NULL;
  X509_NAME       *certname = NULL;
  BIO *outbio2 = NULL;



  OpenSSL_add_all_algorithms();
  ERR_load_BIO_strings();
  ERR_load_crypto_strings();
  SSL_load_error_strings();

  outbio    = BIO_new(BIO_s_file());
  outbio    = BIO_new_fp(stdout, BIO_NOCLOSE);
      
  if(SSL_library_init() < 0){
    BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
  }

  method = SSLv23_client_method();
  ctx = SSL_CTX_new(method);
  SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

  host = gethostbyname(hostname);
  sd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = *(long*)(host->h_addr);

  if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == -1 ) {
    BIO_printf(outbio, "%s: Cannot connect to host %s [%s] on port %d.\n", argv[0], hostname, inet_ntoa(addr.sin_addr), port);
  }

  ssl = SSL_new(ctx); 
  SSL_set_fd(ssl, sd);
    

  SSL_connect(ssl);

  // try something here
  /* Get the remote certificate into the X509 structure */
  printf("SSL_get_peer_certificate(ssl) \n");
  cert = SSL_get_peer_certificate(ssl);
  if (cert == NULL)
    printf("Error: Could not get a certificate from: %s.\n", dest_url);
  else
    printf("Retrieved the server's certificate from: %s.\n", dest_url);

  printf("\n");
    
  /* extract various certificate information */
  certname = X509_NAME_new();
  certname = X509_get_subject_name(cert);

  /*  display the cert subject here */
  BIO_printf(outbio, "Displaying the certificate subject data:\n");
  X509_NAME_print_ex(outbio, certname, 0, 0);
  BIO_printf(outbio, "\n\n");


  req = "GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n";    
  req_len = strlen(req);
  SSL_write(ssl, req, req_len);

  memset(buf, '\0', sizeof(buf));
  bytes = SSL_read(ssl, buf, sizeof(buf));
  while(bytes > 0){
    write(STDOUT_FILENO, buf, bytes);
    memset(buf, '\0', sizeof(buf));
    bytes = SSL_read(ssl, buf, sizeof(buf));
  }
  SSL_free(ssl);
  close(sd);
  SSL_CTX_free(ctx);
}