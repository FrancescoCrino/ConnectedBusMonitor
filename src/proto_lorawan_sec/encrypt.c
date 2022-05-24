
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "encrypt.h"
#include "random.h"
#include "crypto/aes.h"
#include "crypto/modes/cbc.h"
#include "crypto/ciphers.h"

#define PRINT_KEY_LINE_LENGTH 5

cipher_t ctx;
uint8_t aes_key[AES_KEY_SIZE_128];
uint8_t hidden_key[AES_KEY_SIZE_192];
uint8_t *payload;
size_t payload_size;


size_t get_payload_size(void){
    return payload_size;
}
// Print key
void print_bytes(const uint8_t* key, size_t size) {
	for(size_t i = 0; i < size; i++){
		if(i != 0 && i%PRINT_KEY_LINE_LENGTH == 0)
			printf("\n");
		printf("0x%x\t", key[i]);
	}
	printf("\n");
}

// Init aes context
int make_aes_128_key(void) {
    random_bytes(aes_key, AES_KEY_SIZE_128);
    if(cipher_init(&ctx, CIPHER_AES, aes_key, AES_KEY_SIZE_128) != CIPHER_INIT_SUCCESS){
        printf("\nError in initalizing the cipher. Exit with exit code 1\n");
        return 1;
    }
    return 0;
}

// Insert 128bit key in a 192bit payload
static void hide_key(void){
    // Generate the fake key

    random_bytes(hidden_key, AES_KEY_SIZE_192);
    int j = 0;
    for(int i=0; i<AES_KEY_SIZE_192; i++){
        if (i!=2 && i!=5 && i!=8 && i!=11 && i!=14 && i!=17 && i!=20 && i!=23){
            memcpy(hidden_key+i, aes_key+j, 1);
            j++;
        }
    }

    //memcpy(fake_key+4, aes_key, AES_KEY_SIZE_128);

    puts("\n ----- HIDE KEY ----- \n");
    puts("Real key: \n");
    print_bytes(aes_key, AES_KEY_SIZE_128);

    puts("hidden key: \n");
    print_bytes(hidden_key, AES_KEY_SIZE_192);
    //printf("%s", fake_key);
}

static void enc_payload(uint8_t *ciphertext, uint8_t *iv, size_t ciphertext_len, uint8_t *pld_buff){

    puts("\n ----- ENC-PAYLOAD -----\n");

    payload_size = AES_KEY_SIZE_128 + AES_KEY_SIZE_192 + ciphertext_len;

    printf("\n Dimension of the payload: %d \n", payload_size);


    hide_key();

    memcpy(pld_buff, iv, AES_KEY_SIZE_128);
    memcpy(pld_buff+AES_KEY_SIZE_128, hidden_key, AES_KEY_SIZE_192);
    memcpy(pld_buff+AES_KEY_SIZE_128+AES_KEY_SIZE_192, ciphertext, ciphertext_len);

    puts("ENCRYPTED PAYLOAD: \n");


    print_bytes(pld_buff, payload_size);

    printf("\n Payload add: %p \n", pld_buff);


}

// Decrypt message using aes 128 (Parameters to be set)
int aes_128_decrypt(uint8_t *ciphertext, uint8_t *iv, size_t ciphertext_len) {

    puts("\n ----- DECRYPTION -----\n");

    uint8_t output[ciphertext_len];

    cipher_decrypt_cbc(&ctx, iv, ciphertext, ciphertext_len, output);

    printf("%s", output);

	return 0;
}

// Encrypt msg using aes-128
uint8_t *aes_128_encrypt(char* msg, uint8_t *pld_buff) {

    puts("\n ----- ENCRYPTION -----\n");

	size_t plaintext_len = strlen(msg);

	printf("Message to enrypt: %s of len %d\n", msg, plaintext_len);

	if(strlen(msg) % AES_BLOCK_SIZE != 0){
	    int pad_dim = AES_BLOCK_SIZE - (strlen(msg) % AES_BLOCK_SIZE);
	    plaintext_len = strlen(msg) + pad_dim;
    }

    uint8_t plaintext[plaintext_len];
    memset(plaintext, 0x00, plaintext_len);
    memcpy(plaintext, msg, strlen(msg));

    uint8_t ciphertext[plaintext_len];

    uint8_t iv[AES_KEY_SIZE_128];
	random_bytes(iv, AES_KEY_SIZE_128);

	puts("IV: \n");
	print_bytes(iv, AES_KEY_SIZE_128);

    cipher_encrypt_cbc(&ctx, iv, plaintext, plaintext_len, ciphertext);

    puts("CIPHERTEXT: \n");
    print_bytes(ciphertext, plaintext_len);

    enc_payload(ciphertext, iv, plaintext_len, pld_buff);

    return payload;
}


