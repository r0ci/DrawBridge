/*
	Project: Trigger
	Description: Client Crypto wrappers
	Auther: Bradley Landherr
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <sys/types.h>


unsigned char *gen_digest(unsigned char *buf, unsigned int len, unsigned int *olen)
{

    EVP_MD_CTX *ctx = NULL;
    unsigned char *ret;
    const EVP_MD *sha256;

    sha256 = EVP_sha256();
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    EVP_MD_CTX_init(ctx);
#else
    ctx = EVP_MD_CTX_new();
#endif

    if (!(ret = (unsigned char *)malloc(EVP_MAX_MD_SIZE)))
        return NULL;

    if (ctx == NULL) {
	return NULL;
    }

    EVP_DigestInit(ctx, sha256);
    EVP_DigestUpdate(ctx, buf, len);
    EVP_DigestFinal(ctx, ret, olen);

#if OPENSSL_VERSION_NUMBER < 0x10100000L
    EVP_MD_CTX_cleanup(ctx);
#else
    EVP_MD_CTX_free(ctx);
#endif
    return ret;
}


unsigned char *sign_data(RSA * pkey, unsigned char *data, unsigned int len, unsigned int *olen)
{

    unsigned char *sig;
    int rc = 0;

    if (!(sig = calloc(1, RSA_size(pkey)))) {
        perror("calloc");
        return NULL;
    }

	rc = RSA_sign(NID_sha256, data, len, sig, olen, pkey);

	if (1 != rc) { 
		free(sig);
		return NULL;
	}

    return sig;
} 
