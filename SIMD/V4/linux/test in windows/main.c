#include <stdio.h>
#include <stdint.h>
#include "b16_SIMD_opt.h"

//struct _matf16_t;  
typedef struct _matf16_t matf16_t;

unsigned int aarr[9] = {0x3f630000, 0x3e380000, 0x3d230000,
              0x3d8f0000, 0x3f800000, 0x3e4c0000,
              0x3e050000, 0x00000000, 0x3f330000};
unsigned int barr[9] = {0x3ca30000, 0x3f810000, 0x3e0f0000,
              0x3dcc0000, 0x3e0f0000, 0x3f7d0000,
              0x3f5e0000, 0x3e4c0000, 0x3db80000};
matf16_t amat = {.row = 3, .col = 3, .data = aarr};
matf16_t bmat = {.row = 3, .col = 3, .data = barr};

int main(int argc, char** argv) {
    /*
        answer should be
        0.0706  0.9321  0.3064
        0.2753  0.2507  1.0178
        0.6116  0.2713  0.0812
    */
    /*
        MD_r_h[0] = 0x3d8f0000
        MD_r_h[1] = 0x3f6c0000
        MD_r_h[2] = 0x3e9b0000
        MD_r_h[3] = 0x3e8c0000
        MD_r_h[4] = 0x3e7f0000
        MD_r_h[5] = 0x3f820000s
        MD_r_h[6] = 0x3f1b0000
        MD_r_h[7] = 0x3e8a0000
        MD_r_h[8] = 0x3da50000
    */
    //uint32_t start = get_cycles();
    matf16_t retmat = {0, 0, 0};
    unsigned int retdata[9] = {0};
    unsigned int *ptr_ret = &retdata[0];
    matf16_t *cmat = matmul(&amat, &bmat, &retmat, ptr_ret);
    //uint32_t end = get_cycles();

    //printf("Elapse cycle: %d\n", end-start);
    unsigned int *c = cmat->data;
    printf("result:\n");
    int32_t i = 0;
    do {
        printf("MD_r_h[%d] = 0x%08x\n", i, c[i]);
        i++;
    } while(i < 9);

    return 0;
}
