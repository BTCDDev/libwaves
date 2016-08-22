#include <stdio.h>
#include <stdlib.h>
#include "../../include/wavesaddress.h"

int main()
{
    char *b = GenerateWavesAddressFromSeed("deadbeef");
    char *f = GenerateWavesAddressFromSeedHex("6465616462656566");
    printf("%s\n%s\n", b, f);
    free(b);
    free(f);
    return 0;
}
