#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<time.h>

#define uchar unsigned char
#define uint unsigned int

char** argv2;

void printErr(){
  printf("Wrong command. Usage:\n");
  printf("%s <mode> [<filename>]\n", argv2[0]);
  printf("mode:\n");
  printf("\t0\tGenerate public and private key\n");
  printf("\t1\tEncrypt data, filename option is required. \n");
  printf("\t2\tDecrypt data, filename option is required. \n");
  printf("filename:\n \tfilename to proceed requested operation\n");
}

void generateKeys() {
  
  EVP_PKEY *pkey = EVP_RSA_gen(2048);
  
  if (pkey == NULL) {
    fprintf(stderr, "error: rsa gen\n");
    ERR_print_errors_fp(stderr);
    return;
  }
  
  FILE *fp = fopen("public.txt", "wt");
  
  if (fp != NULL) {
    PEM_write_PUBKEY(fp, pkey);
    fclose(fp);
  } else {
    perror("file error");
  }
  
  fp = fopen("private.txt", "wt");
  
  if (fp != NULL) {
    PEM_write_PrivateKey(fp, pkey, NULL, NULL, 0, NULL, NULL);
    fclose(fp);
  } else {
    perror("file error");
  }

  EVP_PKEY_free(pkey);
}

uchar *encrypt(uchar *src, uint len, int *length) {
  
  FILE *fp = fopen("public.txt", "r");

  if (fp == NULL) {
    perror("file error");
    return NULL;
  }

  EVP_PKEY *pkey;
  pkey = PEM_read_PUBKEY(fp, NULL, NULL, NULL);
  
  fclose(fp);
  
  if (pkey == NULL) {
    fprintf(stderr, "error: read publics key\n");
    return NULL;
  }

  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey, NULL);
  
  EVP_PKEY_encrypt_init(ctx);
  
  uchar *dst = (uchar *)malloc(2048);
  size_t outl;
  
  if (!EVP_PKEY_encrypt(ctx, dst, &outl, src, (size_t)len)) {
    fprintf(stderr, "error: encrypt\n");
    EVP_PKEY_free(pkey);
    free(dst);
    return NULL;
  }
  
  int len2 = outl;
  
  EVP_PKEY_free(pkey);
  EVP_PKEY_CTX_free(ctx);
  BIO_dump_fp(stdout, dst, len2);
  
  printf("len: %d, len2: %d\n", len, len2);
  
  if (length != NULL) {
    *length = len2;
  }
  
  return dst;
}

uchar *decrypt(uchar *src, int len, int* outlen) {
  
  FILE *fp = fopen("private.txt", "r");
  
  if (fp == NULL) {
    perror("file error");
    return NULL;
  }
  
  EVP_PKEY *pkey = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
  
  fclose(fp);
  
  if (pkey == NULL) {
    fprintf(stderr, "error: read private key\n");
    return NULL;
  }

  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey, NULL);
  EVP_PKEY_decrypt_init(ctx);
  
  uchar *dst = (uchar *)malloc(2048);
  size_t outl=2048;
  size_t inl = len;
  
  if (!EVP_PKEY_decrypt(ctx, dst, &outl, src, inl)) {
    fprintf(stderr, "error: decrypt\n");
    free(dst);
    dst = NULL;
  } else {
    BIO_dump_fp(stdout, dst, (int)outl);
    printf("len: %d, outl: %lu\n", len, outl);
  }
  *outlen = outl;

  EVP_PKEY_free(pkey);
  EVP_PKEY_CTX_free(ctx);
  
  return dst;
}

int main(int argc, char** argv) { 

  argv2=argv;

  char msg[] = "Hello openssl!";
  int len;

  
  if( argc == 2 && argv[1][0]=='1')
  {
    generateKeys(); 
  }
  else if(argc == 3)
  {
    char *filename = argv[2];

    FILE *fp = fopen(filename, "r");

    fseek(fp, 0L, SEEK_END);
    int sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);


    char* data = malloc(sz);
    fread(data, 1, sz, fp);

    if(argv[1][0]=='2')
    {

      printf("Encrypting\n\n");
      uchar *msg_e = encrypt(data, sz, &len);

      FILE *f = fopen("out/enc.bin", "w");
      fwrite(msg_e, 1, len, f);

    }
    else if(argv[1][0]=='3')
    {

      int outlen =0;

      printf("decrypting\n");
      uchar *msg_d = decrypt( data, sz, &outlen);
      msg_d[outlen] = '\0';
      printf("%s", msg_d);

      FILE *f = fopen("out/dec.txt", "w");
      fwrite(msg_d, 1, outlen, f);
    }
    else{
      printErr();
    }
  }
  else{
    printErr();
  }
}
