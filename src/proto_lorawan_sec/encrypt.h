/**
 * @ingroup     IoT Course RIOT Security Hands-on Tutorials
 * @{
 *
 * @file        encrypt.h
 * @brief       Tutorial on AES - AES Functions definitions
 *
 * @author      Stefano Milani <milani@diag.uniroma1.it>
 *
 * @}
 */

#ifndef ENCRYPT_H
#define ENCRYPT_H

void print_bytes(const uint8_t* key, size_t size);

int make_aes_128_key(void);

uint8_t *aes_128_encrypt(char* msg, uint8_t *pld_buff);

size_t get_payload_size(void);

#endif
