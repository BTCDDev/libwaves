#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

#include "include/random.h"

#ifndef _WIN32

// from tweetnacl
void randombytes(unsigned char *x,int xlen)
{
    static int fd = -1;
    int32_t i;
    if (fd == -1) {
        for (;;) {
            fd = open("/dev/urandom",O_RDONLY);
            if (fd != -1) break;
            sleep(1);
        }
    }
    while (xlen > 0) {
        if (xlen < 1048576) i = (int32_t)xlen; else i = 1048576;
        i = (int32_t)read(fd,x,i);
        if (i < 1) {
            sleep(1);
            continue;
        }
        x += i;
        xlen -= i;
    }
}

#else
#include <inttypes.h>
#include <windows.h>
#include <wincrypt.h>
#include <process.h>
#include <tlhelp32.h>
#include <time.h>
// from tweetnacl
void randombytes(unsigned char *x,int xlen)
{
    HCRYPTPROV prov = 0;
    CryptAcquireContextW(&prov, NULL, NULL,PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT);
    CryptGenRandom(prov, xlen, x);
    CryptReleaseContext(prov, 0);
}
#endif
