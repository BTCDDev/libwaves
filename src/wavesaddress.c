/*
    2016 BTCDDev
    libwaves - address generation
    mvplineup.com
*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <include/random.h>
#include <include/tweetnacl.h>
#include "../include/wavesaddress.h"
#include "utils/libb2/src/blake2.h"
#include "utils/include/sha256.h"
#include "utils/include/sha3.h"
#include "utils/include/base58.h"

char * GenerateWavesAddressFromSeed(char *seed)
{
    if(strlen(seed) > 2048 || strlen(seed) < 4){
        return NULL;
    }

    const int seed_length = strlen(seed) + 4;

    //Generate Random seed
    unsigned char random[seed_length + 64];
    memset(random, 0, seed_length + 64);

    strcpy(random, seed);

    //fprintf(stderr, "SECRET PHRASE: <%s>\n", seed);

    unsigned char nonce[4];
    //randombytes(nonce, 4);
    memset(nonce, 0, 4);

    unsigned char final_seed[seed_length + 1];
    memset(final_seed, 0, (seed_length+1) * sizeof(unsigned char));
    memcpy(final_seed, nonce, 4 * sizeof(unsigned char));

    memcpy(final_seed+4, random, (seed_length-4) * sizeof(unsigned char));
    int i;
    /*
    for(i=0; i<seed_length; i++){
        fprintf(stderr, "%02x", final_seed[i]);
    }
    fprintf(stderr, "\n");
    */
    //blake25b(nonce+seed)
    uint8_t blake_hash[128];
    blake2b(blake_hash, (void*)final_seed, NULL, 32, seed_length, 0);

    //keccak256(b25b)
    unsigned char k32[32];
    memset(k32,0,32);


    sha3_ctx c;
    rhash_sha3_256_init(&c);
    rhash_sha3_update(&c, blake_hash, 32);
    rhash_sha3_final(&c, k32);

    //SHA256
    unsigned char h[64];
    memset(h, 0, 64);
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, k32, 32);
    sha256_final(&ctx, h);

    h[0] &= 248;
    h[31] &= 127;
    h[31] |= 64;

    //Generate Curve25519 Keypair
    unsigned char q[256];
    memset(q, 0, 256);

    crypto_scalarmult_base(q, h);

    char b58_pub[256],
    b58_priv[256];
    memset(b58_priv, 0, 256);
    memset(b58_pub, 0, 256);

    base58Encode(q,32, b58_pub, 64);
    base58Encode(h, 32, b58_priv, 64);


    char PublicKey[256], PrivateKey[256];
    memset(PublicKey, 0, 256);
    memset(PrivateKey, 0, 256);

    unsigned char *ptrKey = &(b58_pub[0]);
    while((*ptrKey) == '1') ptrKey++;
    strcpy(PublicKey, ptrKey);

    ptrKey = &(b58_priv[0]);
    while((*ptrKey) == '1') ptrKey++;
    strcpy(PrivateKey, ptrKey);
    char *retVal = (char*)malloc(strlen(PublicKey) + strlen(PrivateKey) + 64);
    sprintf(retVal, "{\"public_key\":\"%s\", \"private_key\":\"%s\"}", PublicKey, PrivateKey);
    //fprintf(stderr, "Public Key: <%s>\nPrivate Key: <%s>\n", PublicKey, PrivateKey);

    return retVal;
}

char* GenerateWavesAddressFromSeedHex(char *hex)
{
    if( (strlen(hex) % 2) != 0 || strlen(hex) < 2 || strlen(hex) > 2048 ){
        return -1;
    }
    const int seed_length = strlen(hex)/2 + 4;
    unsigned char byte_array[seed_length + 64];
    memset(byte_array, 0, seed_length + 64);

    int count = 0, i = 0;
    for(count=0; count < seed_length - 4; count++){
        sscanf(hex, "%2hhx", &byte_array[count]);
        hex += 2;
    }
    return GenerateWavesAddressFromSeed(byte_array);
}
