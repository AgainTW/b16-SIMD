#ifndef _B16_M_SIMD_OPT_
#define _B16_M_SIMD_OPT_

struct _matf16_t
{
    int32_t row;
    int32_t col;
    unsigned int *data;
};
typedef struct _matf16_t matf16_t;

uint32_t highestbit(uint32_t x);

unsigned int m_mul(unsigned int a, unsigned int b);

unsigned int muti_data_mul(unsigned int a, unsigned int b);

unsigned int hhw_single_data_add(unsigned int a, unsigned int b);

matf16_t *matmul(matf16_t *first, matf16_t *second, matf16_t *retmat, unsigned int *retdata);

#endif
