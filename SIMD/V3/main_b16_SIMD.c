#include "b16_SIMD.c"
#include <stdio.h>
#include <stdint.h>

typedef struct {
    int32_t row;
    int32_t col;
    unsigned int *data;
} matf16_t;

unsigned int aarr[9] = {0x3f630000, 0x3e380000, 0x3d230000,
              0x3d8f0000, 0x3f800000, 0x3e4c0000,
              0x3e050000, 0x00000000, 0x3f330000};
unsigned int barr[9] = {0x3ca30000, 0x3f810000, 0x3e0f0000,
              0x3dcc0000, 0x3e0f0000, 0x3f7d0000,
              0x3f5e0000, 0x3e4c0000, 0x3db80000};
matf16_t amat = {.row = 3, .col = 3, .data = aarr};
matf16_t bmat = {.row = 3, .col = 3, .data = barr};

int main() {
    /*
    answer should be
    0.0706  0.9321  0.3064
    0.2753  0.2507  1.0178
    0.6116  0.2713  0.0812
    */
    uint32_t start = get_cycles();
    matf16_t retmat = {0, 0, 0};
    unsigned int retdata[9] = {0};
    unsigned int *ptr_ret = &retdata[0];
    matf16_t *cmat = matmul(&amat, &bmat, &retmat, ptr_ret);
    uint32_t end = get_cycles();

    printf("Elapse cycle: %d\n", end-start);
    unsigned int *c = cmat->data;
    printf("result:\n");
    int32_t i = 0;
    do {
        printf("MD_r_h[%d] = 0x%08x\n", i, c[i]);
        i++;
    } while(i < 9);

    return 0;
}