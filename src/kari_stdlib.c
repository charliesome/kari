#include "kari.h"
#include "kari_stdlib.h"
#include <stdlib.h>
#include <stdio.h>

K_FN(hello)
{
    printf("Hello world from Kari\n");
    return kari_nil();
}