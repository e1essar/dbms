#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <string.h>

int encrypt(unsigned char* text, int text_len, unsigned char* key, unsigned char* cipher) {
	int cipher_len = 0;
	int len = 0;

	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();


	if (!ctx) {
		perror("EVP_CIPHER_CTX_new() failed");
		exit(-1);
	}

	if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL)) {
		perror("EVP_EncryptInit failed");
		exit(-1);
	}

	if (!EVP_EncryptUpdate(ctx, cipher, &len, text, text_len)) {
		perror("EVP_EncryptUpdate failed");
		exit(-1);
	}

	cipher_len += len;

	if (!EVP_EncryptFinal_ex(ctx, cipher + len, &len)) {
		perror("EVP_EncryptFinal failed");
		exit(-1);
	}

	cipher_len += len;

	EVP_CIPHER_CTX_free(ctx);

	return cipher_len;
}

int decrypt(unsigned char* cipher, int cipher_len, unsigned char* key, unsigned char* text) {
	int text_len = 0;
	int len = 0;

	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

	if (!ctx) {
		perror("EVP_CIPHER_CTX_new() failed");
		exit(-1);
	}

	if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL)) {
		perror("EVP_DecryptInit failed");
		exit(-1);
	}

	if (!EVP_DecryptUpdate(ctx, text, &len, cipher, cipher_len)) {
		perror("EVP_DecryptUpdate failed");
		exit(-1);
	}

	text_len += len;

	if (!EVP_DecryptFinal_ex(ctx, text + len, &len)) {
		perror("EVP_DecryptFinal failed");
		exit(-1);
	}

	text_len += len;

	EVP_CIPHER_CTX_free(ctx);

	return text_len;
}

int main() {
	unsigned char* key = (unsigned char*)"0123456789abcdef";
	unsigned char* text = (unsigned char*)"Hello World!";

	int text_len = strlen((const char*)text);
	unsigned char cipher[64];

	printf("cipher=\n");

	int cipher_len = encrypt(text, text_len, key, cipher);

	for (int i = 0; i < cipher_len; i++) {
		printf("%02x", cipher[i]);
	}
	printf("\n");

	printf("decrypted=\n");
	unsigned char decrypted[64];

	int dec_len = decrypt(cipher, cipher_len, key, decrypted);

	for (int i = 0; i < dec_len; i++) {
		printf("%c", (const char)decrypted[i]);
	}
	printf("\n");
}